#pragma once

/* This file was auto-generated, and should not be manually edited. See pragma.playerdata.codegen.generators.SubServiceUnrealGenerator */

#include "Dto/DemoSubServiceDelegates.h"
#include "Dto/PlayerDataCommon.h"
#include "Dto/PragmaPlayerDataTypesDto.h"

class UPragmaPlayerDataPartnerServiceRaw;

namespace PragmaPartnerPlayerData {

class PRAGMASDK_API DemoSubService {
public:
    void SetDependencies(FPlayerDataRequestDelegate Delegate);            
    
    void AddNumber(const FString& PlayerId, int32 Number, FOnDemoAddNumberDelegate Delegate) const;
    void AddNumberTimesTwo(const FString& PlayerId, int32 Number, FOnDemoAddNumberTimesTwoDelegate Delegate) const;
    void DeleteNumberData(const FString& PlayerId, FOnDemoDeleteNumberDataDelegate Delegate) const;
    void Echo(const FString& PlayerId, const FString& Message, FOnDemoEchoDelegate Delegate) const;
    void EchoList(const FString& PlayerId, TArray<FString> ListOfStrings, FOnDemoEchoListDelegate Delegate) const;
    void HideStuffFromPlayer(const FString& PlayerId, FOnDemoHideStuffFromPlayerDelegate Delegate) const;
    
private:
    FPlayerDataRequestDelegate RequestDelegate;
};

}