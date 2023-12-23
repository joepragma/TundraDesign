#pragma once
#include <utility>

#include "PragmaPlayerApi.h"
#include "Dto/PragmaFriendServiceRaw.h"
#include "PragmaFriendApi.generated.h"

UCLASS(BlueprintType, Transient, Config=Game, Category=Pragma)
class PRAGMASDK_API UPragmaFriendApi final : public UPragmaPlayerApi
{
	GENERATED_BODY()

	using FFriendList = TMap<FString, FPragma_Friend_Friend>;
	using FSentInvites = TMap<FString, FPragma_Friend_FriendOverview>;
	using FReceivedInvites = TMap<FString, FPragma_Friend_FriendOverview>;

public:
	void SetDependencies(UPragmaFriendServiceRaw* InFriendServiceRaw);

	////////////////////////////////////////////////////////////////
	// Events
	////////////////////////////////////////////////////////////////

	// Fired when player's friends have changed. Good event to bind to when (re)rendering a list of friends.
	DECLARE_EVENT_OneParam(UPragmaFriendApi, FFriendsListChangedEvent, FFriendList);

	FFriendsListChangedEvent OnFriendsListChanged;

	// Fired when an individual player's friend has changed. Good event to bind to when notifying a player of friend presence updates.
	DECLARE_EVENT_OneParam(UPragmaFriendApi, FFriendChangedEvent, FPragma_Friend_Friend /* Friend */);
	FFriendChangedEvent OnFriendUpdated;

	// Fired when player's sent invites have changed. Good event to bind to for (re)rendering a list of sent invites.
	DECLARE_EVENT_OneParam(UPragmaFriendApi, FSentInvitesChangedEvent, FSentInvites);

	FSentInvitesChangedEvent OnSentInvitesChanged;

	// Fired when player's received invites have changed. Good event to bind to for (re)rendering a list of received invites.
	DECLARE_EVENT_OneParam(UPragmaFriendApi, FReceivedInvitesChangedEvent, FReceivedInvites);

	FReceivedInvitesChangedEvent OnReceivedInvitesChanged;

	// Fired when player receives a single invite. Good event to bind to when notifying a player of an invite.
	DECLARE_EVENT_OneParam(UPragmaFriendApi, FFriendInviteReceivedEvent, FPragma_Friend_FriendOverview);

	FFriendInviteReceivedEvent OnFriendInviteReceived;

	// Fired when a player has one of their invites accepted by another. Good event to bind to when notifying a player of an accepted invite. 
	DECLARE_EVENT_OneParam(UPragmaFriendApi, FFriendInviteAcceptedEvent, FPragma_Friend_FriendOverview);

	FFriendInviteAcceptedEvent OnFriendInviteAccepted;

	// Fired when a player has one of their invites declined. Good event to bind to when notifying a player of a declined invite.
	DECLARE_EVENT_OneParam(UPragmaFriendApi, FFriendInviteDeclinedEvent, FPragma_Friend_FriendOverview);

	FFriendInviteDeclinedEvent OnFriendInviteDeclined;

	// Fired when a player has been removed by another as their friend. Good event to bind to when notifying a player that a friend removal has occured.
	DECLARE_EVENT_OneParam(UPragmaFriendApi, FRemovedAsFriendEvent, FPragma_Friend_Friend);

	FRemovedAsFriendEvent OnRemovedAsFriend;

	////////////////////////////////////////////////////////////////
	// Actions
	////////////////////////////////////////////////////////////////

	DECLARE_DELEGATE_OneParam(FOnCompleteDelegate, TPragmaResult<> /* Result */);

	// Used to initialize the PragmaFriendApi. All other calls will fail or return nullptr until Initialize has been called.
	// Will do an initial fetch on the friends list. Will error if the Player Session has not been initialized yet.
	void                     Initialize(const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> Initialize();

	// Can be called to reset the PragmaFriendApi if inconsistencies appear in friends or invite lists.
	void                     ForceReset(const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> ForceReset();

	// Gets a map of friends. Returns a nullptr if the API has not been initialized. Map is keyed by Social id.
	TSharedPtr<TMap<FString, FPragma_Friend_Friend>> GetFriends() const;

	// Gets a map of sent invites. Returns a nullptr if the API has not been initialized. Map is keyed by Social id.
	TSharedPtr<TMap<FString, FPragma_Friend_FriendOverview>> GetSentInvites() const;

	// Gets a map of received invites. Returns a nullptr if the API has not been initialized. Map is keyed by Social id.
	TSharedPtr<TMap<FString, FPragma_Friend_FriendOverview>> GetReceivedInvites() const;

	// Sends a friend invite to another player using their display name.
	void                     SendFriendInviteByDisplayName(const FPragma_Account_DisplayName& DisplayName, const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> SendFriendInviteByDisplayName(const FPragma_Account_DisplayName& DisplayName);

	// Sends a friend invite to another player using their Social Id.
	void                     SendFriendInviteBySocialId(const FString& SocialId, const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> SendFriendInviteBySocialId(const FString& SocialId);

	// Accepts a friend invite.
	void                     AcceptFriendInvite(const FString& InviterSocialId, const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> AcceptFriendInvite(const FString& InviterSocialId);

	// Declines a friend invite.
	void                     DeclineFriendInvite(const FString& InviterSocialId, const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> DeclineFriendInvite(const FString& InviterSocialId);

	// Removes a friend.
	void                     RemoveFriend(const FString& SocialId, const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> RemoveFriend(const FString& SocialId);

private:
	UPROPERTY()
	UPragmaFriendServiceRaw* FriendServiceRaw = nullptr;

	TFuture<TPragmaResult<>> Promisify(const TUniqueFunction<void(FOnCompleteDelegate)>& Func) const;

	bool                                         bIsInitialized = false;
	TOptional<int>                               CurrentVersion = 0;
	TMap<FString, FPragma_Friend_Friend>         Friends{};
	TMap<FString, FPragma_Friend_FriendOverview> SentInvites{};
	TMap<FString, FPragma_Friend_FriendOverview> ReceivedInvites{};

	////////////////////////////////////////////////////////////////
	// Notification Handlers
	////////////////////////////////////////////////////////////////

	void HandleFriendListUpdate(
		const FPragma_Friend_FriendListUpdateV1Notification Notification,
		const FPragmaMessageMetadata&);

	void HandleFriendUpdate(
		const FPragma_Friend_FriendUpdateV1Notification Notification,
		const FPragmaMessageMetadata&);

	////////////////////////////////////////////////////////////////
	// Private Functions
	////////////////////////////////////////////////////////////////
	std::pair<bool, bool> HandleSentInviteRemovedAndAddedFriendNotifications(const FPragma_Friend_FriendListUpdateV1Notification& Notification);
	std::pair<bool, bool> HandleReceivedInviteRemoved(const FPragma_Friend_FriendListUpdateV1Notification& Notification);
	bool                  HandleFriendRemoved(const FPragma_Friend_FriendListUpdateV1Notification& Notification);
	bool                  HandleReceivedInvitesAdded(const FPragma_Friend_FriendListUpdateV1Notification& Notification);
	bool                  HandleSentInvitesAdded(const FPragma_Friend_FriendListUpdateV1Notification& Notification);
	bool                  CanProcessUpdates(const FPragma_Friend_FriendListUpdates& FriendListUpdates);
	void                  FetchFriendList(const FOnCompleteDelegate& OnComplete);
	void                  Reinitialize();
	void                  ClearCache();
};
