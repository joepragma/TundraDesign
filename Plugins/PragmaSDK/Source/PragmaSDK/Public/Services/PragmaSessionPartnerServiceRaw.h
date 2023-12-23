#pragma once

#include "PragmaServerApi.h"
#include "PragmaSessionServiceRawBase.h"

#include "PragmaSessionPartnerServiceRaw.generated.h"

//
// This class handles Pragma session-level notifications and state.
//
UCLASS()
class PRAGMASDK_API UPragmaSessionPartnerServiceRaw : public UPragmaServerApi, public Pragma::FSessionServiceRawBase
{
	GENERATED_BODY()

public:
	virtual void OnInitialized() override { Initialize(); }
};
