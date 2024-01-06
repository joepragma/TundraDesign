#pragma once

#include "Services/PragmaGameDataService.h"
#include "Dto/PragmaMatchmakingServiceRaw.h"
#include "Dto/PragmaGameInstanceServiceRaw.h"
#include "PragmaPlayerApi.h"
#include "PragmaSdkConfig.h"
#include "Services/Party/PragmaPartyProxy.h"
#include "Services/PragmaSessionService.h"
#include "Dto/PragmaSessionCommonDto.h"
#include "PragmaError.h"
#include "Services/Party/PragmaPartyInvite.h"
#include "PragmaResult.h"
#include "Async/Future.h"
#include "Services/Party/PragmaPartyPlayer.h"

#include "PragmaGameLoopApi.generated.h"

class UPragmaPartyServiceRaw;
class UPragmaPartyPlayer;
class UPragmaParty;
struct FPragmaMessageMetadata;

// ************************************************************
// *** THIS FILE WAS STUBBED FOR THIS TUNDRA DESIGN PROJECT ***
// ************************************************************

UCLASS(BlueprintType, Transient, Config=Game, Category=Pragma)
class PRAGMASDK_API UPragmaGameLoopApi final : public UPragmaPlayerApi
{
	GENERATED_BODY()

public:
	void SetDependencies(
		UPragmaPartyServiceRaw* InPartyRaw,
		UPragmaMatchmakingServiceRaw* InMatchmakingRaw,
		UPragmaGameInstanceServiceRaw* InGameInstanceRaw,
		UPragmaSessionService* InSessions,
		UPragmaSdkConfig* InConfig);

	void SetDependencies(
		UPragmaPartyServiceRaw* InPartyRaw,
		UPragmaMatchmakingServiceRaw* InMatchmakingRaw,
		UPragmaGameInstanceServiceRaw* InGameInstanceRaw,
		UPragmaSessionService* InSessions,
		UPragmaPartyProxy* InPartyProxy,
		UPragmaSdkConfig* InConfig);

	////////////////////////////////////////////////////////////////
	// Events
	////////////////////////////////////////////////////////////////

	// Fired any time the player joins a party.
	DECLARE_EVENT_OneParam(UPragmaGameLoopApi, FPartyEvent, const UPragmaParty* /* Party */);
	FPartyEvent OnJoinedParty;

	// Fired any time _any_ party information changes, including when it is created/destroyed.
	// All other On* events will fire in addition to this one, be careful to not double-handle!
	FPartyEvent OnPartyChanged;

	// Fired when a new invite code is made for the party.
	DECLARE_EVENT_OneParam(UPragmaGameLoopApi, FPartyInviteCodeEvent, FString /* InviteCode */);
	FPartyInviteCodeEvent OnPartyInviteCodeChanged;

	// Fired when the public extension party info changes.
	DECLARE_EVENT_OneParam(UPragmaGameLoopApi, FExtBroadcastPartyEvent,
		const FPragma_Party_ExtBroadcastParty& /* ExtBroadcastParty */);
	FExtBroadcastPartyEvent OnExtBroadcastPartyChanged;

	// Fired when PreferredGameServerZones change on a party.
	DECLARE_EVENT_OneParam(UPragmaGameLoopApi, FPartyPreferredGameServerZonesEvent,
		const TArray<FString>& /* PreferredGameServerZones */);
	FPartyPreferredGameServerZonesEvent OnPartyPreferredGameServerZonesChanged;

	// Fired any time the player leaves a party intentionally.
	DECLARE_EVENT(UPragmaGameLoopApi, FLeftPartyEvent);
	FLeftPartyEvent OnLeftParty;
	
	// Fired any time the player is removed from a party outside of their control.
	DECLARE_EVENT_OneParam(UPragmaGameLoopApi, FRemovedFromPartyEvent, EPragma_Party_RemovalReason /* RemovalReason */);
	FRemovedFromPartyEvent OnRemovedFromParty;

	// Fired when one or more players join or leave the party. Will fire if the number stays the same (e.g. +1, -1 in one update).
	DECLARE_EVENT_OneParam(UPragmaGameLoopApi, FPlayerListEvent, const TArray<const UPragmaPartyPlayer*>& /* Players */);
	FPlayerListEvent OnPartyPlayersChanged;

	// Fired when a new player joins the party.
	DECLARE_EVENT_OneParam(UPragmaGameLoopApi, FPlayerEvent, const UPragmaPartyPlayer* /* Player */);
	FPlayerEvent OnPlayerJoinedParty;

	// Fired when anything about a player changes (e.g. ready state, ext info).
	FPlayerEvent OnPartyPlayerDataChanged;

	// Fired when a player leaves the party.
	FPlayerEvent OnPlayerLeftParty;

	// Fired when the player's private selections changes.
	DECLARE_EVENT_OneParam(UPragmaGameLoopApi, FExtPrivatePlayerEvent, const FPragma_Party_ExtPrivatePlayer& /* ExtPrivatePlayer */);
	FExtPrivatePlayerEvent OnExtPrivatePlayerChanged;

	// Fired any time the invites list is changed.
	DECLARE_EVENT_OneParam(UPragmaGameLoopApi, FInviteListEvent, const TArray<FPragmaPartyInvite>& /* Invites */);
	FInviteListEvent OnPartyInvitesChanged;

	// Fired when a party invite is received from another player.
	DECLARE_EVENT_OneParam(UPragmaGameLoopApi, FInviteEvent, const FPragmaPartyInvite& /* Invite */);
	FInviteEvent OnPartyInviteReceived;

    // Fired when a party invite is revoked because a newer invite from the same inviter was received.
	DECLARE_EVENT_OneParam(UPragmaGameLoopApi, FPartyInviteIdEvent, FString /* InviteId */);
	FPartyInviteIdEvent OnPartyInviteRevoked;

	// Fired (for inviter only) when an invitee accepts the invite.
	FPartyInviteIdEvent OnPartyInviteAccepted;

	// Fired (for inviter only) when an invitee declines the invite.
	FPartyInviteIdEvent OnPartyInviteDeclined;

	// Fired any time the client needs to update their GameClientVersion.
	DECLARE_EVENT(UPragmaGameLoopApi, FUpdateClientVersionEvent);
	FUpdateClientVersionEvent OnGameClientVersionUpdateRequired;

	// Fired when a compatible GameServerVersion could not be found for the party.
	DECLARE_EVENT(UPragmaGameLoopApi, FPartyClientVersionMismatchEvent);
	FPartyClientVersionMismatchEvent OnPartyClientVersionMismatch;

	// Fired when the party has entered matchmaking.
	DECLARE_EVENT(UPragmaGameLoopApi, FEnteredMatchmakingEvent);
	FEnteredMatchmakingEvent OnEnteredMatchmaking;

	// Fired when the party has left matchmaking.
	DECLARE_EVENT(UPragmaGameLoopApi, FLeftMatchmakingEvent);
	FLeftMatchmakingEvent OnLeftMatchmaking;

	// Fired when there is a failure in matchmaking.
	DECLARE_EVENT_OneParam(UPragmaGameLoopApi, FMatchmakingFailedEvent,
		EPragma_Matchmaking_MatchmakingFailureReason /* FailureReason */);
	FMatchmakingFailedEvent OnMatchmakingFailed;

	// Fired when the player has been added to a game. Will be followed with OnHostConnectionDetailsReceived when the game server is ready.
	DECLARE_EVENT_TwoParams(UPragmaGameLoopApi, FAddedToGameEvent, FString /* GameInstanceId */,
		FPragma_GameInstance_ExtAddedToGame /* Ext */);
	FAddedToGameEvent OnAddedToGame;

	// Fired when the game instance id is changed in the Party.
	DECLARE_EVENT_OneParam(UPragmaGameLoopApi, FGameInstanceIdEvent, FString /* GameInstanceId */);
	FGameInstanceIdEvent OnGameInstanceIdChanged;

	// Fired when game instance allocation fails after matchmaking succeeds.
	DECLARE_EVENT(UPragmaGameLoopApi, FFailedToAllocateGameInstanceEvent);
	FFailedToAllocateGameInstanceEvent OnFailedToAllocateGameInstance;

	// Fired when the game client has received connection details to a game host.
	DECLARE_EVENT_OneParam(UPragmaGameLoopApi, FHostConnectionDetailsEvent,
		FPragma_GameInstance_HostConnectionDetails /* HostConnectionDetails */);
	FHostConnectionDetailsEvent OnHostConnectionDetailsReceived;

	// Fired when reconnecting to the platform after disconnecting from a game instance.
	DECLARE_EVENT_OneParam(UPragmaGameLoopApi, FGameInstanceReconnectEvent,
		FPragma_GameInstance_HostConnectionDetails /* HostConnectionDetails */);
	FGameInstanceReconnectEvent OnGameInstanceReconnect;

	// Fired when the game server has removed the player from the game.
	DECLARE_EVENT_TwoParams(UPragmaGameLoopApi, FRemovedFromGameEvent, FString /* GameInstanceId */,
		FPragma_GameInstance_ExtRemovedFromGame /* ExtRemovedFromGame */);
	FRemovedFromGameEvent OnRemovedFromGame;

	// Fired when a game has ended.
	DECLARE_EVENT_TwoParams(UPragmaGameLoopApi, FGameEndedEvent, FString /* GameInstanceId */,
		FPragma_GameInstance_ExtGameEnded /* Ext */);
	FGameEndedEvent OnGameEnded;

	// Fired when a game instance is terminated due to unhealthy reasons.
	DECLARE_EVENT_TwoParams(UPragmaGameLoopApi, FGameInstanceTerminatedEvent, FString /* GameInstanceId */,
		EPragma_GameInstance_GameInstanceTerminationReason /* TerminationReason */);
	FGameInstanceTerminatedEvent OnGameInstanceTerminated;

	////////////////////////////////////////////////////////////////
	// Actions
	////////////////////////////////////////////////////////////////

	DECLARE_DELEGATE_OneParam(FOnCompleteDelegate, TPragmaResult<> /* Result */);
	
	//
	// Create a new party.
	// - Available when not in a party.
	//
	void CreateParty(
		const FPragma_Party_ExtCreateRequest& ExtCreateRequest,
		const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
		const TArray<FString>& PreferredGameServerZones,
		const TMap<FString, int32>& GameServerZoneToPing,
		const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> CreateParty(
		const FPragma_Party_ExtCreateRequest& ExtCreateRequest,
		const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
		const TArray<FString>& PreferredGameServerZones,
		const TMap<FString, int32>& GameServerZoneToPing);
	void CreateParty(
		const FPragma_Party_ExtCreateRequest& ExtCreateRequest,
		const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
		const TArray<FString>& PreferredGameServerZones,
		const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> CreateParty(
		const FPragma_Party_ExtCreateRequest& ExtCreateRequest,
		const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
		const TArray<FString>& PreferredGameServerZones);
	void CreateParty(
		const FPragma_Party_ExtCreateRequest& ExtCreateRequest,
		const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
		const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> CreateParty(
		const FPragma_Party_ExtCreateRequest& ExtCreateRequest,
		const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest);

	UFUNCTION(BlueprintPure, Category=Pragma)
	bool CanCreateParty() const;
	bool CanCreateParty(FPragmaError& OutError) const;

	//
	// Invite another player to your party.
	// - On success, returns the invite id, usable to track whether a specific invite was accepted or rejected
	//
	DECLARE_DELEGATE_OneParam(FOnInviteSentDelegate, TPragmaResult<FString> /* Invite Id */);
	void SendPartyInvite(const FString& PlayerId, const FOnInviteSentDelegate& OnComplete);
	TFuture<TPragmaResult<FString>> SendPartyInvite(const FString& PlayerId);

	UFUNCTION(BlueprintPure, Category=Pragma)
	bool CanSendPartyInvite() const;
	bool CanSendPartyInvite(FPragmaError& OutError) const;

	//
	// Accept an invite to another player's party.
	// - Available when not in a party.
	//
	void RespondToPartyInvite(
		const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
		const FString& InviteId,
		const bool Accepted,
		const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> RespondToPartyInvite(
		const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
		const FString& InviteId,
		const bool Accepted);
	void RespondToPartyInvite(
		const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
		const FString& InviteId,
		const bool Accepted,
		const TMap<FString, int>& GameServerZoneToPing,
		const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> RespondToPartyInvite(
		const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
		const FString& InviteId,
		const bool Accepted,
		const TMap<FString, int>& GameServerZoneToPing);

	UFUNCTION(BlueprintPure, Category=Pragma)
	bool CanAcceptPartyInvite() const;
	bool CanAcceptPartyInvite(FPragmaError& OutError) const;
	
	//
	// Join a party with an invite code
	// - Available when not in a party.
	//
	void JoinPartyWithInviteCode(
		const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
		const FString& InviteCode,
		const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> JoinPartyWithInviteCode(
		const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
		const FString& InviteCode);
	void JoinPartyWithInviteCode(
		const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
		const FString& InviteCode,
		const TMap<FString, int>& GameServerZoneToPing,
		const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> JoinPartyWithInviteCode(
		const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
		const FString& InviteCode,
		const TMap<FString, int>& GameServerZoneToPing);
	
	UFUNCTION(BlueprintPure, Category=Pragma)
	bool CanJoinParty() const;
	bool CanJoinParty(FPragmaError& OutError) const;

	//
	// Join a party with a party id
	// - Available when not in a party.
	//
	void JoinPartyWithId(
		const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
		const FString& PartyId,
		const FOnCompleteDelegate& Delegate);
	TFuture<TPragmaResult<>> JoinPartyWithId(
		const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
		const FString& PartyId);
	void JoinPartyWithId(
		const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
		const FString& PartyId,
		const TMap<FString, int>& GameServerZoneToPing,
		const FOnCompleteDelegate& Delegate);
	TFuture<TPragmaResult<>> JoinPartyWithId(
		const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
		const FString& PartyId,
		const TMap<FString, int>& GameServerZoneToPing);
	
	//
	// Leave current party.
	// - Available when in party.
	//
	void LeaveParty(const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> LeaveParty();
	bool CanLeaveParty() const;
	bool CanLeaveParty(FPragmaError& OutError) const;

	//
	// Assigns a party leader.
	// - Available when in party.
	// - Party leader only.
	//
	void AssignPartyLeader(const FString& PlayerId, const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> AssignPartyLeader(const FString& PlayerId);
	bool CanAssignPartyLeader(const FString& PlayerId) const;
	bool CanAssignPartyLeader(const FString& PlayerId, FPragmaError& OutError) const;

	//
	// Kick player from party.
	// - Available when in party.
	// - Party leader only.
	//
	void KickPlayerFromParty(const FString& PlayerId, const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> KickPlayerFromParty(const FString& PlayerId);
	
	UFUNCTION(BlueprintPure, Category=Pragma)
	bool CanKickPlayerFromParty(const FString& PlayerId) const;
	bool CanKickPlayerFromParty(const FString& PlayerId, FPragmaError& OutError) const;
	
	//
	// Update selections for the player.
	// - Available when in party.
	//
	void UpdatePartyPlayer(
		const FPragma_Party_ExtUpdatePartyPlayerRequest& RequestExt,
		const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> UpdatePartyPlayer(const FPragma_Party_ExtUpdatePartyPlayerRequest& RequestExt);

	UFUNCTION(BlueprintPure, Category=Pragma)
	bool CanUpdatePartyPlayer() const;
	bool CanUpdatePartyPlayer(FPragmaError& OutError) const;
	
	//
	// Update party information for the party.
	// - Available when in party.
	//
	void UpdateParty(
		const FPragma_Party_ExtUpdatePartyRequest& RequestExt,
		const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> UpdateParty(const FPragma_Party_ExtUpdatePartyRequest& RequestExt);

	UFUNCTION(BlueprintPure, Category=Pragma)
	bool CanUpdateParty() const;
	bool CanUpdateParty(FPragmaError& OutError) const;

	//
	// Set PreferredGameServerZones information for the party.
	// - Available when in party for the Party Leader only
	//
	void SetPartyPreferredGameServerZones(TArray<FString> GameServerZones, const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> SetPartyPreferredGameServerZones(TArray<FString> GameServerZones);

	UFUNCTION(BlueprintPure, Category=Pragma)
	bool CanSetPartyPreferredGameServerZones() const;
	bool CanSetPartyPreferredGameServerZones(FPragmaError& OutError) const;
	
	//
	// Set the game server zone to ping map.
	// - Available when in party.
	//
	void SetPartyPlayerGameServerZoneToPing(
		const TMap<FString, int32>& GameServerZoneToPing,
		const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> SetPartyPlayerGameServerZoneToPing(const TMap<FString, int32>& GameServerZoneToPing);

	UFUNCTION(BlueprintPure, Category=Pragma)
	bool CanSetPartyPlayerGameServerZoneToPing() const;
	bool CanSetPartyPlayerGameServerZoneToPing(FPragmaError& OutError) const;
	
	//
	// Set Ready state for the player.
	// - Available when in party.
	//
	void SetPartyPlayerReady(bool bIsReady, const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> SetPartyPlayerReady(bool bIsReady);

	UFUNCTION(BlueprintPure, Category=Pragma)
	bool CanSetPartyPlayerReady() const;
	bool CanSetPartyPlayerReady(FPragmaError& OutError) const;

	//
	// Enter matchmaking.
	// - Available when in party.
	// - Party leader only.
	// - All players must be ready.
	//
	void EnterMatchmaking(const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> EnterMatchmaking();

	UFUNCTION(BlueprintPure, Category=Pragma)
	bool CanEnterMatchmaking() const;
	bool CanEnterMatchmaking(FPragmaError& OutError) const;

	//
	// Leave matchmaking.
	// - Available while in matchmaking.
	//
	void LeaveMatchmaking(const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> LeaveMatchmaking();

	UFUNCTION(BlueprintPure, Category=Pragma)
	bool CanLeaveMatchmaking() const;
	bool CanLeaveMatchmaking(FPragmaError& OutError) const;

	// 
	// Get Matchmaking Queue Info
	// - Available at all times.
	// - Requires server-side configuration to enable
	//
	DECLARE_DELEGATE_OneParam(FMatchmakingQueueInfoDelegate,
		TPragmaResult<TArray<FPragma_Matchmaking_QueueInfoV2>> /* MatchmakingQueueInfo */);
	void GetMatchmakingInfo(const FMatchmakingQueueInfoDelegate& MatchmakingQueueInfoDelegate);

	//
	// Decline Reconnect
	// - Available when the player has a game instance ID
	// - Requires server-side configuration to enable
	//
	void DeclineReconnect(const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> DeclineReconnect();

	UFUNCTION(BlueprintPure, Category=Pragma)
	bool CanDeclineReconnect() const;
	bool CanDeclineReconnect(FPragmaError& OutError) const;

	//
	// Get Host Connection Details
	// - Available while player has a game instance ID.
	//
	DECLARE_DELEGATE_OneParam(FHostConnectionDetailsDelegate,
		TPragmaResult<FPragma_GameInstance_HostConnectionDetails> /* Result */);
	void GetHostConnectionDetails(const FHostConnectionDetailsDelegate& OnComplete);
	TFuture<TPragmaResult<FPragma_GameInstance_HostConnectionDetails>> GetHostConnectionDetails();

	UFUNCTION(BlueprintPure, Category=Pragma)
	bool CanGetMatchConnectionDetails() const;
	bool CanGetMatchConnectionDetails(FPragmaError& OutError) const;

	////////////////////////////////////////////////////////////////
	// Helper Functions
	////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintPure, Category=Pragma)
	bool HasParty() const;

	bool SessionNotInParty() const;

	UFUNCTION(BlueprintPure, Category=Pragma)
	bool IsLeaderOfParty(FString PlayerId) const;

	UFUNCTION(BlueprintPure, Category=Pragma)
	bool IsInParty(FPragmaError& OutError) const;

	const UPragmaParty* GetParty() const { return PartyProxy->GetParty(); }
	const UPragmaPartyPlayer* GetLocalPartyPlayer() const;

	const TArray<FPragmaPartyInvite>& GetPendingPartyInvites() const { return PendingInvites; }
	TOptional<FPragmaPartyInvite> GetPartyInviteByInviteId(const FString& InviteId) const;
	TOptional<FPragmaPartyInvite> GetPartyInviteByInviterId(const FString& InviterId) const;

	const FString* GetGameInstanceId() const
	{
		return SessionService->GetAttribute(EPragma_GameSessionAttribute::GAME_INSTANCE_ID);
	}

	////////////////////////////////////////////////////////////////
	// Public Stubs
	////////////////////////////////////////////////////////////////

	void StubbedTriggerInviteAccepted(const FString& InviteId, const FString& PlayerId, const FString& SocialId, const FString& DisplayName, const FString& Discriminator) const;
	void StubbedTriggerPlayerLeftParty(const FString& PlayerId) const;
	void StubbedTriggerRemovedFromParty(const FString& PartyId, const EPragma_Party_RemovalReason& RemovalReason) const;

private:
	UPROPERTY()
	UPragmaPartyServiceRaw* PartyServiceRaw = nullptr;
	UPROPERTY()
	UPragmaMatchmakingServiceRaw* MatchmakingServiceRaw = nullptr;
	UPROPERTY()
	UPragmaGameInstanceServiceRaw* GameInstanceServiceRaw = nullptr;
	UPROPERTY()
	UPragmaSessionService* SessionService = nullptr;
	UPROPERTY()
	UPragmaGameDataServiceRaw* GameDataServiceRaw = nullptr;
	UPROPERTY()
	UPragmaSdkConfig* SdkConfig = nullptr;
	UPROPERTY(Transient)
	UPragmaPartyProxy* PartyProxy = nullptr;

	TArray<FPragmaPartyInvite> PendingInvites;

	////////////////////////////////////////////////////////////////
	// Private Stubs
	////////////////////////////////////////////////////////////////

	FPragma_Party_BroadcastParty StubbedConvertToBroadcastParty(const UPragmaParty* PragmaParty) const;
	FPragma_Party_BroadcastPartyMember StubbedCurrentPlayer(const bool IsLeader) const;
	FPragma_Party_BroadcastPartyMember StubbedRandomPlayer(const bool IsLeader) const;
	void StubbedCreateParty(const FPragma_Party_ExtCreateRequest& ExtCreateRequest, const FOnCompleteDelegate& OnComplete) const;
	void StubbedLeaveParty(const FOnCompleteDelegate& OnComplete) const;
	void StubbedSendPartyInvite(const FOnInviteSentDelegate& OnInviteSent) const;
	void StubbedJoinPartyWithInviteCode(const FString& InviteCode, const FOnCompleteDelegate& OnComplete) const;
	
	////////////////////////////////////////////////////////////////
	// Notification Handlers
	////////////////////////////////////////////////////////////////

	void HandleInviteReceived(
		FPragma_Party_InviteReceivedV1Notification Notification,
		const FPragmaMessageMetadata& Metadata);
	void HandleInviteResponse(
		FPragma_Party_InviteResponseV1Notification Notification,
		const FPragmaMessageMetadata& Metadata) const;
	void HandlePartyDetails(
		FPragma_Party_PartyDetailsV1Notification Notification,
		const FPragmaMessageMetadata& Metadata) const;
	void HandleGameInstanceFailure(
		FPragma_GameInstance_GameInstanceStartFailureV1Notification Notification,
		const FPragmaMessageMetadata& Metadata) const;
	void HandleHostConnectionDetailsReceived(
		FPragma_GameInstance_HostConnectionDetailsV1Notification Notification,
		const FPragmaMessageMetadata& Metadata) const;
	void HandleRemovedFromGame(
		FPragma_GameInstance_RemovedFromGameV1Notification Notification,
		const FPragmaMessageMetadata& Metadata) const;
	void HandleGameInstanceReconnect(
		FPragma_GameInstance_GameInstanceReconnectV1Notification Notification,
		const FPragmaMessageMetadata& Metadata) const;
	void HandleGameInstanceTermination(
		FPragma_GameInstance_GameInstanceTerminationV1Notification Notification,
		const FPragmaMessageMetadata& Metadata) const;
	void HandleGameEnded(
		FPragma_GameInstance_GameEndedV1Notification Notification,
		const FPragmaMessageMetadata& Metadata) const;
	void HandleGameClientNotSupported(
		FPragma_Party_GameClientNotSupportedV1Notification Notification,
		const FPragmaMessageMetadata& Metadata) const;
	void HandlePartyClientVersionMismatch(
		FPragma_Party_PartyClientVersionMismatchV1Notification Notification,
		const FPragmaMessageMetadata& Metadata) const;
	void HandleRemovedFromParty(
		FPragma_Party_RemovedV1Notification Notification,
		const FPragmaMessageMetadata& Metadata) const;
	void HandleMatchmakingFailure(
		FPragma_Matchmaking_MatchmakingFailureV1Notification Pragma_Matchmaking_MatchmakingFailureV1Notification,
		const FPragmaMessageMetadata& PragmaMessageMetadata) const;
	void HandleAddedToGame(
		FPragma_GameInstance_AddedToGameV1Notification Notification,
		const FPragmaMessageMetadata& Metadata) const;
	void HandleDisconnected();

	////////////////////////////////////////////////////////////////
	// Party Proxy Handlers
	////////////////////////////////////////////////////////////////
	
	void HandlePartyChanged(const UPragmaParty* Party) const
	{
		OnPartyChanged.Broadcast(Party);
	}

	void HandleJoinedParty(const UPragmaParty* Party) const
	{
		OnJoinedParty.Broadcast(Party);
	}

	void HandleInviteCodeChanged(const FString InviteCode) const
	{
		OnPartyInviteCodeChanged.Broadcast(InviteCode);
	}

	void HandleGameInstanceIdChanged(const FString GameInstanceId) const
	{
		OnGameInstanceIdChanged.Broadcast(GameInstanceId);
	}

	void HandleExtBroadcastPartyChanged(const FPragma_Party_ExtBroadcastParty& ExtBroadcastParty) const
	{
		OnExtBroadcastPartyChanged.Broadcast(ExtBroadcastParty);
	}

	void HandleExtPrivatePlayerChanged(const FPragma_Party_ExtPrivatePlayer& ExtPrivatePlayer) const
	{
		OnExtPrivatePlayerChanged.Broadcast(ExtPrivatePlayer);
	}

	void HandlePlayerJoined(const UPragmaPartyPlayer* Player) const
	{
		OnPlayerJoinedParty.Broadcast(Player);
	}

	void HandlePartyPlayerDataChanged(const UPragmaPartyPlayer* Player) const
	{
		OnPartyPlayerDataChanged.Broadcast(Player);
	}

	void HandlePlayerLeftParty(const UPragmaPartyPlayer* Player) const
	{
		OnPlayerLeftParty.Broadcast(Player);
	}

	void HandlePlayersChanged(const TArray<const UPragmaPartyPlayer*>& Players) const
	{
		OnPartyPlayersChanged.Broadcast(Players);
	}

	void HandlePartyLeft() const
	{
		OnLeftParty.Broadcast();
	}

	void HandleOnGameSessionChanged(const TMap<int, FString> Attributes)
	{
		if (!Attributes.Contains(static_cast<int32>(EPragma_GameSessionAttribute::PARTY_ID)) &&
			!Attributes.Contains(static_cast<int32>(EPragma_GameSessionAttribute::MATCHMAKING_ID)) &&
			!Attributes.Contains(static_cast<int32>(EPragma_GameSessionAttribute::GAME_INSTANCE_ID)))
		{
			HardResetAllState();
		}
	}

	void HandleMatchmakingIdSessionAttributeChanged(const FString* MatchmakingId) const
	{
		if (MatchmakingId != nullptr)
		{
			OnEnteredMatchmaking.Broadcast();
		}
		else
		{
			OnLeftMatchmaking.Broadcast();
		}
	}

	void HandleGameInstanceIdSessionAttributeChanged(const FString* GameInstanceId) const
	{
		if (GameInstanceId != nullptr)
		{
			OnGameInstanceIdChanged.Broadcast(*GameInstanceId);
		}
		else
		{
			OnGameInstanceIdChanged.Broadcast("");
		}
	}

	void HandlePreferredGameServerZonesChanged(const TArray<FString>& GameServerZones) const
	{
		OnPartyPreferredGameServerZonesChanged.Broadcast(GameServerZones);
	}

	////////////////////////////////////////////////////////////////
	// Private Functions
	////////////////////////////////////////////////////////////////

	void RemoveInviteById(const FString& InviteId);
	void ResetParty() const;
	// Completely resets all party state, invites, and other state, e.g. in the case of disconnection.
	void HardResetAllState();
	TFuture<TPragmaResult<>> Promisify(const TUniqueFunction<void(FOnCompleteDelegate)>& Func) const;
};
