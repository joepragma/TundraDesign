#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "PragmaPtr.h"

#include "PragmaGameServerSubsystem.generated.h"

PRAGMA_FWD(FRuntime);
PRAGMA_FWD(FServer);

/**
 * A subsystem which holds a single Pragma Server ptr. This subsystem is only created when running as a Dedicated or Listen server.
 *
 * That this subsystem is a convenience helper that holds a ptr to the Pragma Server, you're free to create your own
 * Server through the Pragma Runtime.
 *
 * If not using this class for Game Server session management, you should set the following values in DefaultGame.ini config:
 *
 *		[/Script/PragmaSdkAux.PragmaGameServerSubsystem]
 *		EnabledDedicatedServer=false
 *		EnabledListenServer=false
 */
UCLASS(Config=Game)
class PRAGMASDKAUX_API UPragmaGameServerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	// Convenience accessor to the Pragma Runtime. Note that the Runtime is technically owned by PragmaGameInstanceSubsystem.
	Pragma::FRuntimePtr Runtime() const;
	
	// Get the Pragma Server session. The ptr is valid for the lifetime of this subsystem.
	Pragma::FServerPtr Server() const { return ServerPtr; }

private:
	Pragma::FServerPtr ServerPtr;
	
	UPROPERTY(Config)
	bool EnabledDedicatedServer{true};
};
