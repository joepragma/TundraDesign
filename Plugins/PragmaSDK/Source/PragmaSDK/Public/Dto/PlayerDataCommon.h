#pragma once

/* This file was auto-generated, and should not be manually edited. See pragma.playerdata.codegen.generators.SubServiceUnrealGenerator */

#include "Misc/TVariant.h"
#include "Dto/PragmaPlayerDataTypesDto.h"

enum class PlayerDataError { Unknown, FailedToUpdateCache };

using PlayerDataResult = TVariant<PlayerDataError, FPragma_PlayerData_PlayerDataResponseProto_Payload>;
DECLARE_DELEGATE_OneParam(FOnPlayerDataResult, PlayerDataResult Result);

namespace PragmaPlayerData {
    DECLARE_DELEGATE_TwoParams(FPlayerDataRequestDelegate, FPragma_PlayerData_PlayerDataRequestProto_Payload Payload, FOnPlayerDataResult Result);
}

namespace PragmaPartnerPlayerData {
    DECLARE_DELEGATE_ThreeParams(FPlayerDataRequestDelegate, const FString& PlayerId, FPragma_PlayerData_PlayerDataRequestProto_Payload Payload, FOnPlayerDataResult Result);
}