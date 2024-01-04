#pragma once
#include "Dto/PragmaAccountServiceRaw.h"
#include "Dto/PragmaAccountRpcDto.h"
#include "PragmaResult.h"
#include "PragmaPlayerApi.h"

#include "PragmaAccountService.generated.h"

class UPragmaAccountServiceRaw;

// ************************************************************
// *** THIS FILE WAS STUBBED FOR THIS TUNDRA DESIGN PROJECT ***
// ************************************************************

UCLASS()
class PRAGMASDK_API UPragmaAccountService : public UPragmaPlayerApi
{
	GENERATED_BODY()

public:
	virtual ~UPragmaAccountService() override = default;

	void SetDependencies(UPragmaAccountServiceRaw* AccountRaw);

	// Access to this service's Raw API methods. Methods on the Raw API bypass any logic or caching provided by this service.
	// All capabilities should be available on this service, use the Raw API only for custom functionality.
	UPragmaAccountServiceRaw& Raw() const;

	DECLARE_DELEGATE_OneParam(FGetPragmaPlayerIdDelegate, TPragmaResult<FString> /* Player Id */);
	void GetPragmaPlayerIdForDisplayName(
		const FString& DisplayName,
		const FString& Discriminator,
		const FGetPragmaPlayerIdDelegate& OnComplete);

private:
	UPROPERTY()
	UPragmaAccountServiceRaw* AccountServiceRaw;

	void StubbedGetPragmaPlayerIdForDisplayName(const FGetPragmaPlayerIdDelegate& OnComplete) const;
};
