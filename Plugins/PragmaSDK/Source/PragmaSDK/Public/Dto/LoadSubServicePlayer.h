#pragma once

/* This file was auto-generated, and should not be manually edited. See pragma.playerdata.codegen.generators.SubServiceUnrealGenerator */

#include "Dto/LoadSubServiceDelegates.h"
#include "Dto/PlayerDataCommon.h"
#include "Dto/PragmaPlayerDataTypesDto.h"

class UPragmaPlayerDataServiceRaw;

namespace PragmaPlayerData {

class PRAGMASDK_API LoadSubService {
public:
    void SetDependencies(FPlayerDataRequestDelegate Delegate);            
    
    void BigDataEntity(FOnLoadBigDataEntityDelegate Delegate) const;
    void ContentDrivenGrants(FOnLoadContentDrivenGrantsDelegate Delegate) const;
    void EmptyOperation(FOnLoadEmptyOperationDelegate Delegate) const;
    void GrantStarterItems(FOnLoadGrantStarterItemsDelegate Delegate) const;
    void LoadTestEntityTransform(FOnLoadLoadTestEntityTransformDelegate Delegate) const;
    void ManyComponentEntity(FOnLoadManyComponentEntityDelegate Delegate) const;
    
private:
    FPlayerDataRequestDelegate RequestDelegate;
};

}