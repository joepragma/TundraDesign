#pragma once

#include "PragmaServerApi.h"
#include "PragmaSessionServiceBase.h"

#include "PragmaSessionPartnerService.generated.h"

//
// This class handles Pragma session-level notifications and state.
//
UCLASS()
class PRAGMASDK_API UPragmaSessionPartnerService : public UPragmaServerApi, public Pragma::FSessionServiceBase
{
	GENERATED_BODY()

public:
	virtual void OnInitialized() override { Initialize(); }
	virtual UObject* WeakThisBinding() override { return this; }
};
