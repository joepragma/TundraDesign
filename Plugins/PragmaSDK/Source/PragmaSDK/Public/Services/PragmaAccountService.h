#pragma once
#include "Dto/PragmaAccountServiceRaw.h"
#include "Dto/PragmaAccountRpcDto.h"
#include "PragmaResult.h"
#include "PragmaPlayerApi.h"

#include "PragmaAccountService.generated.h"

class UPragmaAccountServiceRaw;

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

private:
	UPROPERTY()
	UPragmaAccountServiceRaw* AccountServiceRaw;
};
