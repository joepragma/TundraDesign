#include "PragmaGameServerSubsystem.h"

#include "PragmaGameInstanceSubsystem.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

void UPragmaGameServerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Collection.InitializeDependency<UPragmaGameInstanceSubsystem>();
	ServerPtr = Runtime()->GetOrCreateServer();
}

void UPragmaGameServerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	// Check important during shutdown.
	if (Runtime() != nullptr)
	{
		Runtime()->DestroyServer();
	}
}

bool UPragmaGameServerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!Outer || !Outer->GetWorld())
	{
		return false;
	}
	switch (Outer->GetWorld()->GetNetMode())
	{
		case NM_DedicatedServer:
			return EnabledDedicatedServer;
		case NM_ListenServer:
			// Not tested, not going to pretend it works.
			// If intended to use listen servers w/ Pragma in prod, there are security issues around clients using protected partner ports.
			// Please discuss with Pragma if this is a use case you have.
			return false;
		default:
			return false;
	}
}

Pragma::FRuntimePtr UPragmaGameServerSubsystem::Runtime() const
{
	const auto* GameInstanceSubsystem = GetGameInstance()->GetSubsystem<UPragmaGameInstanceSubsystem>();
	checkf(GameInstanceSubsystem, TEXT("PragmaGameInstanceSubsystem must be loaded in order to use PragmaGameServerSubsystem."
		"Ensure that it is set to Enabled in configuration. If you override Init in a custom GameInstance, ensure that you call Super::Init() in your override."));
	return GameInstanceSubsystem->Runtime();
}
