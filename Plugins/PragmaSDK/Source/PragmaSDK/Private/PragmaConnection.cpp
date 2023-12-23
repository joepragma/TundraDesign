#include "PragmaConnection.h"

#include "PragmaSdkConfig.h"
#include "PragmaWebSocket.h"
#include "PragmaHttp.h"
#include "PragmaLog.h"
#include "PragmaBackendTypeConverter.h"
#include "PragmaTimerManager.h"
#include "Dto/PragmaConfigDto.h"

#if !UE_BUILD_SHIPPING
#include "Debug/PragmaActiveDebugServer.h"
#endif

namespace
{
FString BuildBaseUrl(const FPragmaBackendInfo& Info, const EPragmaProtocolType Protocol)
{
	return FString::Printf(TEXT("%s://%s:%d"),
		(Protocol == EPragmaProtocolType::Http ? *Info.Protocol : *Info.WebSocketProtocol), *Info.Host, Info.Port);
}

FString BuildApiUrl(const FPragmaBackendInfo& Info, const EPragmaProtocolType Protocol)
{
	return FString::Printf(TEXT("%s/v1/rpc"), *BuildBaseUrl(Info, Protocol));
}

void ApplyBackendInfoOverrides(const UPragmaSdkConfig& Config,
	const EPragmaBackendType BackendType,
	FPragmaBackendInfo& BackendInfo)
{
	if (BackendType == EPragmaBackendType::Game)
	{
		if (!Config.OverrideGameProtocol.IsEmpty())
		{
			BackendInfo.Protocol = Config.OverrideGameProtocol;
		}
		if (!Config.OverrideGameHost.IsEmpty())
		{
			BackendInfo.Host = Config.OverrideGameHost;
		}
		if (Config.OverrideGamePort > 0)
		{
			BackendInfo.Port = Config.OverrideGamePort;
		}
	}
	if (BackendType == EPragmaBackendType::Social)
	{
		if (!Config.OverrideSocialProtocol.IsEmpty())
		{
			BackendInfo.Protocol = Config.OverrideSocialProtocol;
		}
		if (!Config.OverrideSocialHost.IsEmpty())
		{
			BackendInfo.Host = Config.OverrideSocialHost;
		}
		if (Config.OverrideSocialPort > 0)
		{
			BackendInfo.Port = Config.OverrideSocialPort;
		}
	}
}

void CleanupProtocol(FPragmaProtocol* Protocol)
{
	if (Protocol != nullptr)
	{
		Protocol->CleanupBeforeDestruction();
	}
}
}

FPragmaError FPragmaConnectionError::ToPragmaError() const
{
	if (SocialBackendError != EPragmaProtocolError::None)
	{
		return EProtocolErrorToPragmaError(SocialBackendError);
	}
	return EProtocolErrorToPragmaError(GameBackendError);
}

void UPragmaConnection::SetBackendAddress(FString InBackendAddress)
{
	if (Status != EStatus::WaitingForBackendAddress)
	{
		PRAGMA_LOG(Error,
			"UPragmaConnection::SetBackendAddress -- Backend Address already initialized. SetBackendAddress is available external to the"
			" SDK for development setups. Use configuration via UPragmaSdkConfig to set Backend Address for shipping builds.");
		return;
	}
	BackendAddress = MoveTemp(InBackendAddress);
	PRAGMA_LOG(Log, "UPragmaConnection::SetBackendAddress -- Backend Address set to %s.", *BackendAddress);
	Status = EStatus::ReadyForBackendConfig;
}

FPragmaProtocol& UPragmaConnection::SocialBackend() const
{
	return Protocol(Config().GetProtocolType(bIsPartnerSession), EPragmaBackendType::Social);
}

FPragmaProtocol& UPragmaConnection::GameBackend() const
{
	return Protocol(Config().GetProtocolType(bIsPartnerSession), EPragmaBackendType::Game);
}

void UPragmaConnection::SetAuthTokens(FString InPragmaSocialToken, FString InPragmaGameToken)
{
	PragmaSocialToken = MoveTemp(InPragmaSocialToken);
	PragmaGameToken = MoveTemp(InPragmaGameToken);
	SocialWebSocket->SetAuthToken(PragmaSocialToken);
	GameWebSocket->SetAuthToken(PragmaGameToken);
	SocialHttp->SetAuthToken(PragmaSocialToken);
	GameHttp->SetAuthToken(PragmaGameToken);
	OnAuthTokenChanged.Broadcast(PragmaSocialToken, PragmaGameToken);
}

void UPragmaConnection::Init(UPragmaSdkConfig* InSdkConfig, const TSharedPtr<IPragmaTimerManager>& InTimerManager)
{
	if (Status != EStatus::NotInitialized)
	{
		PRAGMA_LOG(Error, "UPragmaConnection::Init -- Init already called.");
		return;
	}

	// Debug server force to always be inactive in shipping builds.
#if !UE_BUILD_SHIPPING
	if (InSdkConfig->GetDebugServerEnabled())
	{
		DebugServer = MakeShared<FPragmaActiveDebugServer>();
		static_cast<FPragmaActiveDebugServer&>(*DebugServer).Init(InSdkConfig, this);
	}
#endif

	BackendTypeConverter = MakeShared<FPragmaBackendTypeConverter>();
	SdkConfig = InSdkConfig;
	TimerManager = InTimerManager;

	SocialBackoff = FPragmaBackoff{TimerManager};
	ConfigureBackoff(SocialBackoff.GetValue());
	SocialWebSocket = MakeShared<FPragmaWebSocket>(EPragmaBackendType::Social, &SocialBackoff.GetValue(), Config().GetLogPragmaDetailedMessages());
	SocialHttp = MakeShared<FPragmaHttp>(EPragmaBackendType::Social);

	GameBackoff = FPragmaBackoff{TimerManager};
	ConfigureBackoff(GameBackoff.GetValue());
	GameWebSocket = MakeShared<FPragmaWebSocket>(EPragmaBackendType::Game, &GameBackoff.GetValue(), Config().GetLogPragmaDetailedMessages());
	GameHttp = MakeShared<FPragmaHttp>(EPragmaBackendType::Game);

	if (!SdkConfig->GetPartnerSessionGameAuthToken().IsEmpty())
	{
		GameHttp->SetAuthToken(SdkConfig->GetPartnerSessionGameAuthToken());
	}
	if (!SdkConfig->GetPartnerSessionSocialAuthToken().IsEmpty())
	{
		SocialHttp->SetAuthToken(SdkConfig->GetPartnerSessionSocialAuthToken());
	}

	const auto ConverterRef = BackendTypeConverter->AsShared();
	SocialHttp->SetBackendTypeConverter(ConverterRef);
	GameHttp->SetBackendTypeConverter(ConverterRef);
	SocialWebSocket->SetBackendTypeConverter(ConverterRef);
	GameWebSocket->SetBackendTypeConverter(ConverterRef);

	SocialWebSocket->SetDebugServer(DebugServer);
	GameWebSocket->SetDebugServer(DebugServer);
	SocialHttp->SetDebugServer(DebugServer);
	GameHttp->SetDebugServer(DebugServer);

	// Bind event handlers.
	SocialWebSocket->OnConnected().AddUObject(this, &UPragmaConnection::HandleConnected);
	SocialWebSocket->OnDegraded().AddUObject(this, &UPragmaConnection::HandleDegraded);
	SocialWebSocket->OnDisconnected().AddUObject(this, &UPragmaConnection::HandleDisconnected);
	GameWebSocket->OnConnected().AddUObject(this, &UPragmaConnection::HandleConnected);
	GameWebSocket->OnDegraded().AddUObject(this, &UPragmaConnection::HandleDegraded);
	GameWebSocket->OnDisconnected().AddUObject(this, &UPragmaConnection::HandleDisconnected);

	Status = EStatus::WaitingForBackendAddress;

	// Try pulling backend address from config.
	const FString& HttpAddress = SdkConfig->GetBackendAddress(bIsPartnerSession);
	if (HttpAddress.IsEmpty())
	{
		PRAGMA_LOG(Warning, "UPragmaConnection::Init -- No BackendAddress found in configuration or command line."
			" This is ok for development as long as you manually set the BackendAddress using SetBackendAddress()");
		return;
	}
	SetBackendAddress(HttpAddress);

	InitResult = TPragmaResult<>::Success();
}

EPragmaConnectionStatus UPragmaConnection::GetStatus() const
{
	switch(Status)
	{
		case EStatus::ReadyForBackendConfig:
		case EStatus::RequestedBackendConfig:
		case EStatus::Connecting:
			return  EPragmaConnectionStatus::Connecting;
		case EStatus::Connected:
			return EPragmaConnectionStatus::Connected;
		case EStatus::Degraded:
			return EPragmaConnectionStatus::Degraded;
		case EStatus::NotInitialized:
		case EStatus::WaitingForBackendAddress:
		case EStatus::NotConnected:
		default:
			return EPragmaConnectionStatus::NotConnected;
	}
}

void UPragmaConnection::CleanupBeforeDestruction() const
{
	CleanupProtocol(SocialHttp.Get());
	CleanupProtocol(GameHttp.Get());
	CleanupProtocol(SocialWebSocket.Get());
	CleanupProtocol(GameWebSocket.Get());
}

void UPragmaConnection::UpdateBackendConfig(const TFunction<void(TPragmaResult<>)> OnComplete)
{
	if (BackendAddress.IsEmpty())
	{
		PRAGMA_LOG(Error, "UPragmaConnection::UpdateBackendConfig -- Must set BackendAddress, either through"
			" UPragmaSdkConfig or SetBackendAddress() before calling any service methods.");
		OnComplete(FPragmaError{EPragma_SdkError::InvalidConnectionState});
		return;
	}
	if (Status < EStatus::ReadyForBackendConfig)
	{
		PRAGMA_LOG(Error, "UPragmaConnection::UpdateBackendConfig -- Must call Init before anything else.");
		OnComplete(FPragmaError{EPragma_SdkError::InvalidConnectionState});
		return;
	}
	if (Status >= EStatus::Initialized)
	{
		// Must always refresh v1 types in case platform restarted.
		RefreshV1Types(OnComplete);
		return;
	}
	// Collect inbound fns in a delegate so we can broadcast to multiple, e.g. if this is called multiple times
	// in a row they should all get the result from the in-progress call.
	OnBackendConfigComplete.AddWeakLambda(this, OnComplete);
	if (Status == EStatus::RequestedBackendConfig)
	{
		return;
	}
	Status = EStatus::RequestedBackendConfig;

	PRAGMA_LOG(Log, "UPragmaConnection::UpdateBackendConfig -- Updating backend configuration...");

	// Use a temporary http object since HttpAddress isn't necessarily the address we should use for Game or Social backends.
	if (InitializerHttp == nullptr)
	{
		InitializerHttp = MakeShared<FPragmaHttp>(EPragmaBackendType::Game);
	}
	InitializerHttp->Init(BackendAddress, FString{});
	InitializerHttp->SetDebugServer(DebugServer);

	// Get backend info to construct correct Social/Game backend urls.
	InitializerHttp->HttpGet<FPragmaInfoV1Response>(TEXT("/v1/info"),
		[this](TPragmaResult<FPragmaInfoV1Response> Result)
		{
			if (Status != EStatus::RequestedBackendConfig)
			{
				// Other call probably failed and changed state already.
				return;
			}
			if (Result.IsFailure())
			{
				PRAGMA_LOG(Error,
					"UPragmaConnection::UpdateBackendConfig -- Failed to retrieve backend info. Will be unable to"
					" connect to Pragma backend. Check to make sure the PragmaBackendAddress provided is correct!");
				Status = EStatus::ReadyForBackendConfig;
				InfoInitConfigState = EInitConfigState::Error;
				InitResult = TPragmaResult<>::Failure(Result.Error());
				TryFinishInit();
				return;
			}
			FPragmaInfoV1Response& Response = Result.Payload();
			PRAGMA_LOG(Log, "Received Pragma Backend Info. Name: %s, version: %s.", *Response.Name, *Response.Version);

			ApplyBackendInfoOverrides(Config(), EPragmaBackendType::Social, Response.SocialBackend);
			ApplyBackendInfoOverrides(Config(), EPragmaBackendType::Game, Response.GameBackend);

			SocialHttp->Init(
				BuildBaseUrl(Response.SocialBackend, EPragmaProtocolType::Http),
				BuildApiUrl(Response.SocialBackend, EPragmaProtocolType::Http)
			);
			GameHttp->Init(
				BuildBaseUrl(Response.GameBackend, EPragmaProtocolType::Http),
				BuildApiUrl(Response.GameBackend, EPragmaProtocolType::Http)
			);
			SocialWebSocket->Init(BuildApiUrl(Response.SocialBackend, EPragmaProtocolType::WebSocket));
			GameWebSocket->Init(BuildApiUrl(Response.GameBackend, EPragmaProtocolType::WebSocket));

			InfoInitConfigState = EInitConfigState::Received;

			GameShardId = Response.GameShardId;
			TryFinishInit();
		});

	// InitializerHttp can be a nullptr if TryFinishInit is called. This can be triggered when Initializer->HttpGet calls its callback synchronously (in tests).
	if (InitializerHttp == nullptr)
	{
		return;
	}
	// Get Types info to pass along to Protocols.
	InitializerHttp->HttpGet<FPragmaTypesV1Response>(TEXT("/v1/types"),
		[this](const TPragmaResult<TSharedPtr<FJsonObject>> Result)
		{
			if (Status != EStatus::RequestedBackendConfig)
			{
				// Other call probably failed and changed state already.
				return;
			}
			if (Result.IsFailure())
			{
				PRAGMA_LOG(Error,
					"UPragmaConnection::UpdateBackendConfig -- Failed to retrieve backend types. Will be unable to"
					" connect to Pragma backend. Check to make sure the PragmaBackendAddress provided is correct!");
				Status = EStatus::ReadyForBackendConfig;
				InfoInitConfigState = EInitConfigState::Error;
				InitResult = TPragmaResult<>::Failure(Result.Error());
				TryFinishInit();
				return;
			}

			BackendTypeConverter->LoadBackendIds(Result.Payload());

			TypesInitConfigState = EInitConfigState::Received;
			TryFinishInit();
		});
}

void UPragmaConnection::Connect(FString InPragmaSocialToken,
	FString InPragmaGameToken,
	const TFunction<void(TPragmaResult<>)> OnComplete)
{
	if (Status != EStatus::NotConnected)
	{
		PRAGMA_LOG(Error, "UPragmaConnection::Connect -- Invalid state to attempt to connect. SessionStatus = %d",
			Status);
		OnComplete(TPragmaResult<>::Failure(FPragmaError{EPragma_SdkError::InvalidConnectionState}));
		return;
	}
	PRAGMA_LOG(Log, "UPragmaConnection::Connect -- Attempting to establish websocket connection...");
	OnConnectedComplete.AddWeakLambda(this, OnComplete);
	LastConnectionError.Reset();
	Status = EStatus::Connecting;
	SetAuthTokens(MoveTemp(InPragmaSocialToken), MoveTemp(InPragmaGameToken));
	SocialWebSocket->Connect();
	GameWebSocket->Connect();
}

TOptional<float> UPragmaConnection::RemainingSecondsUntilFullDisconnect() const
{
	return TimerManager->RemainingSeconds(FullDisconnectTimerHandle);
}

void UPragmaConnection::Disconnect()
{
	SocialWebSocket->Close();
	GameWebSocket->Close();
}

FPragmaProtocol& UPragmaConnection::Protocol(const EPragmaProtocolType Protocol,
	const EPragmaBackendType BackendType) const
{
	return Protocol == EPragmaProtocolType::Http
		       ? static_cast<FPragmaProtocol&>(Http(BackendType))
		       : static_cast<FPragmaProtocol&>(WebSocket(BackendType));
}

FPragmaHttp& UPragmaConnection::Http(const EPragmaBackendType BackendType) const
{
	checkf(Status != EStatus::NotInitialized, TEXT("UPragmaConnection::GetHttp -- Call Init before usage."));
	return BackendType == EPragmaBackendType::Social ? *SocialHttp : *GameHttp;
}

FPragmaWebSocket& UPragmaConnection::WebSocket(const EPragmaBackendType BackendType) const
{
	checkf(Status != EStatus::NotInitialized, TEXT("UPragmaConnection::GetWebSocket -- Call Init before usage."));
	return BackendType == EPragmaBackendType::Social ? *SocialWebSocket : *GameWebSocket;
}

FString UPragmaConnection::GetGameShardId() const
{
	return GameShardId;
}

void UPragmaConnection::TryFinishInit()
{
	const bool bAnyError = (InfoInitConfigState == EInitConfigState::Error
		|| TypesInitConfigState == EInitConfigState::Error);
	const bool bComplete = (InfoInitConfigState != EInitConfigState::None
		&& TypesInitConfigState != EInitConfigState::None);
	if (!bComplete && !bAnyError)
	{
		// Wait for all to finish. Bypass if we errored.
		return;
	}

	if (bAnyError)
	{
		// Go back to Ready state so that we can try again.
		InfoInitConfigState = EInitConfigState::None;
		TypesInitConfigState = EInitConfigState::None;
		Status = EStatus::ReadyForBackendConfig;
	}
	else
	{
		PRAGMA_LOG(Log, "UPragmaConnection::TryFinishInit -- Successfully initialized.");
		Status = EStatus::Initialized;
		OnInitialized.Broadcast();
		OnInitialized.Clear();
	}
	OnBackendConfigComplete.Broadcast(InitResult);
	OnBackendConfigComplete.Clear();
	InitResult = TPragmaResult<>::Success();
}

void UPragmaConnection::OnInitiallyConnected()
{
	if (SocialWebSocket->IsConnected() && GameWebSocket->IsConnected())
	{
		Status = EStatus::Connected;
		OnConnected.Broadcast();
		OnConnectedComplete.Broadcast(TPragmaResult<>::Success());
		OnConnectedComplete.Clear();
	}
}

void UPragmaConnection::OnReconnected()
{
	RefreshV1Types();
	LastConnectionError = FPragmaConnectionError(SocialWebSocket->GetLastError(), GameWebSocket->GetLastError());

	if (TimerManager->IsTimerActive(FullDisconnectTimerHandle))
	{
		PRAGMA_LOG(Log, "%s - Cancelling pending full disconnect.", ANSI_TO_TCHAR(__FUNCTION__));
		TimerManager->ClearTimer(FullDisconnectTimerHandle);
	}

	if (SocialWebSocket->IsConnected() && GameWebSocket->IsConnected())
	{
		Status = EStatus::Connected;
		OnRecovered.Broadcast();
	}
}

void UPragmaConnection::HandleStateChanged(FPragmaWebSocket& Socket)
{
	if (Socket.GetBackendType() == EPragmaBackendType::Social)
	{
		OnSocialStatusChanged.Broadcast(Socket.GetConnectionStatus());
	}
	else
	{
		OnGameStatusChanged.Broadcast(Socket.GetConnectionStatus());
	}
}

void UPragmaConnection::HandleConnected(FPragmaWebSocket& Socket)
{
	HandleStateChanged(Socket);
	switch (Status)
	{
	case EStatus::Connecting:
		OnInitiallyConnected();
		break;
	case EStatus::Degraded:
		OnReconnected();
		break;
	default:
		break;
	}
}

void UPragmaConnection::HandleDegraded(FPragmaWebSocket& Socket)
{
	HandleStateChanged(Socket);

	LastConnectionError = FPragmaConnectionError(SocialWebSocket->GetLastError(), GameWebSocket->GetLastError());
	if (Status != EStatus::Degraded)
	{
		const FString BackendType = StaticEnum<EPragmaBackendType>()->GetValueAsString(Socket.GetBackendType());
		PRAGMA_LOG(VeryVerbose, "%s [%s] - Status %d, LastConnectionError - %s.", ANSI_TO_TCHAR(__FUNCTION__), *BackendType, Status, *(LastConnectionError->ToPragmaError().ToString()));
		Status = EStatus::Degraded;
		OnDegraded.Broadcast(LastConnectionError);
	}

	Status = EStatus::Degraded;

	const bool IsFullDisconnectEnabled = Config().BackoffMaxConnectionReconnectSeconds >= 0.0f;
	const bool BothSocketsDegraded = AreBothSocketsDegraded();
	const bool IsExistingTimerActive = TimerManager->IsTimerActive(FullDisconnectTimerHandle);

	if (IsFullDisconnectEnabled && BothSocketsDegraded && !IsExistingTimerActive)
	{
		PRAGMA_LOG(Log, "%s - Both sockets degraded, fully disconnecting in {%f} seconds.", ANSI_TO_TCHAR(__FUNCTION__),
			Config().BackoffMaxConnectionReconnectSeconds);
		TimerManager->SetTimer(FullDisconnectTimerHandle,
			FPragmaTimerDelegate::CreateUObject(this, &UPragmaConnection::Disconnect),
			Config().BackoffMaxConnectionReconnectSeconds,
			false);
	}
}

void UPragmaConnection::HandleDisconnected(FPragmaWebSocket& Socket)
{
	if (Socket.GetConnectionStatus() != EPragmaConnectionStatus::NotConnected)
	{
		return;
	}
	HandleStateChanged(Socket);

	const FString BackendType = StaticEnum<EPragmaBackendType>()->GetValueAsString(Socket.GetBackendType());
	const FString ProtocolError = StaticEnum<EPragmaProtocolError>()->GetValueAsString(Socket.GetLastError());
	PRAGMA_LOG(Log, "%s - WebSocket[%s] disconnected with protocol error: %s", ANSI_TO_TCHAR(__FUNCTION__), *BackendType, *ProtocolError);

	if (SocialWebSocket->GetConnectionStatus() == EPragmaConnectionStatus::NotConnected
		&& GameWebSocket->GetConnectionStatus() == EPragmaConnectionStatus::NotConnected)
	{
		const auto SocialError = SocialWebSocket->GetLastError();
		const auto GameError = GameWebSocket->GetLastError();
		if (SocialError == EPragmaProtocolError::None && GameError == EPragmaProtocolError::None)
		{
			FinalizeDisconnect({});
		}
		else
		{
			LastConnectionError = FPragmaConnectionError(SocialError, GameError);
			FinalizeDisconnect(LastConnectionError);
		}
	}
}

void UPragmaConnection::FinalizeDisconnect(TOptional<FPragmaConnectionError> Error)
{
	PRAGMA_LOG(Log, "%s - Fully disconnected. Connection Status %d", ANSI_TO_TCHAR(__FUNCTION__), Status);
	if (Status <= EStatus::NotConnected)
	{
		return;
	}
	Status = EStatus::NotConnected;
	PragmaSocialToken.Empty();
	PragmaGameToken.Empty();
	SocialBackoff->Cancel();
	GameBackoff->Cancel();
	OnPreDisconnect.Broadcast();
	OnDisconnectedV2.Broadcast(Error);
	OnConnectedComplete.Broadcast(Error.IsSet() ? TPragmaResult<>::Failure(Error.GetValue().ToPragmaError()) : TPragmaResult<>::Success());
	OnConnectedComplete.Clear();
}

void UPragmaConnection::ConfigureBackoff(FPragmaBackoff& Backoff) const
{
	Backoff.FuncName = "Reconnect";
	Backoff.InitialRetrySeconds = Config().GetBackoffInitialRetrySeconds();
	Backoff.StaggerReconnectRandomSeconds = Config().GetBackoffStaggerReconnectRandomSeconds();
	Backoff.MaxSecondsBetweenRetries = Config().GetBackoffMaxSecondsBetweenRetries();
}

bool UPragmaConnection::AreBothSocketsDegraded() const
{
	return SocialWebSocket->GetConnectionStatus() == EPragmaConnectionStatus::Degraded &&
		GameWebSocket->GetConnectionStatus() == EPragmaConnectionStatus::Degraded;
}

void UPragmaConnection::RefreshV1Types(TFunction<void(TPragmaResult<>)> OnComplete)
{
	InitializerHttp->HttpGet<FPragmaTypesV1Response>(TEXT("/v1/types"),
	[this, OnComplete](const TPragmaResult<TSharedPtr<FJsonObject>> Result)
	{
		if (Result.IsSuccessful())
		{
			PRAGMA_LOG(Log, "RefreshV1Types -- Success.");
			BackendTypeConverter->LoadBackendIds(Result.Payload());
			if (OnComplete != nullptr)
			{
				OnComplete(TPragmaResult<>::Success());
			}
		}
		else if (OnComplete != nullptr)
		{
			OnComplete(TPragmaResult<>::Failure("RefreshV1Types -- Failed"));
		}
	});
}
