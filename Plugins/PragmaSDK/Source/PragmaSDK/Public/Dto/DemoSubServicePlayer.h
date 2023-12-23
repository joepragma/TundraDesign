#pragma once

/* This file was auto-generated, and should not be manually edited. See pragma.playerdata.codegen.generators.SubServiceUnrealGenerator */

#include "Dto/DemoSubServiceDelegates.h"
#include "Dto/PlayerDataCommon.h"
#include "Dto/PragmaPlayerDataTypesDto.h"

class UPragmaPlayerDataServiceRaw;

namespace PragmaPlayerData {

class PRAGMASDK_API DemoSubService {
public:
    void SetDependencies(FPlayerDataRequestDelegate Delegate);            
    
    void DeleteNumberData(FOnDemoDeleteNumberDataDelegate Delegate) const;
    void Echo(const FString& Message, FOnDemoEchoDelegate Delegate) const;
    void EchoComplex(FPragma_PlayerData_DemoComplexTypeProto DemoComplexType, FOnDemoEchoComplexDelegate Delegate) const;
    void EchoList(TArray<FString> ListOfStrings, FOnDemoEchoListDelegate Delegate) const;
    void HideStuffFromPlayer(FOnDemoHideStuffFromPlayerDelegate Delegate) const;
    void SendEmptyPayload(FOnDemoSendEmptyPayloadDelegate Delegate) const;
    
private:
    FPlayerDataRequestDelegate RequestDelegate;
};

}