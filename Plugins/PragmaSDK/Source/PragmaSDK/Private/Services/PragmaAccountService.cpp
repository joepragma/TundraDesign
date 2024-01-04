#include "Services/PragmaAccountService.h"

#include "Dto/PragmaAccountServiceRaw.h"
#include "PragmaLog.h"
#include "PragmaPlayer.h"

// ************************************************************
// *** THIS FILE WAS STUBBED FOR THIS TUNDRA DESIGN PROJECT ***
// ************************************************************

namespace
{
const FString& CoreSdkDoNotCall(const FString& Alternate)
{
	PRAGMA_LOG(Error, "When CoreSDK is enabled, you must use the %s method.", *Alternate);
	static FString Empty;
	return Empty;
}
}

void UPragmaAccountService::SetDependencies(UPragmaAccountServiceRaw* AccountRaw)
{
	AccountServiceRaw = AccountRaw;
}

UPragmaAccountServiceRaw& UPragmaAccountService::Raw() const
{
	return *AccountServiceRaw;
}

void UPragmaAccountService::GetPragmaPlayerIdForDisplayName(
	const FString& DisplayName,
	const FString& Discriminator,
	const FGetPragmaPlayerIdDelegate& OnComplete)
{
	StubbedGetPragmaPlayerIdForDisplayName(OnComplete);
}

void UPragmaAccountService::StubbedGetPragmaPlayerIdForDisplayName(const FGetPragmaPlayerIdDelegate& OnComplete) const
{
	OnComplete.ExecuteIfBound(TPragmaResult(FString("FakePragmaPlayerId")));
}
