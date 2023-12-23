#include "PragmaLocalPlayerSubsystem.h"
#include "Engine/LocalPlayer.h"
#include "Engine/GameInstance.h"

#include "PragmaGameInstanceSubsystem.h"

void UPragmaLocalPlayerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	PlayerPtr = Runtime()->CreatePlayer();
}

void UPragmaLocalPlayerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	Runtime()->DestroyPlayer(PlayerPtr);
}

Pragma::FRuntimePtr UPragmaLocalPlayerSubsystem::Runtime() const
{
	const auto* GameInstanceSubsystem = GetLocalPlayer<>()->GetGameInstance()->GetSubsystem<UPragmaGameInstanceSubsystem>();
	checkf(GameInstanceSubsystem, TEXT("PragmaGameInstanceSubsystem must be loaded in order to use PragmaGameInstanceSubsystem."
		"Ensure that it is set to Enabled in configuration. If you override Init in a custom GameInstance, ensure that you call Super::Init() in your override."));
	return GameInstanceSubsystem->Runtime();
}
