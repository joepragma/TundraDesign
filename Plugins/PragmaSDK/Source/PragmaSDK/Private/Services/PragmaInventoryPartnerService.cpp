#include "Services/PragmaInventoryPartnerService.h"
#include "Dto/PragmaInventoryPartnerServiceRaw.h"

void UPragmaInventoryPartnerService::SetDependencies(UPragmaInventoryPartnerServiceRaw* InventoryRaw)
{
	InventoryServiceRaw = InventoryRaw;
}

void UPragmaInventoryPartnerService::OnInitialized()
{
}

UPragmaInventoryPartnerServiceRaw& UPragmaInventoryPartnerService::Raw() const
{
	checkf(InventoryServiceRaw != nullptr, TEXT("You need to call SetDependencies before running this."));
	return *InventoryServiceRaw;
}
