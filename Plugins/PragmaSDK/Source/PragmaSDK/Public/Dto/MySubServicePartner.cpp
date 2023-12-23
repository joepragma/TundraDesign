/* This file was auto-generated, and should not be manually edited. See pragma.playerdata.codegen.generators.SubServiceUnrealGenerator */

#include "MySubServicePartner.h"
#include "PragmaMessageMetadata.h"
#include "Dto/PragmaPlayerDataServiceRpcDto.h"

void PragmaPartnerPlayerData::MySubService::SetDependencies(FPlayerDataRequestDelegate Delegate) {
    RequestDelegate = MoveTemp(Delegate);
}
