#pragma once
#include "PragmaServerApi.h"
#include "Dto/PragmaInventoryPartnerServiceRaw.h"

#include "PragmaInventoryPartnerService.generated.h"

UCLASS()
class PRAGMASDK_API UPragmaInventoryPartnerService final : public UPragmaServerApi
{
	GENERATED_BODY()

public:
	void SetDependencies(UPragmaInventoryPartnerServiceRaw* InventoryRaw);
	virtual void OnInitialized() override;
	
	// Access to this service's Raw API methods. Methods on the Raw API bypass any logic or caching provided by this service.
	// All capabilities should be available on this service, use the Raw API only for custom functionality.
	UPragmaInventoryPartnerServiceRaw& Raw() const;

private:
	UPROPERTY()
	UPragmaInventoryPartnerServiceRaw* InventoryServiceRaw;
};
