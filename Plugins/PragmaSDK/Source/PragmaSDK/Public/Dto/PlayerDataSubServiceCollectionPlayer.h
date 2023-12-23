#pragma once

/* This file was auto-generated, and should not be manually edited. See pragma.playerdata.codegen.generators.SubServiceUnrealGenerator */

#include "MySubServicePlayer.h"
#include "LoadSubServicePlayer.h"
#include "DemoSubServicePlayer.h"
#include "ContentSubServicePlayer.h"

class UPragmaPlayerDataServiceRaw;

namespace PragmaPlayerData {

class PRAGMASDK_API PlayerDataOperationsSubServiceCollection {
public:
    MySubService& My() { return MySubServiceInstance; }
    LoadSubService& Load() { return LoadSubServiceInstance; }
    DemoSubService& Demo() { return DemoSubServiceInstance; }
    ContentSubService& Content() { return ContentSubServiceInstance; }

protected:
    void CreateSubServices(FPlayerDataRequestDelegate Delegate);

private:
    MySubService MySubServiceInstance;
    LoadSubService LoadSubServiceInstance;
    DemoSubService DemoSubServiceInstance;
    ContentSubService ContentSubServiceInstance;
};
}