#pragma once

#include "PragmaBackoff.h"
#include "PragmaConnectionError.h"
#include "PragmaConnectionStatus.h"
#include "PragmaProtocol.h"
#include "PragmaSdkPackage.h"
#include "PragmaTypes.h"
#include "PragmaConnection.generated.h"

class FPragmaHttp;
class FPragmaWebSocket;
class UPragmaSdkConfig;
class FPragmaDebugServer;
class FPragmaBackendTypeConverter;
class IPragmaTimerManager;

// ************************************************************
// *** THIS FILE WAS STUBBED FOR THIS TUNDRA DESIGN PROJECT ***
// ************************************************************

UCLASS(Transient)
class PRAGMASDK_API UPragmaConnection : public UObject
{
public:
	DECLARE_EVENT(UPragmaConnection, FInitializedEvent);
	DECLARE_EVENT(UPragmaConnection, FConnectedEvent);
	DECLARE_EVENT_OneParam(UPragmaConnection, FConnectionEventV2, TOptional<FPragmaConnectionError>);

	FInitializedEvent OnInitialized;
	FConnectedEvent OnConnected;

	// Fired when completely loses connection either intentionally or all retries have failed.
	// This typically should result in kicking the player back to the login screen, or entering some sort
	// of 'offline only' mode if the game supports it.
	FConnectionEventV2 OnDisconnectedV2;

	// Fired when one or both websockets are operating in a degraded state which may result in long response times,
	// failed requests, or complete disconnection if we remain degraded.
	FConnectionEventV2 OnDegraded;

	// Fired when connection is recovered from a degraded state.
	FConnectedEvent OnRecovered;

	DECLARE_EVENT_OneParam(IPragmaConnection, FStatusChangedEvent, EPragmaConnectionStatus);
	FStatusChangedEvent OnSocialStatusChanged;
	FStatusChangedEvent OnGameStatusChanged;

	DECLARE_EVENT(UPragmaConnection, FPreDisconnectEvent);
	// Fired on disconnect, before the actual Disconnected event. This allows binding cleanup methods before actual disconnect handlers so the handlers
	// can guarantee state is cleared.
	FPreDisconnectEvent OnPreDisconnect;

	DECLARE_EVENT_TwoParams(UPragmaConnection, FAuthTokenEvent, FString, FString);
	// Fired when SetAuthToken has been called
	FAuthTokenEvent OnAuthTokenChanged;

	GENERATED_BODY()

	UPragmaConnection() {}

	// Check if the Connection is ready to communicate over http.
	virtual bool IsInitialized() const { return Status >= EStatus::Initialized; }

	bool IsInitializing() const { return Status > EStatus::ReadyForBackendConfig && Status < EStatus::Initialized; }

	// Check if the Connection is ready to communicate over websocket.
	virtual bool IsConnected() const { return Status > EStatus::NotConnected; }

	bool IsConnecting() const { return Status > EStatus::NotConnected && Status < EStatus::Connected; }

	// Only use externally if not specifying the backend address in configuration.
	void SetBackendAddress(FString InBackendAddress);

	// Get the protocol that handles messages with the Social backend.
	virtual FPragmaProtocol& SocialBackend() const;
	// Get the protocol that handles messages with the Game backend.
	virtual FPragmaProtocol& GameBackend() const;

	// Sets the auth token which in turn gives the tokens to the underlying WebSockets
	virtual void SetAuthTokens(FString InPragmaSocialToken, FString InPragmaGameToken);
	FString GetPragmaSocialToken() const { return PragmaSocialToken; }
	FString GetPragmaGameToken() const { return PragmaGameToken; }

	// Access to specific protocols, for sending messages that require specific protocols.
	virtual FPragmaHttp& Http(EPragmaBackendType BackendType) const;
	FPragmaWebSocket& WebSocket(EPragmaBackendType BackendType) const;

	// Loads, retrieves, and/or updates configuration needed from the backend to begin normal operation.
	virtual void UpdateBackendConfig(TFunction<void(TPragmaResult<>)> OnComplete);

	FPragmaProtocol& Protocol(EPragmaProtocolType Protocol, EPragmaBackendType BackendType) const;

	const UPragmaSdkConfig& Config() const { check(SdkConfig); return *SdkConfig; }

	virtual FString GetGameShardId() const;

	// Try to connect websockets if they're not already connected.
	// Session token can be obtained via logging in with the Account service.
	virtual void Connect(FString InPragmaSocialToken, FString InPragmaGameToken, TFunction<void(TPragmaResult<>)> OnComplete);

	// Disconnect any connected sockets, e.g. in the case of manual log-out, or out of date client and broadcast OnDisconnected with no error.
	virtual void Disconnect();

	TOptional<float> RemainingSecondsUntilFullDisconnect() const;
	virtual TOptional<FPragmaConnectionError> GetLastConnectionError() const { return LastConnectionError; }

	virtual bool IsPartnerSession() const { return bIsPartnerSession; }
	virtual void CleanupBeforeDestruction() const;

PACKAGE_SCOPE:
	// Must be called before usage.
	//
	// bEnableHeartbeat true will send a periodic heartbeat message to the backend to keep the connection alive.
	void Init(UPragmaSdkConfig* InSdkConfig, const TSharedPtr<IPragmaTimerManager>& InTimerManager);

	// This will always return nullptr after initialization.
	FPragmaHttp* GetInitializerHttp() const
	{
		return InitializerHttp.Get();
	}

	EPragmaConnectionStatus GetStatus() const;

	// Core transition - don't mess with this.
	bool bIsPartnerSession{false};

protected:
	enum class EStatus : uint8
	{
		////////////////////////////////////////////////
		// The first few states deal with initialization.
		// The steps below won't necessary happy all at once, but they should happen in order.

		// Not initialized. Init must be called.
		NotInitialized = 0,
		// Init has been called. Need to set backend address so we know where to get further backend config.
		WaitingForBackendAddress,
		// Ready to request backend config. This is a separate state since there's no
		ReadyForBackendConfig,
		// Requested necessary backend configuration (e.g. info, types).
		RequestedBackendConfig,
		// Received backend configuration, anything >= Initialized is ready to communicate with the backend
		// (at least over http).
		Initialized,

		////////////////////////////////////////////////
		// Below here, we're fully initialized, and may bounce between states.

		// Initialized, but not connected. Ok to send http requests.
		// Must Connect() before sending and receiving messages over websocket.
		NotConnected = Initialized,
		// In process of connecting websockets.
		Connecting,
		// Connection issues, will attempt to automatically resolve.
		Degraded,
		// Fully connected. Ready to send and receive messages over http or websocket.
		Connected
	};

	UPROPERTY()
	UPragmaSdkConfig* SdkConfig{nullptr};

	void SetStatusForTest(EStatus InStatus) { Status = InStatus;}

private:
	// Disconnect and broadcast OnDisconnected with a connection error.
	virtual void FinalizeDisconnect(TOptional<FPragmaConnectionError> Error);

	void StubbedUpdateBackendConfig();
	void StubbedConnect();

	// Once all initialization requests have finished successfully, we can finish init.
	void TryFinishInit();
	void OnInitiallyConnected();
	void OnReconnected();

	void HandleStateChanged(FPragmaWebSocket& Socket);
	void HandleConnected(FPragmaWebSocket& Socket);
	void HandleDegraded(FPragmaWebSocket& Socket);
	void HandleDisconnected(FPragmaWebSocket& Socket);

	void ConfigureBackoff(FPragmaBackoff& Backoff) const;
	bool AreBothSocketsDegraded() const;

	void RefreshV1Types(TFunction<void(TPragmaResult<>)> OnComplete = nullptr);

	// Current status of the connection to the backend, incorporating information about both Social and Game websockets.
	EStatus Status{EStatus::NotInitialized};

	TSharedPtr<IPragmaTimerManager> TimerManager;

	// This exists only until we're initialized, to ensure we use /v1/info endpoints for the majority of requests.
	TSharedPtr<FPragmaHttp> InitializerHttp;

	TSharedPtr<FPragmaHttp> SocialHttp;
	TSharedPtr<FPragmaHttp> GameHttp;
	TSharedPtr<FPragmaWebSocket> SocialWebSocket;
	TSharedPtr<FPragmaWebSocket> GameWebSocket;
	FPragmaTimerHandle FullDisconnectTimerHandle;
	TOptional<FPragmaBackoff> SocialBackoff;
	TOptional<FPragmaBackoff> GameBackoff;

	// Authorization token used to establish Social websocket connection. Cached for the sake of retries.
	FString PragmaSocialToken;
	// Authorization token used to establish Game websocket connection. Cached for the sake of retries.
	FString PragmaGameToken;

	// Initialization tracking.
	FString BackendAddress;

	FString GameShardId;

	TSharedPtr<FPragmaBackendTypeConverter> BackendTypeConverter;

	TOptional<FPragmaConnectionError> LastConnectionError;

	// For these we keep a delegate internally, so that the interface uses a simple TFunction, but will bind to a
	// common delegate in case there's a something in progress. This way both calls can get the same result.
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnComplete, TPragmaResult<> /* Result */);
	FOnComplete OnBackendConfigComplete;
	FOnComplete OnConnectedComplete;

	enum class EInitConfigState : uint8
	{
		None,
		Error,
		Received
	};
	EInitConfigState InfoInitConfigState{EInitConfigState::None};
	EInitConfigState TypesInitConfigState{EInitConfigState::None};
	TPragmaResult<> InitResult;

	// Debug server we pipe info to for nice web browser debug display. When disabled or in shipping builds, this will
	// be nullptr.
	TSharedPtr<FPragmaDebugServer> DebugServer;
};
