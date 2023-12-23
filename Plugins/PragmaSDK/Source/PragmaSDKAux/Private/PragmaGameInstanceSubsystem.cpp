#include "PragmaGameInstanceSubsystem.h"

#include "PragmaLog.h"

void UPragmaGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	PRAGMA_LOG(Log, "Creating new Pragma Runtime.");
	RuntimePtr = Pragma::FRuntime::Create();
}

void UPragmaGameInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();
	RuntimePtr.Reset();
}
