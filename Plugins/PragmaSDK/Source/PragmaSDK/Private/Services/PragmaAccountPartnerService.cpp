#include "Services/PragmaAccountPartnerService.h"

#include "Dto/PragmaAccountPartnerServiceRaw.h"
#include "PragmaLog.h"

void UPragmaAccountPartnerService::SetDependencies(UPragmaAccountPartnerServiceRaw* AccountRaw)
{
	AccountServiceRaw = AccountRaw;
}

UPragmaAccountPartnerServiceRaw& UPragmaAccountPartnerService::Raw() const
{
	return *AccountServiceRaw;
}

void UPragmaAccountPartnerService::ViewTags(const FViewTagsDelegate& OnComplete)
{
	const auto Request = FPragma_Account_ViewTagsPartnerV1Request{};
	Raw().ViewTagsPartnerV1(
		Request,
		UPragmaAccountPartnerServiceRaw::FViewTagsPartnerV1Delegate::CreateWeakLambda(this,
			[this, OnComplete](const TPragmaResult<FPragma_Account_ViewTagsPartnerV1Response> Result,
			const FPragmaMessageMetadata&)
			{
				if (Result.IsFailure())
				{
					OnComplete.ExecuteIfBound(
						TPragmaResult<TArray<FPragma_Account_AccountTag>>::Failure(Result.Error()));
					return;
				}

				OnComplete.ExecuteIfBound(
					TPragmaResult<TArray<FPragma_Account_AccountTag>>(Result.Payload().AccountTags));
			}
		));
}

TFuture<TPragmaResult<TArray<FPragma_Account_AccountTag>>> UPragmaAccountPartnerService::ViewTags()
{
	auto Promise = MakeShared<TPromise<TPragmaResult<TArray<FPragma_Account_AccountTag>>>>();
	ViewTags(FViewTagsDelegate::CreateLambda([Promise](TPragmaResult<TArray<FPragma_Account_AccountTag>> Result)
	{
		Promise->SetValue(MoveTemp(Result));
	}));
	return Promise->GetFuture();
}

void UPragmaAccountPartnerService::AddAccountTags(const TArray<FString> SocialIds, const TArray<FString> Tags,
	const FAddAccountTagsDelegate& OnComplete)
{
	const auto Request = FPragma_Account_AddAccountTagsPartnerV1Request{
		FPragma_Account_AddAccountTagsRequest{SocialIds, Tags}
	};
	Raw().AddAccountTagsPartnerV1(
		Request,
		UPragmaAccountPartnerServiceRaw::FAddAccountTagsPartnerV1Delegate::CreateWeakLambda(this,
			[this, OnComplete](const TPragmaResult<FPragma_Account_AddAccountTagsPartnerV1Response> Result,
			const FPragmaMessageMetadata&)
			{
				if (Result.IsFailure())
				{
					OnComplete.ExecuteIfBound(TPragmaResult<AccountTagsBySocialId>::Failure(Result.Error()));
					return;
				}

				auto Response = AccountTagsBySocialId{};
				for (const auto& AccountTag : Result.Payload().AccountTagsBySocialId)
				{
					Response.Add(AccountTag.PragmaSocialId, AccountTag.AccountTags);
				}
				OnComplete.ExecuteIfBound(TPragmaResult<AccountTagsBySocialId>(Response));
			}
		));
}

TFuture<TPragmaResult<UPragmaAccountPartnerService::AccountTagsBySocialId>> UPragmaAccountPartnerService::AddAccountTags(
	const TArray<FString> SocialIds, const TArray<FString> Tags)
{
	auto Promise = MakeShared<TPromise<TPragmaResult<AccountTagsBySocialId>>>();
	AddAccountTags(SocialIds, Tags, FAddAccountTagsDelegate::CreateLambda(
		[Promise](TPragmaResult<AccountTagsBySocialId> Result)
		{
			Promise->SetValue(MoveTemp(Result));
		}));
	return Promise->GetFuture();
}

void UPragmaAccountPartnerService::GetAccountTags(const TArray<FString> SocialIds, const FGetAccountTagsDelegate& OnComplete)
{
	const auto Request = FPragma_Account_GetAccountTagsPartnerV1Request{SocialIds};
	Raw().GetAccountTagsPartnerV1(
		Request,
		UPragmaAccountPartnerServiceRaw::FGetAccountTagsPartnerV1Delegate::CreateWeakLambda(this,
			[this, OnComplete](const TPragmaResult<FPragma_Account_GetAccountTagsPartnerV1Response> Result,
			const FPragmaMessageMetadata&)
			{
				if (Result.IsFailure())
				{
					OnComplete.ExecuteIfBound(TPragmaResult<AccountTagsBySocialId>::Failure(Result.Error()));
					return;
				}

				auto Response = AccountTagsBySocialId{};
				for (const auto& AccountTag : Result.Payload().AccountTagsBySocialId)
				{
					Response.Add(AccountTag.PragmaSocialId, AccountTag.AccountTags);
				}
				OnComplete.ExecuteIfBound(TPragmaResult<AccountTagsBySocialId>(Response));
			}
		));
}

TFuture<TPragmaResult<UPragmaAccountPartnerService::AccountTagsBySocialId>> UPragmaAccountPartnerService::GetAccountTags(
	const TArray<FString> SocialIds)
{
	auto Promise = MakeShared<TPromise<TPragmaResult<AccountTagsBySocialId>>>();
	GetAccountTags(SocialIds, FGetAccountTagsDelegate::CreateLambda(
		[Promise](TPragmaResult<AccountTagsBySocialId> Result)
		{
			Promise->SetValue(MoveTemp(Result));
		}));
	return Promise->GetFuture();
}

void UPragmaAccountPartnerService::RemoveAccountTags(const TArray<FString> SocialIds, const TArray<FString> TagIds,
	const FRemoveAccountTagsDelegate& OnComplete)
{
	const auto Request = FPragma_Account_RemoveAccountTagsPartnerV1Request{
		FPragma_Account_RemoveAccountTagsRequest{SocialIds, TagIds}
	};
	Raw().RemoveAccountTagsPartnerV1(
		Request,
		UPragmaAccountPartnerServiceRaw::FRemoveAccountTagsPartnerV1Delegate::CreateWeakLambda(this,
			[this, OnComplete](const TPragmaResult<FPragma_Account_RemoveAccountTagsPartnerV1Response> Result,
			const FPragmaMessageMetadata&)
			{
				if (Result.IsFailure())
				{
					OnComplete.ExecuteIfBound(TPragmaResult<AccountTagsBySocialId>::Failure(Result.Error()));
					return;
				}

				auto Response = AccountTagsBySocialId{};
				for (const auto& AccountTag : Result.Payload().AccountTagsBySocialId)
				{
					Response.Add(AccountTag.PragmaSocialId, AccountTag.AccountTags);
				}
				OnComplete.ExecuteIfBound(TPragmaResult<AccountTagsBySocialId>(Response));
			}
		));
}

TFuture<TPragmaResult<UPragmaAccountPartnerService::AccountTagsBySocialId>> UPragmaAccountPartnerService::RemoveAccountTags(
	const TArray<FString> SocialIds, const TArray<FString> TagIds)
{
	auto Promise = MakeShared<TPromise<TPragmaResult<AccountTagsBySocialId>>>();
	RemoveAccountTags(SocialIds, TagIds, FRemoveAccountTagsDelegate::CreateLambda(
		[Promise](TPragmaResult<AccountTagsBySocialId> Result)
		{
			Promise->SetValue(MoveTemp(Result));
		}));
	return Promise->GetFuture();
}

void UPragmaAccountPartnerService::AddAccountToPlayerGroup(const FString PlayerGroupId, const FString SocialId, const FAddAccountToPlayerGroupDelegate& OnComplete)
{
	const auto Request = FPragma_Account_AddAccountToPlayerGroupPartnerV1Request{ PlayerGroupId, SocialId };
	Raw().AddAccountToPlayerGroupPartnerV1(
		Request,
		UPragmaAccountPartnerServiceRaw::FAddAccountToPlayerGroupPartnerV1Delegate::CreateWeakLambda(this,
			[this, OnComplete](const TPragmaResult<FPragma_Account_AddAccountToPlayerGroupPartnerV1Response> Result,
			const FPragmaMessageMetadata&)
			{
				if (Result.IsFailure())
				{
					OnComplete.ExecuteIfBound(
						TPragmaResult<TArray<FString>>::Failure(Result.Error()));
					return;
				}

				OnComplete.ExecuteIfBound(
					TPragmaResult<TArray<FString>>(Result.Payload().PlayerGroupIds));
			}
		));
}

TFuture<TPragmaResult<TArray<FString>>> UPragmaAccountPartnerService::AddAccountToPlayerGroup(const FString PlayerGroupId, const FString SocialId)
{
	auto Promise = MakeShared<TPromise<TPragmaResult<TArray<FString>>>>();
	AddAccountToPlayerGroup(PlayerGroupId, SocialId, FAddAccountToPlayerGroupDelegate::CreateLambda([Promise](TPragmaResult<TArray<FString>> Result)
	{
		Promise->SetValue(MoveTemp(Result));
	}));
	return Promise->GetFuture();
}

void UPragmaAccountPartnerService::RemoveAccountFromPlayerGroup(const FString PlayerGroupId, const FString SocialId, const FRemoveAccountFromPlayerGroupDelegate& OnComplete)
{
	const auto Request = FPragma_Account_RemoveAccountFromPlayerGroupPartnerV1Request{ PlayerGroupId, SocialId };
	Raw().RemoveAccountFromPlayerGroupPartnerV1(
		Request,
		UPragmaAccountPartnerServiceRaw::FRemoveAccountFromPlayerGroupPartnerV1Delegate::CreateWeakLambda(this,
			[this, OnComplete](const TPragmaResult<FPragma_Account_RemoveAccountFromPlayerGroupPartnerV1Response> Result,
			const FPragmaMessageMetadata&)
			{
				if (Result.IsFailure())
				{
					OnComplete.ExecuteIfBound(
						TPragmaResult<TArray<FString>>::Failure(Result.Error()));
					return;
				}

				OnComplete.ExecuteIfBound(
					TPragmaResult<TArray<FString>>(Result.Payload().PlayerGroupIds));
			}
		));
}

TFuture<TPragmaResult<TArray<FString>>> UPragmaAccountPartnerService::RemoveAccountFromPlayerGroup(const FString PlayerGroupId, const FString SocialId)
{
	auto Promise = MakeShared<TPromise<TPragmaResult<TArray<FString>>>>();
	RemoveAccountFromPlayerGroup(PlayerGroupId, SocialId, FRemoveAccountFromPlayerGroupDelegate::CreateLambda([Promise](TPragmaResult<TArray<FString>> Result)
	{
		Promise->SetValue(MoveTemp(Result));
	}));
	return Promise->GetFuture();
}

void UPragmaAccountPartnerService::GetPlayerIdentities(const TArray<FString> PlayerIds, const FGetPlayerIdentitiesDelegate& OnComplete)
{
	const auto Request = FPragma_Account_GetPlayerIdentitiesPartnerV1Request {{ PlayerIds }};
	Raw().GetPlayerIdentitiesPartnerV1(
		Request,
		UPragmaAccountPartnerServiceRaw::FGetPlayerIdentitiesPartnerV1Delegate::CreateWeakLambda(this,
			[this, OnComplete](const TPragmaResult<FPragma_Account_GetPlayerIdentitiesPartnerV1Response> Result,
			const FPragmaMessageMetadata&)
			{
				if (Result.IsFailure())
				{
					OnComplete.ExecuteIfBound(TPragmaResult<PlayerIdentitiesResponse>::Failure(Result.Error()));
					return;
				}

				OnComplete.ExecuteIfBound(TPragmaResult<PlayerIdentitiesResponse>(
					PlayerIdentitiesResponse { Result.Payload().PlayerIdentities, Result.Payload().Failures }
				));
			}
		));
}

TFuture<TPragmaResult<PlayerIdentitiesResponse>> UPragmaAccountPartnerService::GetPlayerIdentities(const TArray<FString> PlayerIds)
{
	auto Promise = MakeShared<TPromise<TPragmaResult<PlayerIdentitiesResponse>>>();
	GetPlayerIdentities(PlayerIds, FGetPlayerIdentitiesDelegate::CreateLambda(
		[Promise](TPragmaResult<PlayerIdentitiesResponse> Result)
		{
			Promise->SetValue(MoveTemp(Result));
		}));
	return Promise->GetFuture();
}

void UPragmaAccountPartnerService::GetSocialIdentities(const TArray<FString> SocialIds, const FGetSocialIdentitiesDelegate& OnComplete)
{
	const auto Request = FPragma_Account_GetSocialIdentitiesPartnerV1Request {{ SocialIds }};
	Raw().GetSocialIdentitiesPartnerV1(
		Request,
		UPragmaAccountPartnerServiceRaw::FGetSocialIdentitiesPartnerV1Delegate::CreateWeakLambda(this,
			[this, OnComplete](const TPragmaResult<FPragma_Account_GetSocialIdentitiesPartnerV1Response> Result,
			const FPragmaMessageMetadata&)
			{
				if (Result.IsFailure())
				{
					OnComplete.ExecuteIfBound(TPragmaResult<SocialIdentitiesResponse>::Failure(Result.Error()));
					return;
				}

				OnComplete.ExecuteIfBound(TPragmaResult<SocialIdentitiesResponse>(
					SocialIdentitiesResponse { Result.Payload().SocialIdentities, Result.Payload().Failures }
				));
			}
		));
}

TFuture<TPragmaResult<SocialIdentitiesResponse>> UPragmaAccountPartnerService::GetSocialIdentities(const TArray<FString> SocialIds)
{
	auto Promise = MakeShared<TPromise<TPragmaResult<SocialIdentitiesResponse>>>();
	GetSocialIdentities(SocialIds, FGetSocialIdentitiesDelegate::CreateLambda(
		[Promise](TPragmaResult<SocialIdentitiesResponse> Result)
		{
			Promise->SetValue(MoveTemp(Result));
		}));
	return Promise->GetFuture();
}
