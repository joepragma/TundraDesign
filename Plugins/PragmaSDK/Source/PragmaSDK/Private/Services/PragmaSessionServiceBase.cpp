#include "Services/PragmaSessionServiceBase.h"
#include "Services/PragmaSessionServiceRawBase.h"
#include "PragmaNotificationFuture.h"
#include "PragmaPlayer.h"
#include "Dto/PragmaPartnerSessionRpcDto.h"

DEFINE_LOG_CATEGORY_STATIC(LogPragmaSessionSvc, Error, All);

#define SESSION_SVC_LOG(Verbosity, Format, ...) { PRAGMA_BASE_LOG(LogPragmaSessionSvc, Verbosity, Format, ##__VA_ARGS__); }

namespace Pragma
{
void FSessionServiceBase::SetDependencies(const FSessionPtr InSession, FSessionServiceRawBase* InRawService, const TSharedPtr<IPragmaTimerManager>& InTimerManager, const float InHeartbeatPeriodSeconds)
{
	Session = InSession;
	RawService = InRawService;
	TimerManager = InTimerManager;
	HeartbeatPeriodSeconds = InHeartbeatPeriodSeconds;
}

void FSessionServiceBase::Initialize()
{
	Raw().OnSocialSessionChangedV1Notification.AddWeakLambda(WeakThisBinding(), [this](auto Notification, auto Metadata){ HandleSocialSessionChanged(Notification, Metadata); });
	Raw().OnGameSessionChangedV1Notification.AddWeakLambda(WeakThisBinding(), [this](auto Notification, auto Metadata){ HandleGameSessionChanged(Notification, Metadata); });

	CreateHeartbeats();
	BindHeartbeats();
	// Currently unbound.
	// Raw().OnSocialSessionTerminatedV1Notification.AddUObject(this, &FSessionServiceBase::HandleSocialSessionTerminated);
	// Raw().OnGameSessionTerminatedV1Notification.AddUObject(this, &FSessionServiceBase::HandleGameSessionTerminated);
}

FSessionServiceRawBase& FSessionServiceBase::Raw() const
{
	return *RawService;
}

void FSessionServiceBase::CreateHeartbeats()
{
	SocialHeartbeat = FPragmaHeartbeat::Create(TimerManager);
	SocialHeartbeat->HeartbeatPeriodSeconds = HeartbeatPeriodSeconds;
	GameHeartbeat = FPragmaHeartbeat::Create(TimerManager);
	GameHeartbeat->HeartbeatPeriodSeconds = HeartbeatPeriodSeconds;
}

void FSessionServiceBase::BindHeartbeats()
{
	Session->OnSocialStatusChanged.AddWeakLambda(WeakThisBinding(), [this](const EPragmaConnectionStatus Status)
	{
		switch (Status)
		{
		case EPragmaConnectionStatus::NotConnected: // fallthrough
		case EPragmaConnectionStatus::Degraded:
			SocialHeartbeat->Stop();
			break;
		case EPragmaConnectionStatus::Connected:
			SocialHeartbeat->Start();
			break;
		case EPragmaConnectionStatus::Connecting: // fallthrough
		default:
			break;
		}
	});
	Session->OnGameStatusChanged.AddWeakLambda(WeakThisBinding(), [this](const EPragmaConnectionStatus Status)
	{
		switch (Status)
		{
		case EPragmaConnectionStatus::NotConnected: // fallthrough
		case EPragmaConnectionStatus::Degraded:
			GameHeartbeat->Stop();
			break;
		case EPragmaConnectionStatus::Connected:
			GameHeartbeat->Start();
			break;
		case EPragmaConnectionStatus::Connecting: // fallthrough
		default:
			break;
		}
	});

	if (Session->IsPartnerSession())
	{
		SocialHeartbeat->BeatActionDelegate.BindWeakLambda(WeakThisBinding(), [this]
		{
			// The FSessionPtr and FSessionServiceRawBase* share the same lifecycle.
			// This lambda could fire after the FSessionServiceRawBase* has gone away.
			if (!Session.IsValid()) { return; }
			Raw().SocialHeartbeatPartnerV1(FPragma_Session_HeartbeatPartnerV1Request{},
				FSessionServiceRawBase::FHeartbeatPartnerV1Delegate());
		});
	}
	else
	{
		SocialHeartbeat->BeatActionDelegate.BindWeakLambda(WeakThisBinding(), [this]
		{
			// The FSessionPtr and FSessionServiceRawBase* share the same lifecycle.
			// This lambda could fire after the FSessionServiceRawBase* has gone away.
			if (!Session.IsValid()) { return; }
			Raw().SocialHeartbeatV1(FPragma_Session_HeartbeatV1Request{},
				FSessionServiceRawBase::FHeartbeatV1Delegate());
		});
	}

	if (Session->IsPartnerSession())
	{
		GameHeartbeat->BeatActionDelegate.BindWeakLambda(WeakThisBinding(), [this]()
		{
			// The FSessionPtr and FSessionServiceRawBase* share the same lifecycle.
			// This lambda could fire after the FSessionServiceRawBase* has gone away.
			if (!Session.IsValid()) { return; }
			Raw().GameHeartbeatPartnerV1(FPragma_Session_HeartbeatPartnerV1Request{},
				FSessionServiceRawBase::FHeartbeatPartnerV1Delegate());
		});
	}
	else
	{
		GameHeartbeat->BeatActionDelegate.BindWeakLambda(WeakThisBinding(), [this]()
		{
			// The FSessionPtr and FSessionServiceRawBase* share the same lifecycle.
			// This lambda could fire after the FSessionServiceRawBase* has gone away.
			if (!Session.IsValid()) { return; }
			Raw().GameHeartbeatV1(FPragma_Session_HeartbeatV1Request{},
				FSessionServiceRawBase::FHeartbeatV1Delegate());
		});
	}
}

FSessionServiceBase::FAttributeChanged& FSessionServiceBase::GetAttributeChangedDelegate(
	EPragma_SocialSessionAttribute Attribute)
{
	return SocialSessionData.Events.FindOrAdd(static_cast<int32>(Attribute));
}

FSessionServiceBase::FAttributeChanged& FSessionServiceBase::GetAttributeChangedDelegate(
	EPragma_GameSessionAttribute Attribute)
{
	return GameSessionData.Events.FindOrAdd(static_cast<int32>(Attribute));
}

const FString* FSessionServiceBase::GetAttribute(EPragma_SocialSessionAttribute Attribute)
{
	return SocialSessionData.Attributes.Find(static_cast<int32>(Attribute));
}

const FString* FSessionServiceBase::GetAttribute(EPragma_GameSessionAttribute Attribute)
{
	return GameSessionData.Attributes.Find(static_cast<int32>(Attribute));
}

void FSessionServiceBase::HandleSocialSessionTerminated(
	const FPragma_Session_SessionTerminatedV1Notification Notification,
	const FPragmaMessageMetadata& Metadata)
{
	// Note: we don't do any handling here currently as the sockets will be closed anyway.
	// Eventually this will be used for broadcasting relevant events (e.g. kicked, server shutdown).
	//
	// When this is implemented be sure to uncomment the AddNotificationHandler in OnInitialized.
}

void FSessionServiceBase::HandleGameSessionTerminated(FPragma_Session_SessionTerminatedV1Notification Notification,
	const FPragmaMessageMetadata& Metadata)
{
	// Note: we don't do any handling here currently as the sockets will be closed anyway.
	// Eventually this will be used for broadcasting relevant events (e.g. kicked, server shutdown).
	//
	// When this is implemented be sure to uncomment the AddNotificationHandler in OnInitialized.
}

void FSessionServiceBase::HandleSocialSessionChanged(FPragma_Session_SessionChangedV1Notification Notification,
	const FPragmaMessageMetadata& Metadata)
{
	HandleSessionChangedNotification(SocialSessionData, MoveTemp(Notification));
	OnSocialSessionChanged.Broadcast(SocialSessionData.Attributes);
}

void FSessionServiceBase::HandleGameSessionChanged(FPragma_Session_SessionChangedV1Notification Notification,
	const FPragmaMessageMetadata& Metadata)
{
	HandleSessionChangedNotification(GameSessionData, MoveTemp(Notification));
	OnGameSessionChanged.Broadcast(GameSessionData.Attributes);
}

void FSessionServiceBase::HandleSessionChangedNotification(FSessionData& SessionData,
	FPragma_Session_SessionChangedV1Notification Notification)
{
	if (Notification.NewVersion <= SessionData.Version)
	{
		SESSION_SVC_LOG(Warning,
			"Ignoring out-of-date SessionChangedV1Notification with version %d. Current Version is %d",
			Notification.NewVersion, SessionData.Version);
		return;
	}
	SessionData.Version = Notification.NewVersion;

	SessionData.Attributes.Reserve(SessionData.Attributes.Num() + Notification.AttributesSet.Num());
	for (auto& Pair : Notification.AttributesSet)
	{
		SessionData.Attributes.Add(Pair.Key, Pair.Value);
		BroadcastAttributeChanged(SessionData.Events, Pair.Key, &Pair.Value);
	}
	for (auto& Key : Notification.AttributesRemoved)
	{
		if(SessionData.Attributes.Contains(Key))
		{
			SessionData.Attributes.Remove(Key);
			BroadcastAttributeChanged(SessionData.Events, Key, nullptr);
		}
	}
}

void FSessionServiceBase::BroadcastAttributeChanged(const TMap<int32, FAttributeChanged>& Events,
	const int32 Key,
	const FString* AttributeValue)
{
	auto* Event = Events.Find(Key);
	if (Event != nullptr)
	{
		Event->Broadcast(AttributeValue);
	}
}
}
