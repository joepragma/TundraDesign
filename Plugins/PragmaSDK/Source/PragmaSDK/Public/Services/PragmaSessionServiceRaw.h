#pragma once

#include "PragmaPlayerApi.h"
#include "PragmaSessionServiceRawBase.h"

#include "PragmaSessionServiceRaw.generated.h"

//
// This class handles Pragma session-level notifications and state.
//
UCLASS()
class PRAGMASDK_API UPragmaSessionServiceRaw : public UPragmaPlayerApi, public Pragma::FSessionServiceRawBase
{
	GENERATED_BODY()

public:
	virtual void OnInitialized() override { Initialize(); }
};
