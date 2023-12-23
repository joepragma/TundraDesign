#include "Pragma/Api/Player/PragmaFriendApi.h"
#include "Services/Friend/FriendResponseHandler.h"
#include "PragmaSessionV2.h"

DEFINE_LOG_CATEGORY_STATIC(LogPragmaFriendsApi, Error, All);

#define FRIENDS_API_LOG(Verbosity, Format, ...) { PRAGMA_BASE_LOG(LogPragmaFriendsApi, Verbosity, Format, ##__VA_ARGS__); }

void UPragmaFriendApi::SetDependencies(UPragmaFriendServiceRaw* InFriendServiceRaw)
{
	FriendServiceRaw = InFriendServiceRaw;

	Session()->OnDegraded.AddUObject(this, &UPragmaFriendApi::ClearCache);
	Session()->OnDisconnected.AddUObject(this, &UPragmaFriendApi::ClearCache);
	Session()->OnReconnected.AddUObject(this, &UPragmaFriendApi::Reinitialize);
}

void UPragmaFriendApi::Initialize(const FOnCompleteDelegate& OnComplete)
{
	if (bIsInitialized)
	{
		OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
		return;
	}

	FetchFriendList(FOnCompleteDelegate::CreateLambda([this, OnComplete](const TPragmaResult<>& Result) {
		if (Result.IsSuccessful())
		{
			FriendServiceRaw->OnFriendListUpdateV1.AddUObject(this, &UPragmaFriendApi::HandleFriendListUpdate);
			FriendServiceRaw->OnFriendUpdateV1.AddUObject(this, &UPragmaFriendApi::HandleFriendUpdate);
		}

		OnComplete.ExecuteIfBound(Result);
	}));
}

TFuture<TPragmaResult<>> UPragmaFriendApi::Initialize()
{
	auto                                                                   Promise = MakeShared<TPromise<TPragmaResult<>>>();
	Initialize(FOnCompleteDelegate::CreateLambda([Promise](TPragmaResult<> Result) { Promise->SetValue(MoveTemp(Result)); }));
	return Promise->GetFuture();
}

void UPragmaFriendApi::ForceReset(const FOnCompleteDelegate& OnComplete)
{
	if (!bIsInitialized)
	{
		UE_LOG(LogPragmaFriendsApi, Warning, TEXT("ForceReset - FriendApi is not initialized"));
		OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(EPragma_SdkError::Friend_ApiNotInitialized));
		return;
	}

	FetchFriendList(OnComplete);
}

TFuture<TPragmaResult<>> UPragmaFriendApi::ForceReset()
{
	auto                                                                   Promise = MakeShared<TPromise<TPragmaResult<>>>();
	ForceReset(FOnCompleteDelegate::CreateLambda([Promise](TPragmaResult<> Result) { Promise->SetValue(MoveTemp(Result)); }));
	return Promise->GetFuture();
}

TSharedPtr<TMap<FString, FPragma_Friend_Friend>> UPragmaFriendApi::GetFriends() const
{
	if (!bIsInitialized)
	{
		return nullptr;
	}
	TSharedPtr<TMap<FString, FPragma_Friend_Friend>> RefMap = MakeShared<TMap<FString, FPragma_Friend_Friend>>();
	*RefMap = Friends;
	return RefMap;
}

TSharedPtr<TMap<FString, FPragma_Friend_FriendOverview>> UPragmaFriendApi::GetSentInvites() const
{
	if (!bIsInitialized)
	{
		return nullptr;
	}
	TSharedPtr<TMap<FString, FPragma_Friend_FriendOverview>> RefMap = MakeShared<TMap<FString, FPragma_Friend_FriendOverview>>();
	*RefMap = SentInvites;
	return RefMap;
}

TSharedPtr<TMap<FString, FPragma_Friend_FriendOverview>> UPragmaFriendApi::GetReceivedInvites() const
{
	if (!bIsInitialized)
	{
		return nullptr;
	}
	TSharedPtr<TMap<FString, FPragma_Friend_FriendOverview>> RefMap = MakeShared<TMap<FString, FPragma_Friend_FriendOverview>>();
	*RefMap = ReceivedInvites;
	return RefMap;
}

void UPragmaFriendApi::HandleFriendListUpdate(
	const FPragma_Friend_FriendListUpdateV1Notification Notification,
	const FPragmaMessageMetadata&)
{
	if (!CanProcessUpdates(Notification.Updates))
	{
		ForceReset();
		return;
	}

	CurrentVersion = Notification.Updates.Version;

	const auto SentInviteRefreshing = this->HandleSentInviteRemovedAndAddedFriendNotifications(Notification);
	const auto ReceivedInviteRefreshing = this->HandleReceivedInviteRemoved(Notification);

	const bool bShouldForceReset = SentInviteRefreshing.first || ReceivedInviteRefreshing.first;
	const bool bShouldBroadcastFriends = HandleFriendRemoved(Notification);
	const bool bShouldBroadcastReceivedInvites = HandleReceivedInvitesAdded(Notification) || ReceivedInviteRefreshing.second;
	const bool bShouldBroadcastSentInvites = HandleSentInvitesAdded(Notification) || SentInviteRefreshing.second;

	if (bShouldForceReset)
	{
		ForceReset();
	}
	else
	{
		if (bShouldBroadcastFriends)
		{
			OnFriendsListChanged.Broadcast(Friends);
		}
		if (bShouldBroadcastReceivedInvites)
		{
			OnReceivedInvitesChanged.Broadcast(ReceivedInvites);
		}
		if (bShouldBroadcastSentInvites)
		{
			OnSentInvitesChanged.Broadcast(SentInvites);
		}
	}
}

void UPragmaFriendApi::HandleFriendUpdate(const FPragma_Friend_FriendUpdateV1Notification Notification, const FPragmaMessageMetadata&)
{
	if (!Friends.Contains(Notification.SocialId))
	{
		UE_LOG(LogPragmaFriendsApi, Verbose, TEXT("Ignoring friend update for unknown friend social id `%s`."), *Notification.SocialId);
		return;
	}
	auto& Friend = Friends[Notification.SocialId];
	const auto Update = Notification.UpdatedPresence;
	const auto OldPresence = Friend.Presences.FindByPredicate([Update](const FPragma_Friend_Presence& Val) {
		return Val.GameShardId == Update.GameShardId;
	});
	
	if(OldPresence!=nullptr)
	{
		if (OldPresence->Version > Update.Version)
		{
			UE_LOG(LogPragmaFriendsApi, Verbose, TEXT("Ignoring out of date friend update."));
			return;
		}
		
		OldPresence->Version = Update.Version;
		OldPresence->BasicPresence = Update.BasicPresence;
	} else
	{
		Friend.Presences.Add(Update);
	}
	OnFriendUpdated.Broadcast(Friend);
	OnFriendsListChanged.Broadcast(Friends);
}

std::pair<bool, bool> UPragmaFriendApi::HandleSentInviteRemovedAndAddedFriendNotifications(const FPragma_Friend_FriendListUpdateV1Notification& Notification)
{
	bool bShouldForceRefresh = false;
	bool bShouldBroadcastSentInvites = false;
	for (auto& SentInviteRemoved : Notification.Updates.SentInvitesRemoved)
	{
		if (!SentInvites.Contains(SentInviteRemoved))
		{
			continue;
		}

		const bool bWasAccepted = Notification.Updates.FriendsAdded.FindByPredicate([&SentInviteRemoved](const FPragma_Friend_FriendOverview& Overview) {
			return Overview.SocialId == SentInviteRemoved;
		}) != nullptr;

		const auto FriendOverview = SentInvites[SentInviteRemoved];

		if (bWasAccepted)
		{
			OnFriendInviteAccepted.Broadcast(FriendOverview);
			UE_LOG(LogTemp, Log, TEXT("Called OnFriendInviteAccepted"));
			bShouldForceRefresh = true;
		}
		else
		{
			SentInvites.Remove(SentInviteRemoved);
			OnFriendInviteDeclined.Broadcast(FriendOverview);
			bShouldBroadcastSentInvites = true;
		}
	}

	return std::make_pair(bShouldForceRefresh, bShouldBroadcastSentInvites);
}

std::pair<bool, bool> UPragmaFriendApi::HandleReceivedInviteRemoved(const FPragma_Friend_FriendListUpdateV1Notification& Notification)
{
	bool bShouldForceRefresh = false;
	bool bShouldBroadcastReceivedInvites = false;
	for (auto& ReceivedInviteRemoved : Notification.Updates.ReceivedInvitesRemoved)
	{
		if (!ReceivedInvites.Contains(ReceivedInviteRemoved))
		{
			continue;
		}

		const bool bWasAccepted = Notification.Updates.FriendsAdded.FindByPredicate([&ReceivedInviteRemoved](const FPragma_Friend_FriendOverview& Overview) {
			return Overview.SocialId == ReceivedInviteRemoved;
		}) != nullptr;

		if (bWasAccepted)
		{
			bShouldForceRefresh = true;
		}
		else
		{
			ReceivedInvites.Remove(ReceivedInviteRemoved);
			bShouldBroadcastReceivedInvites = true;
		}
	}

	return std::make_pair(bShouldForceRefresh, bShouldBroadcastReceivedInvites);
}

bool UPragmaFriendApi::HandleFriendRemoved(const FPragma_Friend_FriendListUpdateV1Notification& Notification)
{
	bool bShouldBroadcast = false;
	for (auto& FriendRemoved : Notification.Updates.FriendsRemoved)
	{
		if (!Friends.Contains(FriendRemoved))
		{
			continue;
		}

		const auto Friend = Friends[FriendRemoved];

		Friends.Remove(FriendRemoved);
		OnRemovedAsFriend.Broadcast(Friend);
		bShouldBroadcast = true;
	}
	return bShouldBroadcast;
}

bool UPragmaFriendApi::HandleReceivedInvitesAdded(const FPragma_Friend_FriendListUpdateV1Notification& Notification)
{
	bool bShouldBroadcast = false;
	for (FPragma_Friend_FriendOverview InviteAdded : Notification.Updates.ReceivedInvitesAdded)
	{
		auto SocialId = InviteAdded.SocialId;

		if (ReceivedInvites.Contains(SocialId))
		{
			continue;
		}

		ReceivedInvites.Emplace(SocialId, MoveTemp(InviteAdded));
		OnFriendInviteReceived.Broadcast(ReceivedInvites[SocialId]);
		bShouldBroadcast = true;
	}
	return bShouldBroadcast;
}

bool UPragmaFriendApi::HandleSentInvitesAdded(const FPragma_Friend_FriendListUpdateV1Notification& Notification)
{
	bool bShouldBroadcast = false;
	for (FPragma_Friend_FriendOverview InviteAdded : Notification.Updates.SentInvitesAdded)
	{
		auto SocialId = InviteAdded.SocialId;

		if (SentInvites.Contains(SocialId))
		{
			continue;
		}

		SentInvites.Emplace(SocialId, MoveTemp(InviteAdded));
		bShouldBroadcast = true;
	}
	return bShouldBroadcast;
}

void UPragmaFriendApi::SendFriendInviteByDisplayName(const FPragma_Account_DisplayName& DisplayName, const FOnCompleteDelegate& OnComplete)
{
	if (!bIsInitialized)
	{
		UE_LOG(LogPragmaFriendsApi, Warning, TEXT("SendFriendInviteByDisplayName - FriendApi is not initialized"));
		OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(EPragma_SdkError::Friend_ApiNotInitialized));
		return;
	}

	FriendServiceRaw->SendFriendInviteV1(FPragma_Friend_SendFriendInviteV1Request{ DisplayName },
		UPragmaFriendServiceRaw::FSendFriendInviteV1Delegate::CreateWeakLambda(this,
			[this, OnComplete]
		(const TPragmaResult<FPragma_Friend_SendFriendInviteV1Response>& Result, const FPragmaMessageMetadata& _) {
				if (Result.IsFailure())
				{
					UE_LOG(LogPragmaFriendsApi, Warning, TEXT("SendFriendInviteByDisplayName - Error sending invite by display name"));
					OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(Result.ErrorCode()));
					return;
				}

				OnComplete.ExecuteIfBound(FFriendResponseHandler::HandleResponse(Result.Payload().Response));
			}
			));
}

TFuture<TPragmaResult<>> UPragmaFriendApi::SendFriendInviteByDisplayName(const FPragma_Account_DisplayName& DisplayName)
{
	auto                                                                                                   Promise = MakeShared<TPromise<TPragmaResult<>>>();
	SendFriendInviteByDisplayName(DisplayName, FOnCompleteDelegate::CreateLambda([Promise](TPragmaResult<> Result) { Promise->SetValue(MoveTemp(Result)); }));
	return Promise->GetFuture();
}

void UPragmaFriendApi::SendFriendInviteBySocialId(const FString& SocialId, const FOnCompleteDelegate& OnComplete)
{
	if (!bIsInitialized)
	{
		UE_LOG(LogPragmaFriendsApi, Warning, TEXT("SendFriendInviteBySocialId - FriendApi is not initialized"));
		OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(EPragma_SdkError::Friend_ApiNotInitialized));
		return;
	}

	FriendServiceRaw->SendFriendInviteBySocialIdV1(FPragma_Friend_SendFriendInviteBySocialIdV1Request{ SocialId },
		UPragmaFriendServiceRaw::FSendFriendInviteBySocialIdV1Delegate::CreateWeakLambda(this,
			[this, OnComplete]
		(const TPragmaResult<FPragma_Friend_SendFriendInviteBySocialIdV1Response>& Result, const FPragmaMessageMetadata& _) {
				if (Result.IsFailure())
				{
					UE_LOG(LogPragmaFriendsApi, Warning, TEXT("SendFriendInviteBySocialId - Error sending invite by social id"));
					OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(Result.ErrorCode()));
					return;
				}

				OnComplete.ExecuteIfBound(FFriendResponseHandler::HandleResponse(Result.Payload().Response));
			}
			));
}

TFuture<TPragmaResult<>> UPragmaFriendApi::SendFriendInviteBySocialId(const FString& SocialId)
{
	auto                                                                                             Promise = MakeShared<TPromise<TPragmaResult<>>>();
	SendFriendInviteBySocialId(SocialId, FOnCompleteDelegate::CreateLambda([Promise](TPragmaResult<> Result) { Promise->SetValue(MoveTemp(Result)); }));
	return Promise->GetFuture();
}

void UPragmaFriendApi::AcceptFriendInvite(const FString& InviterSocialId, const FOnCompleteDelegate& OnComplete)
{
	if (!bIsInitialized)
	{
		UE_LOG(LogPragmaFriendsApi, Warning, TEXT("AcceptFriendInvite - FriendApi is not initialized"));
		OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(EPragma_SdkError::Friend_ApiNotInitialized));
		return;
	}

	FriendServiceRaw->RespondToFriendInviteV1(FPragma_Friend_RespondToFriendInviteV1Request{ InviterSocialId, true },
		UPragmaFriendServiceRaw::FRespondToFriendInviteV1Delegate::CreateWeakLambda(this,
			[this, OnComplete]
		(const TPragmaResult<FPragma_Friend_RespondToFriendInviteV1Response>& Result, const FPragmaMessageMetadata& _) {
				if (Result.IsFailure())
				{
					UE_LOG(LogPragmaFriendsApi, Warning, TEXT("AcceptFriendInvite - Error accepting friend invite"));
					OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(Result.ErrorCode()));
					return;
				}

				OnComplete.ExecuteIfBound(FFriendResponseHandler::HandleResponse(Result.Payload().Response));
			}
			));
}

TFuture<TPragmaResult<>> UPragmaFriendApi::AcceptFriendInvite(const FString& InviterSocialId)
{
	auto                                                                                            Promise = MakeShared<TPromise<TPragmaResult<>>>();
	AcceptFriendInvite(InviterSocialId, FOnCompleteDelegate::CreateLambda([Promise](TPragmaResult<> Result) { Promise->SetValue(MoveTemp(Result)); }));
	return Promise->GetFuture();
}

void UPragmaFriendApi::DeclineFriendInvite(const FString& InviterSocialId, const FOnCompleteDelegate& OnComplete)
{
	if (!bIsInitialized)
	{
		UE_LOG(LogPragmaFriendsApi, Warning, TEXT("DeclineFriendInvite - FriendApi is not initialized"));
		OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(EPragma_SdkError::Friend_ApiNotInitialized));
		return;
	}

	FriendServiceRaw->RespondToFriendInviteV1(FPragma_Friend_RespondToFriendInviteV1Request{ InviterSocialId, false },
		UPragmaFriendServiceRaw::FRespondToFriendInviteV1Delegate::CreateWeakLambda(this,
			[this, OnComplete]
		(const TPragmaResult<FPragma_Friend_RespondToFriendInviteV1Response>& Result, const FPragmaMessageMetadata& _) {
				if (Result.IsFailure())
				{
					UE_LOG(LogPragmaFriendsApi, Warning, TEXT("DeclineFriendInvite - Error declining friend invite"));
					OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(Result.ErrorCode()));
					return;
				}

				OnComplete.ExecuteIfBound(FFriendResponseHandler::HandleResponse(Result.Payload().Response));
			}
			));
}

TFuture<TPragmaResult<>> UPragmaFriendApi::DeclineFriendInvite(const FString& InviterSocialId)
{
	auto                                                                                             Promise = MakeShared<TPromise<TPragmaResult<>>>();
	DeclineFriendInvite(InviterSocialId, FOnCompleteDelegate::CreateLambda([Promise](TPragmaResult<> Result) { Promise->SetValue(MoveTemp(Result)); }));
	return Promise->GetFuture();
}

void UPragmaFriendApi::RemoveFriend(const FString& SocialId, const FOnCompleteDelegate& OnComplete)
{
	if (!bIsInitialized)
	{
		UE_LOG(LogPragmaFriendsApi, Warning, TEXT("RemoveFriend - FriendApi is not initialized"));
		OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(EPragma_SdkError::Friend_ApiNotInitialized));
		return;
	}

	FriendServiceRaw->RemoveFriendV1(FPragma_Friend_RemoveFriendV1Request{ SocialId },
		UPragmaFriendServiceRaw::FRemoveFriendV1Delegate::CreateWeakLambda(this,
			[this, OnComplete]
		(const TPragmaResult<FPragma_Friend_RemoveFriendV1Response>& Result, const FPragmaMessageMetadata& _) {
				if (Result.IsFailure())
				{
					UE_LOG(LogPragmaFriendsApi, Warning, TEXT("RemoveFriend - Error removing friend"));
					OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(Result.ErrorCode()));
					return;
				}

				OnComplete.ExecuteIfBound(FFriendResponseHandler::HandleResponse(Result.Payload().Response));
			}
			));
}

TFuture<TPragmaResult<>> UPragmaFriendApi::RemoveFriend(const FString& SocialId)
{
	auto                                                                               Promise = MakeShared<TPromise<TPragmaResult<>>>();
	RemoveFriend(SocialId, FOnCompleteDelegate::CreateLambda([Promise](TPragmaResult<> Result) { Promise->SetValue(MoveTemp(Result)); }));
	return Promise->GetFuture();
}

bool UPragmaFriendApi::CanProcessUpdates(const FPragma_Friend_FriendListUpdates& FriendListUpdates)
{
	return CurrentVersion.IsSet() && FriendListUpdates.Version - CurrentVersion.GetValue() == 1;
}

void UPragmaFriendApi::FetchFriendList(const FOnCompleteDelegate& OnComplete)
{
	if (!Session()->IsInitialized())
	{
		UE_LOG(LogPragmaFriendsApi, Warning, TEXT("InitializeCache - Session is not initialized"));
		OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(EPragma_SdkError::Friend_SessionNotInitialized));
		return;
	}

	FriendServiceRaw->GetFriendListV1(FPragma_Friend_GetFriendListV1Request{ Session()->GameShardId() },
		UPragmaFriendServiceRaw::FGetFriendListV1Delegate::CreateWeakLambda(this,
			[this, OnComplete]
		(TPragmaResult<FPragma_Friend_GetFriendListV1Response> Result, const FPragmaMessageMetadata& _) {
				if (Result.IsFailure())
				{
					UE_LOG(LogPragmaFriendsApi, Warning, TEXT("InitializeCache - Error fetching cache"));
					OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(Result.ErrorCode()));
					return;
				}

				auto FriendsList = Result.Payload().FriendList;

				Friends.Empty();
				SentInvites.Empty();
				ReceivedInvites.Empty();

				for (auto& Friend : FriendsList.Friends)
				{
					auto SocialId = Friend.SocialId;

					Friends.Emplace(SocialId, MoveTemp(Friend));
				}
				for (auto& SentInvite : FriendsList.SentInvites)
				{
					auto SocialId = SentInvite.SocialId;
					SentInvites.Emplace(SocialId, MoveTemp(SentInvite));
				}
				for (auto& ReceivedInvite : FriendsList.ReceivedInvites)
				{
					auto SocialId = ReceivedInvite.SocialId;
					ReceivedInvites.Emplace(SocialId, MoveTemp(ReceivedInvite));
				}

				bIsInitialized = true;
				CurrentVersion = Result.Payload().FriendList.Version;

				OnFriendsListChanged.Broadcast(Friends);
				OnSentInvitesChanged.Broadcast(SentInvites);
				OnReceivedInvitesChanged.Broadcast(ReceivedInvites);

				OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
			}
			)
		);
}

void UPragmaFriendApi::Reinitialize()
{
	UE_LOG(LogPragmaFriendsApi, Verbose, TEXT("Reinitialize - the Player Sesssion has reconnected. Reinitializing FriendApi."));

	Initialize();
}

void UPragmaFriendApi::ClearCache()
{
	UE_LOG(LogPragmaFriendsApi, Verbose, TEXT("ClearCache - the Player Session has been degraded or disconnected."));

	Friends.Empty();
	SentInvites.Empty();
	ReceivedInvites.Empty();
	bIsInitialized = false;
	CurrentVersion = 0;
}

TFuture<TPragmaResult<>> UPragmaFriendApi::Promisify(const TUniqueFunction<void(FOnCompleteDelegate)>& Func) const
{
	auto                                                             Promise = MakeShared<TPromise<TPragmaResult<>>>();
	Func(FOnCompleteDelegate::CreateLambda([Promise](TPragmaResult<> Result) { Promise->SetValue(MoveTemp(Result)); }));
	return Promise->GetFuture();
}
