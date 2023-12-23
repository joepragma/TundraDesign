/* This file was auto-generated, and should not be manually edited. See pragma.playerdata.codegen.generators.SubServiceUnrealGenerator */

#include "LoadSubServicePlayer.h"
#include "PragmaMessageMetadata.h"
#include "Dto/PragmaPlayerDataServiceRpcDto.h"

void PragmaPlayerData::LoadSubService::SetDependencies(FPlayerDataRequestDelegate Delegate) {
    RequestDelegate = MoveTemp(Delegate);
}

void PragmaPlayerData::LoadSubService::BigDataEntity(FOnLoadBigDataEntityDelegate Delegate) const {
    FPragma_PlayerData_DoOperationV1Request Request;
    Request.Ext.Payload.SetBigDataEntity({});
    
    RequestDelegate.ExecuteIfBound(Request.Ext.Payload, FOnPlayerDataResult::CreateLambda(
        [this, Delegate = MoveTemp(Delegate)](PlayerDataResult Result) {
            if (Result.IsType<PlayerDataError>()) {
                Delegate.Execute(NullOpt);
                return;
            }
            Delegate.Execute(MoveTempIfPossible(Result.Get<FPragma_PlayerData_PlayerDataResponseProto_Payload>().BigDataEntity()));
        }
    ));
}

void PragmaPlayerData::LoadSubService::ContentDrivenGrants(FOnLoadContentDrivenGrantsDelegate Delegate) const {
    FPragma_PlayerData_DoOperationV1Request Request;
    Request.Ext.Payload.SetContentDrivenGrants({});
    
    RequestDelegate.ExecuteIfBound(Request.Ext.Payload, FOnPlayerDataResult::CreateLambda(
        [this, Delegate = MoveTemp(Delegate)](PlayerDataResult Result) {
            if (Result.IsType<PlayerDataError>()) {
                Delegate.Execute(NullOpt);
                return;
            }
            Delegate.Execute(MoveTempIfPossible(Result.Get<FPragma_PlayerData_PlayerDataResponseProto_Payload>().ContentDrivenGrants()));
        }
    ));
}

void PragmaPlayerData::LoadSubService::EmptyOperation(FOnLoadEmptyOperationDelegate Delegate) const {
    FPragma_PlayerData_DoOperationV1Request Request;
    Request.Ext.Payload.SetEmptyOperation({});
    
    RequestDelegate.ExecuteIfBound(Request.Ext.Payload, FOnPlayerDataResult::CreateLambda(
        [this, Delegate = MoveTemp(Delegate)](PlayerDataResult Result) {
            if (Result.IsType<PlayerDataError>()) {
                Delegate.Execute(NullOpt);
                return;
            }
            Delegate.Execute(MoveTempIfPossible(Result.Get<FPragma_PlayerData_PlayerDataResponseProto_Payload>().EmptyOperation()));
        }
    ));
}

void PragmaPlayerData::LoadSubService::GrantStarterItems(FOnLoadGrantStarterItemsDelegate Delegate) const {
    FPragma_PlayerData_DoOperationV1Request Request;
    Request.Ext.Payload.SetGrantStarterItems({});
    
    RequestDelegate.ExecuteIfBound(Request.Ext.Payload, FOnPlayerDataResult::CreateLambda(
        [this, Delegate = MoveTemp(Delegate)](PlayerDataResult Result) {
            if (Result.IsType<PlayerDataError>()) {
                Delegate.Execute(NullOpt);
                return;
            }
            Delegate.Execute(MoveTempIfPossible(Result.Get<FPragma_PlayerData_PlayerDataResponseProto_Payload>().GrantStarterItems()));
        }
    ));
}

void PragmaPlayerData::LoadSubService::LoadTestEntityTransform(FOnLoadLoadTestEntityTransformDelegate Delegate) const {
    FPragma_PlayerData_DoOperationV1Request Request;
    Request.Ext.Payload.SetLoadTestEntityTransform({});
    
    RequestDelegate.ExecuteIfBound(Request.Ext.Payload, FOnPlayerDataResult::CreateLambda(
        [this, Delegate = MoveTemp(Delegate)](PlayerDataResult Result) {
            if (Result.IsType<PlayerDataError>()) {
                Delegate.Execute(NullOpt);
                return;
            }
            Delegate.Execute(MoveTempIfPossible(Result.Get<FPragma_PlayerData_PlayerDataResponseProto_Payload>().LoadTestEntityTransform()));
        }
    ));
}

void PragmaPlayerData::LoadSubService::ManyComponentEntity(FOnLoadManyComponentEntityDelegate Delegate) const {
    FPragma_PlayerData_DoOperationV1Request Request;
    Request.Ext.Payload.SetManyComponentEntity({});
    
    RequestDelegate.ExecuteIfBound(Request.Ext.Payload, FOnPlayerDataResult::CreateLambda(
        [this, Delegate = MoveTemp(Delegate)](PlayerDataResult Result) {
            if (Result.IsType<PlayerDataError>()) {
                Delegate.Execute(NullOpt);
                return;
            }
            Delegate.Execute(MoveTempIfPossible(Result.Get<FPragma_PlayerData_PlayerDataResponseProto_Payload>().ManyComponentEntity()));
        }
    ));
}