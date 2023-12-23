#pragma once

/* This file was auto-generated, and should not be manually edited. See pragma.playerdata.codegen.generators.SubServiceUnrealGenerator */

#include "Dto/LoadSubServiceDelegates.h"
#include "Dto/PlayerDataCommon.h"
#include "Dto/PragmaPlayerDataTypesDto.h"

class UPragmaPlayerDataPartnerServiceRaw;

namespace PragmaPartnerPlayerData {

class PRAGMASDK_API LoadSubService {
public:
    void SetDependencies(FPlayerDataRequestDelegate Delegate);            
    
    void BigDataEntity(const FString& PlayerId, FOnLoadBigDataEntityDelegate Delegate) const;
    void EmptyOperation(const FString& PlayerId, FOnLoadEmptyOperationDelegate Delegate) const;
    void GrantStarterItems(const FString& PlayerId, FOnLoadGrantStarterItemsDelegate Delegate) const;
    void ManyComponentEntity(const FString& PlayerId, FOnLoadManyComponentEntityDelegate Delegate) const;
    
private:
    FPlayerDataRequestDelegate RequestDelegate;
};

}