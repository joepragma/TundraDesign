#pragma once

#include "PragmaPlayerApi.h"
#include "PragmaSessionServiceBase.h"

#include "PragmaSessionService.generated.h"

//
// This class handles Pragma session-level notifications and state.
//
UCLASS()
class PRAGMASDK_API UPragmaSessionService : public UPragmaPlayerApi, public Pragma::FSessionServiceBase
{
	GENERATED_BODY()

public:
	virtual void OnInitialized() override { Initialize(); }
	virtual UObject* WeakThisBinding() override { return this; }
};
