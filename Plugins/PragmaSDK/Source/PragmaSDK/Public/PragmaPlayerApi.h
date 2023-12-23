#pragma once

#include "PragmaPtr.h"
#include "PragmaSessionApiMacros.h" // Included here for simplified child class access.
#include "PragmaPlayerApi.generated.h"

PRAGMA_FWD(FPlayer);
PRAGMA_FWD(FSession);

UCLASS(Transient, Abstract, Category=Pragma)
class PRAGMASDK_API UPragmaPlayerApi : public UObject
{
	GENERATED_BODY()

public:
	void Init(Pragma::FPlayerPtr Player);

	// Add notification handlers here in child implementations.
	virtual void OnInitialized() {}

	Pragma::FPlayerPtr Player() const;

	// Same as Player() but using parent ptr. Simplifies codegen.
	Pragma::FSessionPtr Session() const;

private:
	Pragma::FPlayerPtr PlayerPtr;
};
