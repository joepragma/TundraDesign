#pragma once
#include "PragmaConnectionStatus.h"
#include "PragmaProtocolError.h"
#include "PragmaConnectionInfo.generated.h"

UENUM()
enum class EPragmaConnectionError : uint8
{
	// Catch-all unknown error case. Shouldn't happen.
	Fail_Unknown,

	// Failed because we're already connecting or connected.
	Fail_AlreadyConnected,

	// Failed to retrieve backend config.
	Fail_RetrieveConfig,

	// Failed to get in or refresh login queue ticket.
	Fail_LoginQueue,

	// Failed to authenticate with given credentials.
	Fail_Authentication,

	// Failed to establish a bidirectional connection (e.g. websocket) with the backend.
	Fail_Connect,

	// Failed due to connection closing abnormally.
	Fail_Abnormal,

	// Disconnect due to user request, i.e. explicit log out.
	Reason_UserRequested,

	// ------------------------------------------------------------------
	// --- Below are mapped to the platform SessionTerminationReason. ---

	// Server is shutting down e.g. for downtime.
	Reason_ServerShutdown,

	// Kicked from the server.
	Reason_Kicked,

	// The user has logged in elsewhere and the platform does not support multiple sessions.
	Reason_Usurped,

	// The connection has timed out e.g. due to inactivity.
	Reason_Expired,
};

namespace Pragma
{

struct FBackendConnectionInfo
{
	// The current state of the connection with the backend.
	EPragmaConnectionStatus Status{EPragmaConnectionStatus::NotConnected};

	// If set, the last error encountered during connect/reconnect.
	TOptional<EPragmaProtocolError> ConnectionError;

	// If set, the amount of time before the next automatic reconnect will be attempted.
	TOptional<float> SecondsUntilReconnect;
};

struct FConnectionInfo
{
	// The current state of the connection with the backend.
    // Use `Social` and `Game` to inspect deeper into each connection.
	EPragmaConnectionStatus Status{EPragmaConnectionStatus::NotConnected};

	// Information on the Social backend connection.
	FBackendConnectionInfo Social;

	// Information on the Game backend connection.
	FBackendConnectionInfo Game;

	// Error from most recent disconnect. This will only exist when we are in the NotConnected state after a disconnect.
	TOptional<FString> DisconnectError;

	// If set, the amount of time before the session will be completely disconnected and require manual reconnection.
	TOptional<float> SecondsUntilTimeout;
};

}
