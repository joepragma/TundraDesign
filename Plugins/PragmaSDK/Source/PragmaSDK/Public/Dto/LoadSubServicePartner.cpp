/* This file was auto-generated, and should not be manually edited. See pragma.playerdata.codegen.generators.SubServiceUnrealGenerator */

#include "LoadSubServicePartner.h"
#include "PragmaMessageMetadata.h"
#include "Dto/PragmaPlayerDataServiceRpcDto.h"

void PragmaPartnerPlayerData::LoadSubService::SetDependencies(FPlayerDataRequestDelegate Delegate) {
    RequestDelegate = MoveTemp(Delegate);
}

void PragmaPartnerPlayerData::LoadSubService::BigDataEntity(const FString& PlayerId, FOnLoadBigDataEntityDelegate Delegate) const {
    FPragma_PlayerData_DoOperationPartnerV1Request Request;
    Request.Ext.Payload.SetBigDataEntity({});
    
    RequestDelegate.ExecuteIfBound(PlayerId, Request.Ext.Payload, FOnPlayerDataResult::CreateLambda(
        [this, Delegate = MoveTemp(Delegate)](PlayerDataResult Result) {
            if (Result.IsType<PlayerDataError>()) {
                Delegate.Execute(NullOpt);
                return;
            }
            Delegate.Execute(MoveTempIfPossible(Result.Get<FPragma_PlayerData_PlayerDataResponseProto_Payload>().BigDataEntity()));
        }
    ));
}

void PragmaPartnerPlayerData::LoadSubService::EmptyOperation(const FString& PlayerId, FOnLoadEmptyOperationDelegate Delegate) const {
    FPragma_PlayerData_DoOperationPartnerV1Request Request;
    Request.Ext.Payload.SetEmptyOperation({});
    
    RequestDelegate.ExecuteIfBound(PlayerId, Request.Ext.Payload, FOnPlayerDataResult::CreateLambda(
        [this, Delegate = MoveTemp(Delegate)](PlayerDataResult Result) {
            if (Result.IsType<PlayerDataError>()) {
                Delegate.Execute(NullOpt);
                return;
            }
            Delegate.Execute(MoveTempIfPossible(Result.Get<FPragma_PlayerData_PlayerDataResponseProto_Payload>().EmptyOperation()));
        }
    ));
}

void PragmaPartnerPlayerData::LoadSubService::GrantStarterItems(const FString& PlayerId, FOnLoadGrantStarterItemsDelegate Delegate) const {
    FPragma_PlayerData_DoOperationPartnerV1Request Request;
    Request.Ext.Payload.SetGrantStarterItems({});
    
    RequestDelegate.ExecuteIfBound(PlayerId, Request.Ext.Payload, FOnPlayerDataResult::CreateLambda(
        [this, Delegate = MoveTemp(Delegate)](PlayerDataResult Result) {
            if (Result.IsType<PlayerDataError>()) {
                Delegate.Execute(NullOpt);
                return;
            }
            Delegate.Execute(MoveTempIfPossible(Result.Get<FPragma_PlayerData_PlayerDataResponseProto_Payload>().GrantStarterItems()));
        }
    ));
}

void PragmaPartnerPlayerData::LoadSubService::ManyComponentEntity(const FString& PlayerId, FOnLoadManyComponentEntityDelegate Delegate) const {
    FPragma_PlayerData_DoOperationPartnerV1Request Request;
    Request.Ext.Payload.SetManyComponentEntity({});
    
    RequestDelegate.ExecuteIfBound(PlayerId, Request.Ext.Payload, FOnPlayerDataResult::CreateLambda(
        [this, Delegate = MoveTemp(Delegate)](PlayerDataResult Result) {
            if (Result.IsType<PlayerDataError>()) {
                Delegate.Execute(NullOpt);
                return;
            }
            Delegate.Execute(MoveTempIfPossible(Result.Get<FPragma_PlayerData_PlayerDataResponseProto_Payload>().ManyComponentEntity()));
        }
    ));
}