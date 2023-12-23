/* This file was auto-generated, and should not be manually edited. See pragma.playerdata.codegen.generators.SubServiceUnrealGenerator */

#include "ContentSubServicePartner.h"
#include "PragmaMessageMetadata.h"
#include "Dto/PragmaPlayerDataServiceRpcDto.h"

void PragmaPartnerPlayerData::ContentSubService::SetDependencies(FPlayerDataRequestDelegate Delegate) {
    RequestDelegate = MoveTemp(Delegate);
}

void PragmaPartnerPlayerData::ContentSubService::DemoContentOperation(const FString& PlayerId, FOnContentDemoContentOperationDelegate Delegate) const {
    FPragma_PlayerData_DoOperationPartnerV1Request Request;
    Request.Ext.Payload.SetDemoContentOperation({});
    
    RequestDelegate.ExecuteIfBound(PlayerId, Request.Ext.Payload, FOnPlayerDataResult::CreateLambda(
        [this, Delegate = MoveTemp(Delegate)](PlayerDataResult Result) {
            if (Result.IsType<PlayerDataError>()) {
                Delegate.Execute(NullOpt);
                return;
            }
            Delegate.Execute(MoveTempIfPossible(Result.Get<FPragma_PlayerData_PlayerDataResponseProto_Payload>().DemoContentOperation()));
        }
    ));
}