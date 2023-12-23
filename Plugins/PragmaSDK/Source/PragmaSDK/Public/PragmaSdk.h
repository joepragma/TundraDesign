#pragma once

#include "CoreMinimal.h"

#include "Modules/ModuleInterface.h"

class FPragmaSdkModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
