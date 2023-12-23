#include "PragmaSdkAux.h"

#include "Modules/ModuleManager.h"
#include "PragmaLog.h"

void FPragmaSdkAuxModule::StartupModule()
{
	PRAGMA_LOG(Log, "Module PragmaSdkAux starting up");
}

void FPragmaSdkAuxModule::ShutdownModule()
{
	PRAGMA_LOG(Log, "Module PragmaSdkAux shutting down");
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPragmaSdkAuxModule, PragmaSDKAux)
