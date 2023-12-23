#pragma once
#include "CoreMinimal.h"
#include "PragmaBackoff.h"
#include "PragmaConnectionStatus.h"
#include "PragmaProtocol.h"
#include "PragmaProtocolError.h"

// Forward declarations.
class IWebSocket;

typedef TMap<FString, FString> FUpgradeHeaders;

//
// Manages a websocket connection with the Pragma backend.
// Duties include:
// - Sending and receiving Pragma messages.
// - Monitoring traffic.
// - Managing connection including soft disconnects, retries, etc.
//
class PRAGMASDK_API FPragmaWebSocket final : public FPragmaProtocol, public TSharedFromThis<FPragmaWebSocket>
{
public:
    DECLARE_DELEGATE_RetVal_ThreeParams(TSharedRef<IWebSocket>, FCreateWebSocket, const FString&, const FString&, const FUpgradeHeaders&);
    DECLARE_EVENT_OneParam(FPragmaWebSocket, FConnectionEvent, FPragmaWebSocket& /* Socket */);

    explicit FPragmaWebSocket(const EPragmaBackendType InBackendType, IPragmaBackoff* InBackoff, bool InLogPragmaDetailedMessages = false);
    // Inject a custom websocket creation function. Defaults to FWebSocketsModule::CreateWebSocket().
    explicit FPragmaWebSocket(FCreateWebSocket CreateWebSocketDelegate, IPragmaBackoff* InBackoff, bool InLogPragmaDetailedMessages = false);
    virtual  ~FPragmaWebSocket() override {}

	virtual TWeakPtr<FPragmaProtocol> WeakThis() { return AsShared(); }

    FConnectionEvent& OnConnected() { return ConnectedEvent; }
    FConnectionEvent& OnDegraded() { return DegradedEvent; }
    FConnectionEvent& OnDisconnected() { return DisconnectedEvent; }

    // Must be initialized before use.
    void Init(FString InApiUrl);

    bool IsConnected() const;

    // Attempt to establish the websocket connection. Bind to OnConnected and OnDisconnected to monitor status.
    void Connect();

    // Disconnect the websocket manually. This will fire the Disconnected event.
    void Close();

    // Changes the auth token
    void SetAuthToken(FString NewAuthToken);

    // Gets the auth token
    FString GetAuthToken() const;

    EPragmaProtocolError GetLastError() const;
    EPragmaConnectionStatus GetConnectionStatus() const;

	TOptional<float> SecondsUntilReconnect() const;

protected:
    // FPragmaProtocol impl.
    virtual void DebugRequestStarted(const FString& RequestTypeName, const FString& ResponseTypeName) const override;
    virtual bool CanSendMessage() const override;
    virtual bool SendJsonMessage(const FString& Message, const FSendFailure& /* SendFailure */) override;

private:
    // IWebSocket event handlers.
    void HandleWebSocketConnected();
    void HandleWebSocketDegraded();
    void HandleWebSocketConnectionError(const FString& Error);
    void HandleWebSocketClosed(const int32 Status, const FString& Reason, const bool bWasClean);
    void HandleWebSocketTextMessage(const FString& MessageString);
    void HandleWebSocketBinaryMessage(const void* Data, SIZE_T Size, SIZE_T BytesRemaining);

    void Reconnect();
    TMap<FString, FString> BuildUpgradeHeaders(bool bAddPragmaReconnectHeader) const;
    void ConnectInternal(const TMap<FString, FString>& UpgradeHeaders);

    void SetLastError(int32 StatusCode);
    void SetConnectionStatus(EPragmaConnectionStatus Status);

    TSharedPtr<IWebSocket> WebSocket;
    FString ApiUrl;
    IPragmaBackoff* Backoff;
    FString AuthToken;

    // Fired whenever the Socket uncleanly loses connection to the backend.
    FConnectionEvent DegradedEvent;
    // Fired whenever the Socket cleanly loses connection to the backend.
    FConnectionEvent DisconnectedEvent;
    // Fired whenever the Socket becomes fully connected and ready to send and receive messages.
    FConnectionEvent ConnectedEvent;
    // Cached delegate for creating a websocket.
    FCreateWebSocket CreateWebSocket;

    EPragmaProtocolError LastError{EPragmaProtocolError::None};
    EPragmaConnectionStatus ConnectionStatus{EPragmaConnectionStatus::NotConnected};
};
