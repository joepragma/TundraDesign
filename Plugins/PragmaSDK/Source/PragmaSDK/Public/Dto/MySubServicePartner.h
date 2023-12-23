#pragma once

/* This file was auto-generated, and should not be manually edited. See pragma.playerdata.codegen.generators.SubServiceUnrealGenerator */

#include "Dto/MySubServiceDelegates.h"
#include "Dto/PlayerDataCommon.h"
#include "Dto/PragmaPlayerDataTypesDto.h"

class UPragmaPlayerDataPartnerServiceRaw;

namespace PragmaPartnerPlayerData {

class PRAGMASDK_API MySubService {
public:
    void SetDependencies(FPlayerDataRequestDelegate Delegate);            
    
    
private:
    FPlayerDataRequestDelegate RequestDelegate;
};

}