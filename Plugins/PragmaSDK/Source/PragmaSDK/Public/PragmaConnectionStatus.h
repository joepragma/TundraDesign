#pragma once

#include "UObject/ObjectMacros.h"

UENUM(BlueprintType, Category=Pragma)
enum class EPragmaConnectionStatus: uint8
{
	// Not connected at all.
	NotConnected,
	// Waiting for initial connection from a NotConnected state.
	Connecting,
	// WebSocket has disconnected and we will try to seamlessly reconnect, but may be indicative of a greater issue.
	Degraded,
	// Fully connected to the backend and functioning normally.
	Connected,
};
 
