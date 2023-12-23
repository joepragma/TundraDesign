#pragma once
#include "Dto/PragmaAccountRpcDto.h"

#include "PragmaAuthenticateDto.generated.h"
typedef uint32 AccountIdProvider;
template<typename T>
FString EnumToString(T EnumValue)
{
	return StaticEnum<T>()->GetNameStringByValue(static_cast<uint32>(EnumValue));
}
//
// The /v1/account/authenticateorcreatev2 call is a http/json only call because it is the call that establishes a session in the first place.
// As such, a Player session AuthenticateOrCreate RPC proto doesn't exist, and these types are hand-created.
//

USTRUCT()
struct FPragma_Account_AuthenticateOrCreateV2Request
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString ProviderId{EnumToString<EPragma_Account_IdProvider>(EPragma_Account_IdProvider::UNUSED)};
	
	UPROPERTY()
	FString ProviderToken;
	
	UPROPERTY()
	FString GameShardId;

	UPROPERTY()
	FString LoginQueuePassToken;
};

USTRUCT()
struct FPragma_Account_AuthenticateOrCreateV2Response
{
	GENERATED_BODY()
	
	UPROPERTY()
	FPragma_Account_PragmaTokens PragmaTokens;
};
