#pragma once

#include "Subsystems/LocalPlayerSubsystem.h"
#include "PragmaPtr.h"

#include "PragmaLocalPlayerSubsystem.generated.h"

PRAGMA_FWD(FRuntime);
PRAGMA_FWD(FPlayer);

/**
 * A subsystem which manages the lifetime of a single Pragma Player object for the associated LocalPlayer.
 *
 * This subsystem is a convenience helper that holds a ptr to the Pragma Player, you're free to create your own
 * Player through the Pragma Runtime.
 *
 * If not using this class for Player session management, you should set the following value in DefaultGame.ini config:
 *
 *		[/Script/PragmaSdkAux.PragmaLocalPlayerSubsystem]
 *		Enabled=false
 *
 * Depends on PragmaGameInstanceSubsystem being Enabled.
 */
UCLASS(Config=Game)
class PRAGMASDKAUX_API UPragmaLocalPlayerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override { return Super::ShouldCreateSubsystem(Outer) && Enabled; }

	// Convenience accessor to the Pragma Runtime. Note that the Runtime is technically owned by PragmaGameInstanceSubsystem.
	Pragma::FRuntimePtr Runtime() const;

	// Get the Pragma Player session associated with this LocalPlayer. The ptr is valid for the lifetime of this subsystem.
	Pragma::FPlayerPtr Player() const { return PlayerPtr; }

private:
	Pragma::FPlayerPtr PlayerPtr;

	UPROPERTY(Config)
	bool Enabled{true};
};
