#include "PragmaSessionV2.h"

#include "PragmaConnection.h"
#include "PragmaWebSocket.h"

namespace Pragma
{

FSession::FSession(FSdkConfigStrongPtr Config, TSharedPtr<IPragmaTimerManager> TimerManager)
	: ConfigPtr{MoveTemp(Config)}
	, TimerManagerPtr{MoveTemp(TimerManager)}
{
}

FSession::~FSession()
{
	if (ConnectionPtr.IsValid() && ConnectionPtr->IsInitialized())
	{
		ConnectionPtr->Disconnect();

		// Forcibly clean up the Connection and Protocols before Services so they can flush delegates.
		ConnectionPtr->CleanupBeforeDestruction();
	}
}

FString FSession::GameShardId() const
{
	if (!IsInitialized())
	{
		return FString{};
	}
	return Connection().GetGameShardId();
}

bool FSession::IsInitialized() const
{
	return ConnectionPtr.IsValid() && ConnectionPtr->IsInitialized();
}

bool FSession::IsConnecting() const
{
	if (!IsInitialized())
	{
		return false;
	}
	return Connection().IsInitializing() || Connection().IsConnecting();
}

bool FSession::IsConnected() const
{
	if (!IsInitialized())
	{
		return false;
	}
	return Connection().IsConnected() && !Connection().IsConnecting();
}

FConnectionInfo FSession::ConnectionInfo() const
{
	if (!IsInitialized()) return {};

	FConnectionInfo Info;
	Info.Status = Connection().GetStatus();
	if (Config().GetProtocolType(false) == EPragmaProtocolType::WebSocket)
	{
		const auto& SocialWebSocket = Connection().WebSocket(EPragmaBackendType::Social);
		const auto& GameWebSocket = Connection().WebSocket(EPragmaBackendType::Game);

		Info.Social.Status = SocialWebSocket.GetConnectionStatus();
		Info.Game.Status = GameWebSocket.GetConnectionStatus();
		if (EPragmaConnectionStatus::Degraded == Info.Status)
		{
			Info.Social.SecondsUntilReconnect = SocialWebSocket.SecondsUntilReconnect();
			Info.Game.SecondsUntilReconnect = GameWebSocket.SecondsUntilReconnect();

			const auto SecsUntilDc = Connection().RemainingSecondsUntilFullDisconnect();
			if (SecsUntilDc.IsSet())
			{
				Info.SecondsUntilTimeout = SecsUntilDc.GetValue();
			}
		}
	}

	const auto LastConnectionError = Connection().GetLastConnectionError();
	if (LastConnectionError.IsSet())
	{
		Info.DisconnectError = LastConnectionError.GetValue().ToPragmaError().ToString();
		Info.Social.ConnectionError = LastConnectionError.GetValue().SocialBackendError;
		Info.Game.ConnectionError = LastConnectionError.GetValue().GameBackendError;
	}

	return Info;
}

FPragmaProtocol& FSession::SocialBackend() const
{
	return Connection().SocialBackend();
}

FPragmaProtocol& FSession::GameBackend() const
{
	return Connection().GameBackend();
}

void FSession::InitLegacySession()
{
	if (IsInitialized())
	{
		PRAGMA_LOG(Warning, "FSession::Init -- Called while already initialized.");
		return;
	}

	ConnectionPtr = TStrongObjectPtr<UPragmaConnection>{NewObject<UPragmaConnection>()};
	ConnectionPtr->bIsPartnerSession = IsPartnerSession();
	ConnectionPtr->Init(ConfigPtr.Get(), TimerManagerPtr);

	ConnectionPtr->OnConnected.AddLambda(WeakLambda(this, {[this]{ OnConnected.Broadcast(); }}));
	ConnectionPtr->OnPreDisconnect.AddLambda(WeakLambda(this, {[this]{ OnDisconnected.Broadcast(); }}));
	ConnectionPtr->OnDegraded.AddLambda(WeakLambda<TOptional<FPragmaConnectionError>>(this, {[this](auto){ OnDegraded.Broadcast(); }}));
	ConnectionPtr->OnRecovered.AddLambda(WeakLambda(this, {[this]{ OnReconnected.Broadcast(); }}));
	ConnectionPtr->OnSocialStatusChanged.AddLambda(WeakLambda<EPragmaConnectionStatus>(this, {[this](auto Status){ OnSocialStatusChanged.Broadcast(Status); }}));
	ConnectionPtr->OnGameStatusChanged.AddLambda(WeakLambda<EPragmaConnectionStatus>(this, {[this](auto Status){ OnGameStatusChanged.Broadcast(Status); }}));

	OnInitLegacy();
	OnInitialized.Broadcast();
}
}
