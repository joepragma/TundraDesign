#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "PragmaRuntime.h"

#include "PragmaGameInstanceSubsystem.generated.h"

/**
 * Container to the singular Pragma Runtime.
 *
 * If not using this class for Pragma Runtime management, you should set the following value in DefaultGame.ini config:
 *
 *		[/Script/PragmaSdkAux.PragmaGameInstanceSubsystem]
 *		Enabled=false
 *
 * If overriding this class with your own implementation, you should set the config above to false, and override your subclass's
 * ShouldCreateSubsystem to return true. Otherwise the parent will still be registered and create an unnecessary (and potentially confusing) second Runtime.
 */
UCLASS(Config=Game)
class PRAGMASDKAUX_API UPragmaGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override { return Super::ShouldCreateSubsystem(Outer) && Enabled; }

	// Get a pointer to the Runtime. This pointer will be valid for the duration of the game instance.
	Pragma::FRuntimePtr Runtime() const { return RuntimePtr; }

private:
	Pragma::FRuntimeStrongPtr RuntimePtr;
	
	UPROPERTY(Config)
	bool Enabled{true};
};
