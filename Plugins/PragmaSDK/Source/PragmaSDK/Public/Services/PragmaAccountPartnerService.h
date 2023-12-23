#pragma once
#include "Dto/PragmaAccountPartnerServiceRaw.h"
#include "Dto/PragmaAccountRpcDto.h"
#include "PragmaResult.h"
#include "PragmaServerApi.h"
#include "Async/Future.h"

#include "PragmaAccountPartnerService.generated.h"

class UPragmaAccountPartnerServiceRaw;

struct PlayerIdentitiesResponse
{
	TArray<FPragma_Account_PlayerIdentity> PlayerIdentities;
	TArray<FPragma_BatchRequestFailure> Failures;
};
    
struct SocialIdentitiesResponse
{
	TArray<FPragma_Account_SocialIdentity> SocialIdentities;
	TArray<FPragma_BatchRequestFailure> Failures;
};

UCLASS()
class PRAGMASDK_API UPragmaAccountPartnerService : public UPragmaServerApi
{
	GENERATED_BODY()

public:
	virtual ~UPragmaAccountPartnerService() override = default;

	void SetDependencies(UPragmaAccountPartnerServiceRaw* AccountRaw);

	// Access to this service's Raw API methods. Methods on the Raw API bypass any logic or caching provided by this service.
	// All capabilities should be available on this service, use the Raw API only for custom functionality.
	UPragmaAccountPartnerServiceRaw& Raw() const;

	//
	// Partner Endpoint for Viewing tags
	//
	DECLARE_DELEGATE_OneParam(FViewTagsDelegate, TPragmaResult<TArray<FPragma_Account_AccountTag>> /* Result */);
	void ViewTags(const FViewTagsDelegate& OnComplete);
	TFuture<TPragmaResult<TArray<FPragma_Account_AccountTag>>> ViewTags();
	
	//
	// Partner Endpoint for Adding Account tags
	//
	typedef TMap<FString, TArray<FPragma_Account_AccountTag>> AccountTagsBySocialId;
	DECLARE_DELEGATE_OneParam(FAddAccountTagsDelegate, TPragmaResult<AccountTagsBySocialId> /* Result */);
	void AddAccountTags(const TArray<FString> SocialIds, const TArray<FString> Tags,
		const FAddAccountTagsDelegate& OnComplete);
	TFuture<TPragmaResult<AccountTagsBySocialId>> AddAccountTags(const TArray<FString> SocialIds,
		const TArray<FString> Tags);

	//
	// Partner Endpoint for Getting Account tags
	//
	DECLARE_DELEGATE_OneParam(FGetAccountTagsDelegate, TPragmaResult<AccountTagsBySocialId> /* Result */);
	void GetAccountTags(const TArray<FString> SocialIds, const FGetAccountTagsDelegate& OnComplete);
	TFuture<TPragmaResult<AccountTagsBySocialId>> GetAccountTags(const TArray<FString> SocialIds);

	//
	// Partner Endpoint for Removing Account tags
	//
	DECLARE_DELEGATE_OneParam(FRemoveAccountTagsDelegate, TPragmaResult<AccountTagsBySocialId> /* Result */);
	void RemoveAccountTags(const TArray<FString> SocialIds, const TArray<FString> TagIds,
		const FRemoveAccountTagsDelegate& OnComplete);
	TFuture<TPragmaResult<AccountTagsBySocialId>> RemoveAccountTags(const TArray<FString> SocialIds,
		const TArray<FString> TagIds);

	//
	// Partner Endpoint for adding an Account to a Player Group
	//
	DECLARE_DELEGATE_OneParam(FAddAccountToPlayerGroupDelegate, TPragmaResult<TArray<FString>> /* Result */);
	void AddAccountToPlayerGroup(const FString PlayerGroupId, const FString SocialId, const FAddAccountToPlayerGroupDelegate& OnComplete);
	TFuture<TPragmaResult<TArray<FString>>> AddAccountToPlayerGroup(const FString PlayerGroupId, const FString SocialId);

	//
	// Partner Endpoint for removing an Account from a Player Group
	//
	DECLARE_DELEGATE_OneParam(FRemoveAccountFromPlayerGroupDelegate, TPragmaResult<TArray<FString>> /* Result */);
	void RemoveAccountFromPlayerGroup(const FString PlayerGroupId, const FString SocialId, const FRemoveAccountFromPlayerGroupDelegate& OnComplete);
	TFuture<TPragmaResult<TArray<FString>>> RemoveAccountFromPlayerGroup(const FString PlayerGroupId, const FString SocialId);

	//
	// Partner Endpoint for retrieving Player Identities
	//
	DECLARE_DELEGATE_OneParam(FGetPlayerIdentitiesDelegate, TPragmaResult<PlayerIdentitiesResponse> /* Result */);
	void GetPlayerIdentities(const TArray<FString> PlayerIds, const FGetPlayerIdentitiesDelegate& OnComplete);
	TFuture<TPragmaResult<PlayerIdentitiesResponse>> GetPlayerIdentities(const TArray<FString> PlayerIds);

	//
	// Partner Endpoint for retrieving Social Identities
	//
	DECLARE_DELEGATE_OneParam(FGetSocialIdentitiesDelegate, TPragmaResult<SocialIdentitiesResponse> /* Result */);
	void GetSocialIdentities(const TArray<FString> SocialIds, const FGetSocialIdentitiesDelegate& OnComplete);
	TFuture<TPragmaResult<SocialIdentitiesResponse>> GetSocialIdentities(const TArray<FString> SocialIds);

private:
	UPROPERTY()
	UPragmaAccountPartnerServiceRaw* AccountServiceRaw;
};
