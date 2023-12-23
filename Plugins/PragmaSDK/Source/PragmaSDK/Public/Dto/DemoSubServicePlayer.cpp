/* This file was auto-generated, and should not be manually edited. See pragma.playerdata.codegen.generators.SubServiceUnrealGenerator */

#include "DemoSubServicePlayer.h"
#include "PragmaMessageMetadata.h"
#include "Dto/PragmaPlayerDataServiceRpcDto.h"

void PragmaPlayerData::DemoSubService::SetDependencies(FPlayerDataRequestDelegate Delegate) {
    RequestDelegate = MoveTemp(Delegate);
}

void PragmaPlayerData::DemoSubService::DeleteNumberData(FOnDemoDeleteNumberDataDelegate Delegate) const {
    FPragma_PlayerData_DoOperationV1Request Request;
    Request.Ext.Payload.SetDeleteNumberData({});
    
    RequestDelegate.ExecuteIfBound(Request.Ext.Payload, FOnPlayerDataResult::CreateLambda(
        [this, Delegate = MoveTemp(Delegate)](PlayerDataResult Result) {
            if (Result.IsType<PlayerDataError>()) {
                Delegate.Execute(NullOpt);
                return;
            }
            Delegate.Execute(MoveTempIfPossible(Result.Get<FPragma_PlayerData_PlayerDataResponseProto_Payload>().DeleteNumberData()));
        }
    ));
}

void PragmaPlayerData::DemoSubService::Echo(const FString& Message, FOnDemoEchoDelegate Delegate) const {
    FPragma_PlayerData_DoOperationV1Request Request;
    Request.Ext.Payload.SetEcho({Message});
    
    RequestDelegate.ExecuteIfBound(Request.Ext.Payload, FOnPlayerDataResult::CreateLambda(
        [this, Delegate = MoveTemp(Delegate)](PlayerDataResult Result) {
            if (Result.IsType<PlayerDataError>()) {
                Delegate.Execute(NullOpt);
                return;
            }
            Delegate.Execute(MoveTempIfPossible(Result.Get<FPragma_PlayerData_PlayerDataResponseProto_Payload>().Echo()));
        }
    ));
}

void PragmaPlayerData::DemoSubService::EchoComplex(FPragma_PlayerData_DemoComplexTypeProto DemoComplexType, FOnDemoEchoComplexDelegate Delegate) const {
    FPragma_PlayerData_DoOperationV1Request Request;
    Request.Ext.Payload.SetEchoComplex({DemoComplexType});
    
    RequestDelegate.ExecuteIfBound(Request.Ext.Payload, FOnPlayerDataResult::CreateLambda(
        [this, Delegate = MoveTemp(Delegate)](PlayerDataResult Result) {
            if (Result.IsType<PlayerDataError>()) {
                Delegate.Execute(NullOpt);
                return;
            }
            Delegate.Execute(MoveTempIfPossible(Result.Get<FPragma_PlayerData_PlayerDataResponseProto_Payload>().EchoComplex()));
        }
    ));
}

void PragmaPlayerData::DemoSubService::EchoList(TArray<FString> ListOfStrings, FOnDemoEchoListDelegate Delegate) const {
    FPragma_PlayerData_DoOperationV1Request Request;
    Request.Ext.Payload.SetEchoList({ListOfStrings});
    
    RequestDelegate.ExecuteIfBound(Request.Ext.Payload, FOnPlayerDataResult::CreateLambda(
        [this, Delegate = MoveTemp(Delegate)](PlayerDataResult Result) {
            if (Result.IsType<PlayerDataError>()) {
                Delegate.Execute(NullOpt);
                return;
            }
            Delegate.Execute(MoveTempIfPossible(Result.Get<FPragma_PlayerData_PlayerDataResponseProto_Payload>().EchoList()));
        }
    ));
}

void PragmaPlayerData::DemoSubService::HideStuffFromPlayer(FOnDemoHideStuffFromPlayerDelegate Delegate) const {
    FPragma_PlayerData_DoOperationV1Request Request;
    Request.Ext.Payload.SetHideStuffFromPlayer({});
    
    RequestDelegate.ExecuteIfBound(Request.Ext.Payload, FOnPlayerDataResult::CreateLambda(
        [this, Delegate = MoveTemp(Delegate)](PlayerDataResult Result) {
            if (Result.IsType<PlayerDataError>()) {
                Delegate.Execute(NullOpt);
                return;
            }
            Delegate.Execute(MoveTempIfPossible(Result.Get<FPragma_PlayerData_PlayerDataResponseProto_Payload>().HideStuffFromPlayer()));
        }
    ));
}

void PragmaPlayerData::DemoSubService::SendEmptyPayload(FOnDemoSendEmptyPayloadDelegate Delegate) const {
    FPragma_PlayerData_DoOperationV1Request Request;
    Request.Ext.Payload.SetSendEmptyPayload({});
    
    RequestDelegate.ExecuteIfBound(Request.Ext.Payload, FOnPlayerDataResult::CreateLambda(
        [this, Delegate = MoveTemp(Delegate)](PlayerDataResult Result) {
            if (Result.IsType<PlayerDataError>()) {
                Delegate.Execute(NullOpt);
                return;
            }
            Delegate.Execute(MoveTempIfPossible(Result.Get<FPragma_PlayerData_PlayerDataResponseProto_Payload>().SendEmptyPayload()));
        }
    ));
}