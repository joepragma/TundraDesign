#pragma once

/* This file was auto-generated, and should not be manually edited. See pragma.playerdata.codegen.generators.SubServiceUnrealGenerator */

#include "Dto/MySubServiceDelegates.h"
#include "Dto/PlayerDataCommon.h"
#include "Dto/PragmaPlayerDataTypesDto.h"

class UPragmaPlayerDataServiceRaw;

namespace PragmaPlayerData {

class PRAGMASDK_API MySubService {
public:
    void SetDependencies(FPlayerDataRequestDelegate Delegate);            
    
    void MyOperation(int32 Int, FOnMyMyOperationDelegate Delegate) const;
    
private:
    FPlayerDataRequestDelegate RequestDelegate;
};

}