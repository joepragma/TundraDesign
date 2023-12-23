#pragma once

#include "CoreTypes.h"

//
// Used to pass along additional information with a response or notification payload that may be useful
// to the game client.
//
struct FPragmaMessageMetadata final
{
	// A sequentially increasing number with each message (response or notification) from the backend.
	// A higher number will always indicate a more recent message.
	int32 SequenceNumber{0};

	static constexpr FPragmaMessageMetadata None() { return FPragmaMessageMetadata{}; }
};