#include "PragmaSdk.h"

#include "Modules/ModuleManager.h"
#include "WebSocketsModule.h"
#include "PragmaLog.h"
#include "UObject/StrongObjectPtr.h"

void FPragmaSdkModule::StartupModule()
{
	PRAGMA_LOG(Log, "Module PragmaSdk starting up.");
	// Ensure websockets module is loaded before we start.
	FModuleManager::LoadModuleChecked<FWebSocketsModule>("WebSockets");
}

void FPragmaSdkModule::ShutdownModule()
{
	PRAGMA_LOG(Log, "Module PragmaSdk shutting down.");
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPragmaSdkModule, PragmaSDK)
