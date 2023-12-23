#include "Services/PragmaGameDataService.h"
#include "Dto/PragmaGameDataServiceRaw.h"

void UPragmaGameDataService::SetDependencies(UPragmaGameDataServiceRaw* InGameDataServiceRaw)
{
	GameDataServiceRaw = InGameDataServiceRaw;
}

UPragmaGameDataServiceRaw& UPragmaGameDataService::Raw() const
{
	checkf(GameDataServiceRaw != nullptr, TEXT("You need to call SetDependencies before running this."));
	return *GameDataServiceRaw;
}

void UPragmaGameDataService::GetLoginDataV3(FOnCompleteGetLoginDataV3Delegate OnComplete) const
{
	Raw().GetLoginDataV3(FPragma_GameData_GetLoginDataV3Request{}, UPragmaGameDataServiceRaw::FGetLoginDataV3Delegate::CreateLambda(
		[this, Delegate = MoveTemp(OnComplete)]
		(TPragmaResult<FPragma_GameData_GetLoginDataV3Response> Result, const FPragmaMessageMetadata& _)
		{
			if (Result.IsSuccessful())
			{
				OnGetLoginDataV3.Broadcast(Result.Payload().LoginData);
				Delegate.ExecuteIfBound(TPragmaResult<FPragma_GameData_LoginDataV3>(MoveTemp(Result.Payload().LoginData)));
			} else
			{
				Delegate.ExecuteIfBound(TPragmaResult<FPragma_GameData_LoginDataV3>::Failure(Result.Error()));
			}
		}));
}
