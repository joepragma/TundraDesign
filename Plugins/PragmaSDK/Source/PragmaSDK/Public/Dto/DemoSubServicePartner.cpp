/* This file was auto-generated, and should not be manually edited. See pragma.playerdata.codegen.generators.SubServiceUnrealGenerator */

#include "DemoSubServicePartner.h"
#include "PragmaMessageMetadata.h"
#include "Dto/PragmaPlayerDataServiceRpcDto.h"

void PragmaPartnerPlayerData::DemoSubService::SetDependencies(FPlayerDataRequestDelegate Delegate) {
    RequestDelegate = MoveTemp(Delegate);
}

void PragmaPartnerPlayerData::DemoSubService::AddNumber(const FString& PlayerId, int32 Number, FOnDemoAddNumberDelegate Delegate) const {
    FPragma_PlayerData_DoOperationPartnerV1Request Request;
    Request.Ext.Payload.SetAddNumber({Number});
    
    RequestDelegate.ExecuteIfBound(PlayerId, Request.Ext.Payload, FOnPlayerDataResult::CreateLambda(
        [this, Delegate = MoveTemp(Delegate)](PlayerDataResult Result) {
            if (Result.IsType<PlayerDataError>()) {
                Delegate.Execute(NullOpt);
                return;
            }
            Delegate.Execute(MoveTempIfPossible(Result.Get<FPragma_PlayerData_PlayerDataResponseProto_Payload>().AddNumber()));
        }
    ));
}

void PragmaPartnerPlayerData::DemoSubService::AddNumberTimesTwo(const FString& PlayerId, int32 Number, FOnDemoAddNumberTimesTwoDelegate Delegate) const {
    FPragma_PlayerData_DoOperationPartnerV1Request Request;
    Request.Ext.Payload.SetAddNumberTimesTwo({Number});
    
    RequestDelegate.ExecuteIfBound(PlayerId, Request.Ext.Payload, FOnPlayerDataResult::CreateLambda(
        [this, Delegate = MoveTemp(Delegate)](PlayerDataResult Result) {
            if (Result.IsType<PlayerDataError>()) {
                Delegate.Execute(NullOpt);
                return;
            }
            Delegate.Execute(MoveTempIfPossible(Result.Get<FPragma_PlayerData_PlayerDataResponseProto_Payload>().AddNumberTimesTwo()));
        }
    ));
}

void PragmaPartnerPlayerData::DemoSubService::DeleteNumberData(const FString& PlayerId, FOnDemoDeleteNumberDataDelegate Delegate) const {
    FPragma_PlayerData_DoOperationPartnerV1Request Request;
    Request.Ext.Payload.SetDeleteNumberData({});
    
    RequestDelegate.ExecuteIfBound(PlayerId, Request.Ext.Payload, FOnPlayerDataResult::CreateLambda(
        [this, Delegate = MoveTemp(Delegate)](PlayerDataResult Result) {
            if (Result.IsType<PlayerDataError>()) {
                Delegate.Execute(NullOpt);
                return;
            }
            Delegate.Execute(MoveTempIfPossible(Result.Get<FPragma_PlayerData_PlayerDataResponseProto_Payload>().DeleteNumberData()));
        }
    ));
}

void PragmaPartnerPlayerData::DemoSubService::Echo(const FString& PlayerId, const FString& Message, FOnDemoEchoDelegate Delegate) const {
    FPragma_PlayerData_DoOperationPartnerV1Request Request;
    Request.Ext.Payload.SetEcho({Message});
    
    RequestDelegate.ExecuteIfBound(PlayerId, Request.Ext.Payload, FOnPlayerDataResult::CreateLambda(
        [this, Delegate = MoveTemp(Delegate)](PlayerDataResult Result) {
            if (Result.IsType<PlayerDataError>()) {
                Delegate.Execute(NullOpt);
                return;
            }
            Delegate.Execute(MoveTempIfPossible(Result.Get<FPragma_PlayerData_PlayerDataResponseProto_Payload>().Echo()));
        }
    ));
}

void PragmaPartnerPlayerData::DemoSubService::EchoList(const FString& PlayerId, TArray<FString> ListOfStrings, FOnDemoEchoListDelegate Delegate) const {
    FPragma_PlayerData_DoOperationPartnerV1Request Request;
    Request.Ext.Payload.SetEchoList({ListOfStrings});
    
    RequestDelegate.ExecuteIfBound(PlayerId, Request.Ext.Payload, FOnPlayerDataResult::CreateLambda(
        [this, Delegate = MoveTemp(Delegate)](PlayerDataResult Result) {
            if (Result.IsType<PlayerDataError>()) {
                Delegate.Execute(NullOpt);
                return;
            }
            Delegate.Execute(MoveTempIfPossible(Result.Get<FPragma_PlayerData_PlayerDataResponseProto_Payload>().EchoList()));
        }
    ));
}

void PragmaPartnerPlayerData::DemoSubService::HideStuffFromPlayer(const FString& PlayerId, FOnDemoHideStuffFromPlayerDelegate Delegate) const {
    FPragma_PlayerData_DoOperationPartnerV1Request Request;
    Request.Ext.Payload.SetHideStuffFromPlayer({});
    
    RequestDelegate.ExecuteIfBound(PlayerId, Request.Ext.Payload, FOnPlayerDataResult::CreateLambda(
        [this, Delegate = MoveTemp(Delegate)](PlayerDataResult Result) {
            if (Result.IsType<PlayerDataError>()) {
                Delegate.Execute(NullOpt);
                return;
            }
            Delegate.Execute(MoveTempIfPossible(Result.Get<FPragma_PlayerData_PlayerDataResponseProto_Payload>().HideStuffFromPlayer()));
        }
    ));
}