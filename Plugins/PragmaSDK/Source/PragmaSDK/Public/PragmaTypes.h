#pragma once

#include "PragmaTypes.generated.h"

UENUM(BlueprintType, Category=Pragma)
enum class EPragmaProtocolType : uint8
{
	Http,
    WebSocket
};

UENUM(BlueprintType, Category=Pragma)
enum class EPragmaBackendType : uint8
{
	Social,
    Game
};
