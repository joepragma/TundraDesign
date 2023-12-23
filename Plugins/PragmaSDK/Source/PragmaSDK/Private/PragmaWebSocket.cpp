#include "PragmaWebSocket.h"
#include "WebSocketsModule.h"
#include "IWebSocket.h"
#include "Debug/PragmaDebugServer.h"
#include "PragmaProtocolError.h"

#include "PragmaLog.h"

FPragmaWebSocket::FPragmaWebSocket(const EPragmaBackendType InBackendType, IPragmaBackoff* InBackoff, const bool InLogPragmaDetailedMessages)
	: FPragmaProtocol(InBackendType, InLogPragmaDetailedMessages), Backoff(InBackoff)

{
	// In packaged builds WebSockets plugin doesn't load automatically, so ensure websockets module is loaded
	// before we try to use it.
	FModuleManager::LoadModuleChecked<FWebSocketsModule>("WebSockets");
}

FPragmaWebSocket::FPragmaWebSocket(FCreateWebSocket CreateWebSocketDelegate, IPragmaBackoff* InBackoff, const bool InLogPragmaDetailedMessages)
	: FPragmaProtocol(EPragmaBackendType::Game, InLogPragmaDetailedMessages), Backoff(InBackoff)
{
	CreateWebSocket = MoveTemp(CreateWebSocketDelegate);
}

void FPragmaWebSocket::Init(FString InApiUrl)
{
	if (WebSocket.IsValid())
	{
		PRAGMA_LOG(Error, "FPragmaWebSocket::Init -- Cannot set the WebSocketApiUrl after the websocket has already connected!");
		return;
	}
	ApiUrl = MoveTemp(InApiUrl);
}

bool FPragmaWebSocket::IsConnected() const
{
	return (WebSocket.IsValid() && WebSocket->IsConnected());
}

TMap<FString, FString> FPragmaWebSocket::BuildUpgradeHeaders(const bool bAddPragmaReconnectHeader) const
{
	const FString BearerHeader = FString::Printf(TEXT("Bearer %s"), *AuthToken);
	TMap<FString, FString> headers = {
		{TEXT("Authorization"), BearerHeader},
		{TEXT("Accept"), TEXT("application/json")}
	};
	if (bAddPragmaReconnectHeader)
	{
		headers.Add(TEXT("pragma-reconnect"), TEXT(""));
	}
	return headers;
}

void FPragmaWebSocket::Connect()
{
	ConnectInternal(BuildUpgradeHeaders(false));
}

void FPragmaWebSocket::ConnectInternal(const TMap<FString, FString>& UpgradeHeaders)
{
	if (ApiUrl.IsEmpty())
	{
		PRAGMA_LOG(Error, "FPragmaWebSocket::Connect -- Must call Init to set ApiUrl before connecting.");
		DisconnectedEvent.Broadcast(*this);
		return;
	}
	SetConnectionStatus(EPragmaConnectionStatus::Connecting);
	static const FString SubProtocol = "pragma"; // We don't use subprotocols, but WinHttp requires this field is not empty.

	WebSocket = CreateWebSocket.IsBound()
		? CreateWebSocket.Execute(ApiUrl, SubProtocol, UpgradeHeaders)
		: FWebSocketsModule::Get().CreateWebSocket(ApiUrl, SubProtocol, UpgradeHeaders);
	if (!ensure(WebSocket.IsValid()))
	{
		PRAGMA_LOG(Error, "FPragmaWebSocket::Connect -- Failed to create WebSocket.");
		DisconnectedEvent.Broadcast(*this);
		return;
	}
	WebSocket->OnConnected().AddSP(this, &FPragmaWebSocket::HandleWebSocketConnected);
	WebSocket->OnConnectionError().AddSP(this, &FPragmaWebSocket::HandleWebSocketConnectionError);
	WebSocket->OnClosed().AddSP(this, &FPragmaWebSocket::HandleWebSocketClosed);
	WebSocket->OnMessage().AddSP(this, &FPragmaWebSocket::HandleWebSocketTextMessage);
	WebSocket->OnRawMessage().AddSP(this, &FPragmaWebSocket::HandleWebSocketBinaryMessage);
	WebSocket->Connect();
}

void FPragmaWebSocket::Reconnect()
{
	WebSocket->OnConnected().RemoveAll(this);
	WebSocket->OnConnectionError().RemoveAll(this);
	WebSocket->OnClosed().RemoveAll(this);
	WebSocket->OnMessage().RemoveAll(this);
	WebSocket->OnRawMessage().RemoveAll(this);
	WebSocket->Close();

	ConnectInternal(BuildUpgradeHeaders(true));
}

void FPragmaWebSocket::Close()
{
	if (WebSocket.IsValid() && WebSocket->IsConnected())
	{
		WebSocket->Close();
	}
	else
	{
		SetConnectionStatus(EPragmaConnectionStatus::NotConnected);
	}
}

void FPragmaWebSocket::SetAuthToken(FString NewAuthToken)
{
	if (!NewAuthToken.Equals(AuthToken))
	{
		AuthToken = MoveTemp(NewAuthToken);
	}
}

FString FPragmaWebSocket::GetAuthToken() const
{
	return FString(AuthToken);
}

void FPragmaWebSocket::DebugRequestStarted(const FString& RequestTypeName, const FString& ResponseTypeName) const
{
	if (GetDebugServer() != nullptr)
	{
		GetDebugServer()->RequestStarted(GetBackendType(), EPragmaProtocolType::WebSocket, RequestTypeName, ResponseTypeName);
	}
}

bool FPragmaWebSocket::CanSendMessage() const
{
	return WebSocket.IsValid() && WebSocket->IsConnected();
}

bool FPragmaWebSocket::SendJsonMessage(const FString& Message, const FSendFailure& /* SendFailure */)
{
	if (CanSendMessage())
	{
		WebSocket->Send(Message);
		return true;
	}
	return false;
}

void FPragmaWebSocket::HandleWebSocketConnected()
{
	PRAGMA_LOG(Log, "FPragmaWebSocket::HandleWebSocketConnected[%s] successful!", *ApiUrl);
	SetConnectionStatus(EPragmaConnectionStatus::Connected);
}

void FPragmaWebSocket::HandleWebSocketDegraded()
{
	DegradedEvent.Broadcast(*this);
	if(!Backoff->IsBackoffFuncSet())
	{
		Backoff->SetBackoffFunc(FPragmaTimerDelegate::CreateSP(this, &FPragmaWebSocket::Reconnect));
	}
	Backoff->Continue();
}

void FPragmaWebSocket::HandleWebSocketConnectionError(const FString& Error)
{
	if (ConnectionStatus == EPragmaConnectionStatus::NotConnected)
	{
		// Ignore superfluous errors from websocket library when we're already disconnected.
		return;
	}
	PRAGMA_LOG(Error, "FPragmaWebSocket::HandleWebSocketConnectionError[%s]: %s", *ApiUrl, *Error);
	SetConnectionStatus(EPragmaConnectionStatus::Degraded);
}

void FPragmaWebSocket::HandleWebSocketClosed(const int32 Status, const FString& Reason, const bool bWasClean)
{
	SetLastError(Status);
	if (bWasClean)
	{
		PRAGMA_LOG(VeryVerbose, "FPragmaWebSocket::HandleWebSocketClosed[%s] (peer requested): Status? %d, Reason? %s", *ApiUrl, Status, *Reason);
		SetConnectionStatus(EPragmaConnectionStatus::NotConnected);
	}
	else
	{
		PRAGMA_LOG(Error, "FPragmaWebSocket::HandleWebSocketClosed[%s] (not peer requested): Status? %d, Reason? %s", *ApiUrl, Status, *Reason);
		SetConnectionStatus(EPragmaConnectionStatus::Degraded);
	}
}

void FPragmaWebSocket::HandleWebSocketTextMessage(const FString& MessageString)
{
	if (GetDebugServer() != nullptr && GetDebugServer()->GetMockEnabled())
	{
		return;
	}
	JsonMessageReceived(MessageString);
}

void FPragmaWebSocket::HandleWebSocketBinaryMessage(const void* Data, SIZE_T Size, SIZE_T BytesRemaining)
{
	if (GetDebugServer() != nullptr && GetDebugServer()->GetMockEnabled())
	{
		return;
	}
	// TODO protos will come in here.
}

void FPragmaWebSocket::SetLastError(int32 StatusCode)
{
	switch (StatusCode)
	{
		// Handling errors defined here https://tools.ietf.org/html/rfc6455#section-7.4.1
		case 1001: // platform going away
		case 1006: // connection closed abnormally
			LastError = EPragmaProtocolError::ServiceUnavailable;
			break;
		case 1010: // client termination due to extension negotiation failure
		case 1015: // TLS handshake failure
			LastError = EPragmaProtocolError::BadConfiguration;
			break;
		case 1002: // terminating due to protocol error
		case 1003: // received incorrect data type
		case 1007: // received inconsistent data with type of message
		case 1008: // general policy violation
		case 1009: // received data that is too big to process
			LastError = EPragmaProtocolError::BadRequest;
			break;
		case 1011: // server terminating due to unexpected condition
			LastError = EPragmaProtocolError::Unknown;
			break;
		case 1000: // normal closure
		case 1004: // reserved. unused.
		case 1005: // reserved. no status code
		default:
			LastError = EPragmaProtocolError::None;
			break;
	}
}

EPragmaProtocolError FPragmaWebSocket::GetLastError() const
{
	return LastError;
}

void FPragmaWebSocket::SetConnectionStatus(EPragmaConnectionStatus Status)
{
	ConnectionStatus = Status;
	switch (Status)
	{
	case EPragmaConnectionStatus::Connected:
		LastError = EPragmaProtocolError::None;
		Backoff->Cancel();
		ConnectedEvent.Broadcast(*this);
		break;
	case EPragmaConnectionStatus::Degraded:
		HandleWebSocketDegraded();
		break;
	case EPragmaConnectionStatus::NotConnected:
		DisconnectedEvent.Broadcast(*this);
		break;
	default:
		break;
	}
}

EPragmaConnectionStatus FPragmaWebSocket::GetConnectionStatus() const
{
	return ConnectionStatus;
}

TOptional<float> FPragmaWebSocket::SecondsUntilReconnect() const
{
	return Backoff->SecondsUntilNextRetry();
}
