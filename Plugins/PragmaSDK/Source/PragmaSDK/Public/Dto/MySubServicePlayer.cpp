/* This file was auto-generated, and should not be manually edited. See pragma.playerdata.codegen.generators.SubServiceUnrealGenerator */

#include "MySubServicePlayer.h"
#include "PragmaMessageMetadata.h"
#include "Dto/PragmaPlayerDataServiceRpcDto.h"

void PragmaPlayerData::MySubService::SetDependencies(FPlayerDataRequestDelegate Delegate) {
    RequestDelegate = MoveTemp(Delegate);
}

void PragmaPlayerData::MySubService::MyOperation(int32 Int, FOnMyMyOperationDelegate Delegate) const {
    FPragma_PlayerData_DoOperationV1Request Request;
    Request.Ext.Payload.SetMyOperation({Int});
    
    RequestDelegate.ExecuteIfBound(Request.Ext.Payload, FOnPlayerDataResult::CreateLambda(
        [this, Delegate = MoveTemp(Delegate)](PlayerDataResult Result) {
            if (Result.IsType<PlayerDataError>()) {
                Delegate.Execute(NullOpt);
                return;
            }
            Delegate.Execute(MoveTempIfPossible(Result.Get<FPragma_PlayerData_PlayerDataResponseProto_Payload>().MyOperation()));
        }
    ));
}