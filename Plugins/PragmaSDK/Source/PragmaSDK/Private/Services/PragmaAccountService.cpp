#include "Services/PragmaAccountService.h"

#include "Dto/PragmaAccountServiceRaw.h"
#include "PragmaLog.h"
#include "PragmaPlayer.h"

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
