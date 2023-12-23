#pragma once

#include "PragmaPtr.h"
#include "PragmaSessionApiMacros.h" // Included here for simplified child class access.
#include "PragmaServerApi.generated.h"

PRAGMA_FWD(FSession);
PRAGMA_FWD(FServer);

UCLASS(Transient, Abstract, Category=Pragma)
class PRAGMASDK_API UPragmaServerApi : public UObject
{
	GENERATED_BODY()

public:
	void Init(Pragma::FServerPtr Server);

	// Add notification handlers here in child implementations.
	virtual void OnInitialized() {}

	Pragma::FServerPtr Server() const;

	// Same as Player() but using parent ptr. Simplifies codegen.
	Pragma::FSessionPtr Session() const;

private:
	Pragma::FServerPtr ServerPtr;
};
