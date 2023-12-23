#pragma once

/* This file was auto-generated, and should not be manually edited. See pragma.playerdata.codegen.generators.SubServiceUnrealGenerator */

#include "Dto/ContentSubServiceDelegates.h"
#include "Dto/PlayerDataCommon.h"
#include "Dto/PragmaPlayerDataTypesDto.h"

class UPragmaPlayerDataPartnerServiceRaw;

namespace PragmaPartnerPlayerData {

class PRAGMASDK_API ContentSubService {
public:
    void SetDependencies(FPlayerDataRequestDelegate Delegate);            
    
    void DemoContentOperation(const FString& PlayerId, FOnContentDemoContentOperationDelegate Delegate) const;
    
private:
    FPlayerDataRequestDelegate RequestDelegate;
};

}