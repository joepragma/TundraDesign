#pragma once
#include "PragmaPlayerApi.h"
#include "PragmaResult.h"
#include "Dto/PragmaGameDataServiceRaw.h"
#include "Dto/PragmaGameDataRpcDto.h"

#include "PragmaGameDataService.generated.h"

class UPragmaGameDataServiceRaw;

DECLARE_DELEGATE_OneParam(FOnCompleteGetLoginDataV3Delegate, TPragmaResult<FPragma_GameData_LoginDataV3>);

UCLASS()
class PRAGMASDK_API UPragmaGameDataService : public UPragmaPlayerApi
{
	GENERATED_BODY()

public:
	UPragmaGameDataService()
	{
	}

	void SetDependencies(UPragmaGameDataServiceRaw* InGameDataServiceRaw);

	// Access to this service's Raw API methods. Methods on the Raw API bypass any logic or caching provided by this service.
	// All capabilities should be available on this service, use the Raw API only for custom functionality.
	virtual UPragmaGameDataServiceRaw& Raw() const;

	//
	// Get LoginDataV3 from the GameDataService. By default this will contain inventory content and limited grants issued.
	// With a custom LoginDataPlugin and DependentJob other data can be returned through an ext of the payload.
	//
	void GetLoginDataV3(const FOnCompleteGetLoginDataV3Delegate OnComplete) const;
	DECLARE_EVENT_OneParam(UPragmaGameDataService, FGetLoginDataV3Event, FPragma_GameData_LoginDataV3);
	FGetLoginDataV3Event OnGetLoginDataV3;

private:
	UPROPERTY()
	UPragmaGameDataServiceRaw* GameDataServiceRaw;
};
