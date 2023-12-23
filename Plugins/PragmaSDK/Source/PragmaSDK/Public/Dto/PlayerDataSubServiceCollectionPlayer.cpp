#include "Dto/PlayerDataSubServiceCollectionPlayer.h"

/* This file was auto-generated, and should not be manually edited. See pragma.playerdata.codegen.generators.SubServiceUnrealGenerator */

void PragmaPlayerData::PlayerDataOperationsSubServiceCollection::CreateSubServices(FPlayerDataRequestDelegate Delegate) { 
        MySubServiceInstance.SetDependencies(Delegate);
        LoadSubServiceInstance.SetDependencies(Delegate);
        DemoSubServiceInstance.SetDependencies(Delegate);
        ContentSubServiceInstance.SetDependencies(Delegate);
}