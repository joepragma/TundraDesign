#include "Pragma/Api/Player/PragmaGameLoopApi.h"

#include "PragmaSdkConfig.h"
#include "PragmaPlayer.h"
#include "Dto/PragmaMatchmakingServiceRaw.h"
#include "Dto/PragmaPartyServiceRaw.h"
#include "Services/Party/PragmaPartyProxy.h"
#include "Services/Party/PragmaParty.h"
#include "Services/Party/PragmaPartyPlayer.h"
#include "Services/PragmaSessionService.h"

DEFINE_LOG_CATEGORY_STATIC(LogPragmaGameLoopApi, Error, All);

#define GAME_LOOP_API_LOG(Verbosity, Format, ...) { PRAGMA_BASE_LOG(LogPragmaGameLoopApi, Verbosity, Format, ##__VA_ARGS__); }

// ************************************************************
// *** THIS FILE WAS STUBBED FOR THIS TUNDRA DESIGN PROJECT ***
// ************************************************************

void UPragmaGameLoopApi::SetDependencies(
	UPragmaPartyServiceRaw* InPartyRaw,
	UPragmaMatchmakingServiceRaw* InMatchmakingRaw,
	UPragmaGameInstanceServiceRaw* InGameInstanceRaw,
	UPragmaSessionService* InSessions,
	UPragmaSdkConfig* InConfig)
{
	SetDependencies(InPartyRaw, InMatchmakingRaw, InGameInstanceRaw, InSessions, nullptr, InConfig);
}

void UPragmaGameLoopApi::SetDependencies(
	UPragmaPartyServiceRaw* InPartyRaw,
	UPragmaMatchmakingServiceRaw* InMatchmakingRaw,
	UPragmaGameInstanceServiceRaw* InGameInstanceRaw,
	UPragmaSessionService* InSessions,
	UPragmaPartyProxy* InPartyProxy,
	UPragmaSdkConfig* InConfig)
{
	PartyServiceRaw = InPartyRaw;
	MatchmakingServiceRaw = InMatchmakingRaw;
	GameInstanceServiceRaw = InGameInstanceRaw;
	SessionService = InSessions;
	SdkConfig = InConfig;

	if (InPartyProxy == nullptr)
	{
		PartyProxy = NewObject<UPragmaPartyProxy>();
	}
	else
	{
		PartyProxy = InPartyProxy;
	}

	PartyServiceRaw->OnPartyDetailsV1.AddUObject(this, &UPragmaGameLoopApi::HandlePartyDetails);
	PartyServiceRaw->OnInviteReceivedV1.AddUObject(this, &UPragmaGameLoopApi::HandleInviteReceived);
	PartyServiceRaw->OnInviteResponseV1.AddUObject(this, &UPragmaGameLoopApi::HandleInviteResponse);
	PartyServiceRaw->OnRemovedV1.AddUObject(this, &UPragmaGameLoopApi::HandleRemovedFromParty);
	PartyServiceRaw->OnGameClientNotSupportedV1.AddUObject(this, &UPragmaGameLoopApi::HandleGameClientNotSupported);
	PartyServiceRaw->OnPartyClientVersionMismatchV1.AddUObject(this, &UPragmaGameLoopApi::HandlePartyClientVersionMismatch);
	MatchmakingServiceRaw->OnMatchmakingFailureV1.AddUObject(this, &UPragmaGameLoopApi::HandleMatchmakingFailure);
	GameInstanceServiceRaw->OnAddedToGameV1.AddUObject(this, &UPragmaGameLoopApi::HandleAddedToGame);
	GameInstanceServiceRaw->OnGameInstanceStartFailureV1.AddUObject(this, &UPragmaGameLoopApi::HandleGameInstanceFailure);
	GameInstanceServiceRaw->OnHostConnectionDetailsV1.AddUObject(this, &UPragmaGameLoopApi::HandleHostConnectionDetailsReceived);
	GameInstanceServiceRaw->OnRemovedFromGameV1.AddUObject(this, &UPragmaGameLoopApi::HandleRemovedFromGame);
	GameInstanceServiceRaw->OnGameInstanceReconnectV1.AddUObject(this, &UPragmaGameLoopApi::HandleGameInstanceReconnect);
	GameInstanceServiceRaw->OnGameInstanceTerminationV1.AddUObject(this, &UPragmaGameLoopApi::HandleGameInstanceTermination);
	GameInstanceServiceRaw->OnGameEndedV1.AddUObject(this, &UPragmaGameLoopApi::HandleGameEnded);
	SessionService->OnGameSessionChanged.AddUObject(this, &UPragmaGameLoopApi::HandleOnGameSessionChanged);
	SessionService->GetAttributeChangedDelegate(EPragma_GameSessionAttribute::MATCHMAKING_ID)
				  .AddUObject(this, &UPragmaGameLoopApi::HandleMatchmakingIdSessionAttributeChanged);
	SessionService->GetAttributeChangedDelegate(EPragma_GameSessionAttribute::GAME_INSTANCE_ID)
				  .AddUObject(this, &UPragmaGameLoopApi::HandleGameInstanceIdSessionAttributeChanged);
	PartyProxy->OnPartyChanged.AddUObject(this, &UPragmaGameLoopApi::HandlePartyChanged);
	PartyProxy->OnPartyJoined.AddUObject(this, &UPragmaGameLoopApi::HandleJoinedParty);
	PartyProxy->OnInviteCodeChanged.AddUObject(this, &UPragmaGameLoopApi::HandleInviteCodeChanged);
	PartyProxy->OnExtBroadcastPartyChanged.AddUObject(this, &UPragmaGameLoopApi::HandleExtBroadcastPartyChanged);
	PartyProxy->OnExtPrivatePlayerChanged.AddUObject(this,
		&UPragmaGameLoopApi::HandleExtPrivatePlayerChanged);
	PartyProxy->OnPlayerJoined.AddUObject(this, &UPragmaGameLoopApi::HandlePlayerJoined);
	PartyProxy->OnPlayerDataChanged.AddUObject(this, &UPragmaGameLoopApi::HandlePartyPlayerDataChanged);
	PartyProxy->OnPlayerLeft.AddUObject(this, &UPragmaGameLoopApi::HandlePlayerLeftParty);
	PartyProxy->OnPlayersChanged.AddUObject(this, &UPragmaGameLoopApi::HandlePlayersChanged);
	PartyProxy->OnPartyLeft.AddUObject(this, &UPragmaGameLoopApi::HandlePartyLeft);
	PartyProxy->OnPreferredGameServerZonesChanged.AddUObject(this,
		&UPragmaGameLoopApi::HandlePreferredGameServerZonesChanged);

	// At the moment degraded state will leave party on the platform because when we reconnect it will be a new session.
	Player()->OnDegraded.AddUObject(this, &UPragmaGameLoopApi::HandleDisconnected);
	Player()->OnDisconnected.AddUObject(this, &UPragmaGameLoopApi::HandleDisconnected);
}

////////////////////////////////////////////////////////////////
// Actions.
////////////////////////////////////////////////////////////////

void UPragmaGameLoopApi::CreateParty(
	const FPragma_Party_ExtCreateRequest& ExtCreateRequest,
	const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
	const TArray<FString>& PreferredGameServerZones,
	const TMap<FString, int32>& GameServerZoneToPing,
	const FOnCompleteDelegate& OnComplete)
{
	FPragmaError Error;
	if (!CanCreateParty(Error))
	{
		OnComplete.ExecuteIfBound(TPragmaResult<>(Error));
		return;
	}

	StubbedCreateParty(ExtCreateRequest, OnComplete);
}

void UPragmaGameLoopApi::StubbedCreateParty(
	const FPragma_Party_ExtCreateRequest& ExtCreateRequest,
	const FOnCompleteDelegate& OnComplete) const
{
	FPragma_Party_BroadcastParty BroadcastParty;
	BroadcastParty.PartyId = "StubbedPragmaPartyId";
	BroadcastParty.InviteCode = "BQ4TN9";
	BroadcastParty.ExtBroadcastParty = {ExtCreateRequest.GameMode};
	BroadcastParty.ExtPrivatePlayer = {};
	BroadcastParty.PreferredGameServerZones = TArray<FString>();
	FPragma_Party_BroadcastPartyMember BroadcastPartyMember;
	BroadcastPartyMember.PlayerId = Player()->Id();
	BroadcastPartyMember.SocialId = Player()->SocialId();
	FPragma_Account_DisplayName DisplayName;
	DisplayName.DisplayName = Player()->DisplayName();
	DisplayName.Discriminator = Player()->Discriminator();
	BroadcastPartyMember.DisplayName = DisplayName;
	BroadcastPartyMember.Ext = {};
	BroadcastPartyMember.IsLeader = true;
	BroadcastPartyMember.IsReady = false;
	BroadcastParty.PartyMembers = {BroadcastPartyMember};

	PartyProxy->Initialize(MoveTemp(BroadcastParty), 0);
	SessionService->StubbedSetGameAttribute(EPragma_GameSessionAttribute::PARTY_ID, BroadcastParty.PartyId);
	OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
}

TFuture<TPragmaResult<>> UPragmaGameLoopApi::CreateParty(
	const FPragma_Party_ExtCreateRequest& ExtCreateRequest,
	const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
	const TArray<FString>& PreferredGameServerZones,
	const TMap<FString, int32>& GameServerZoneToPing)
{
	return Promisify(
		[this, &ExtCreateRequest, &ExtPlayerJoinRequest, &PreferredGameServerZones, &GameServerZoneToPing](
		const FOnCompleteDelegate& OnComplete)
		{
			CreateParty(ExtCreateRequest, ExtPlayerJoinRequest, PreferredGameServerZones, GameServerZoneToPing,
				OnComplete);
		});
}

void UPragmaGameLoopApi::CreateParty(
	const FPragma_Party_ExtCreateRequest& ExtCreateRequest,
	const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
	const TArray<FString>& PreferredGameServerZones,
	const FOnCompleteDelegate& OnComplete)
{
	CreateParty(ExtCreateRequest, ExtPlayerJoinRequest, PreferredGameServerZones, TMap<FString, int32>{}, OnComplete);
}

TFuture<TPragmaResult<>> UPragmaGameLoopApi::CreateParty(
	const FPragma_Party_ExtCreateRequest& ExtCreateRequest,
	const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
	const TArray<FString>& PreferredGameServerZones)
{
	return Promisify(
		[this, &ExtCreateRequest, &ExtPlayerJoinRequest, &PreferredGameServerZones](
		const FOnCompleteDelegate& OnComplete)
		{
			CreateParty(ExtCreateRequest, ExtPlayerJoinRequest, PreferredGameServerZones, OnComplete);
		});
}

void UPragmaGameLoopApi::CreateParty(
	const FPragma_Party_ExtCreateRequest& ExtCreateRequest,
	const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
	const FOnCompleteDelegate& OnComplete)
{
	CreateParty(ExtCreateRequest, ExtPlayerJoinRequest, TArray<FString>{}, OnComplete);
}

TFuture<TPragmaResult<>> UPragmaGameLoopApi::CreateParty(
	const FPragma_Party_ExtCreateRequest& ExtCreateRequest,
	const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest)
{
	return Promisify([this, &ExtCreateRequest, &ExtPlayerJoinRequest](const FOnCompleteDelegate& OnComplete)
	{
		CreateParty(ExtCreateRequest, ExtPlayerJoinRequest, OnComplete);
	});
}

bool UPragmaGameLoopApi::CanCreateParty() const
{
	FPragmaError Error;
	return CanCreateParty(Error);
}

bool UPragmaGameLoopApi::CanCreateParty(FPragmaError& OutError) const
{
	return true;
}

void UPragmaGameLoopApi::SendPartyInvite(
	const FString& PlayerId,
	const FOnInviteSentDelegate& OnComplete)
{
	FPragmaError Error;
	if (!CanSendPartyInvite(Error))
	{
		OnComplete.ExecuteIfBound(TPragmaResult<FString>(Error));
		return;
	}

	FPragma_Party_SendInviteV1Request Request = {PlayerId};
	PartyServiceRaw->SendInviteV1(
		Request,
		UPragmaPartyServiceRaw::FSendInviteV1Delegate::CreateWeakLambda(this,
			[this, Request, OnComplete](const TPragmaResult<FPragma_Party_SendInviteV1Response> Result,
			const FPragmaMessageMetadata&)
			{
				if (Result.IsFailure())
				{
					OnComplete.ExecuteIfBound(TPragmaResult<FString>::Failure(Result.Error()));
					return;
				}
				GAME_LOOP_API_LOG(Log, "Successfully invited player '%s'.", *Request.InviteePlayerId);
				OnComplete.ExecuteIfBound(TPragmaResult<FString>(Result.Payload().InviteId));
			}
		));
}

TFuture<TPragmaResult<FString>> UPragmaGameLoopApi::SendPartyInvite(const FString& PlayerId)
{
	auto Promise = MakeShared<TPromise<TPragmaResult<FString>>>();
	SendPartyInvite(PlayerId, FOnInviteSentDelegate::CreateLambda([Promise](TPragmaResult<FString> Result)
	{
		Promise->SetValue(MoveTemp(Result));
	}));
	return Promise->GetFuture();
}

bool UPragmaGameLoopApi::CanSendPartyInvite() const
{
	FPragmaError Error;
	return CanSendPartyInvite(Error);
}

bool UPragmaGameLoopApi::CanSendPartyInvite(FPragmaError& OutError) const
{
	return IsInParty(OutError);
}

void UPragmaGameLoopApi::RespondToPartyInvite(
	const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
	const FString& InviteId,
	const bool Accepted,
	const FOnCompleteDelegate& OnComplete)
{
	RespondToPartyInvite(ExtPlayerJoinRequest, InviteId, Accepted, {}, OnComplete);
}

TFuture<TPragmaResult<>> UPragmaGameLoopApi::RespondToPartyInvite(
	const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
	const FString& InviteId,
	const bool Accepted)
{
	return Promisify([this, &ExtPlayerJoinRequest, &InviteId,&Accepted](const FOnCompleteDelegate& OnComplete)
	{
		RespondToPartyInvite(ExtPlayerJoinRequest, InviteId, Accepted, OnComplete);
	});
}

void UPragmaGameLoopApi::RespondToPartyInvite(
	const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
	const FString& InviteId,
	const bool Accepted,
	const TMap<FString, int>& GameServerZoneToPing,
	const FOnCompleteDelegate& OnComplete)
{
	FPragmaError Error;
	if (!CanAcceptPartyInvite(Error))
	{
		OnComplete.ExecuteIfBound(TPragmaResult<>(Error));
		return;
	}

	auto Request = FPragma_Party_RespondToInviteV1Request{
		ExtPlayerJoinRequest,
		InviteId,
		Accepted,
		SdkConfig->GetGameClientVersion(),
		GameServerZoneToPing
	};
	PartyServiceRaw->RespondToInviteV1(Request,
		UPragmaPartyServiceRaw::FRespondToInviteV1Delegate::CreateWeakLambda(this,
			[this, InviteId = Request.InviteId, Accepted, OnComplete](
			TPragmaResult<FPragma_Party_RespondToInviteV1Response> Result,
			const FPragmaMessageMetadata& Metadata)
			{
				if (Result.IsFailure())
				{
					OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(Result.Error()));
					return;
				}

				if (Accepted)
				{
					PartyProxy->Initialize(MoveTemp(Result.Payload().Party), Metadata.SequenceNumber);
					GAME_LOOP_API_LOG(Log, "Successfully accepted invite.");
				}
				else
				{
					GAME_LOOP_API_LOG(Log, "Successfully rejected invite.");
				}

				RemoveInviteById(InviteId);
				OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
			}
		));
}

TFuture<TPragmaResult<>> UPragmaGameLoopApi::RespondToPartyInvite(
	const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
	const FString& InviteId,
	const bool Accepted,
	const TMap<FString, int>& GameServerZoneToPing)
{
	return Promisify(
		[this, &ExtPlayerJoinRequest, &InviteId, &Accepted, &GameServerZoneToPing](
		const FOnCompleteDelegate& OnComplete)
		{
			RespondToPartyInvite(ExtPlayerJoinRequest, InviteId, Accepted, GameServerZoneToPing, OnComplete);
		});
}

bool UPragmaGameLoopApi::CanAcceptPartyInvite() const
{
	FPragmaError Error;
	return CanAcceptPartyInvite(Error);
}

bool UPragmaGameLoopApi::CanAcceptPartyInvite(FPragmaError& OutError) const
{
	return true;
}

void UPragmaGameLoopApi::JoinPartyWithInviteCode(
	const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
	const FString& InviteCode,
	const FOnCompleteDelegate& OnComplete)
{
	JoinPartyWithInviteCode(ExtPlayerJoinRequest, InviteCode, {}, OnComplete);
}

TFuture<TPragmaResult<>> UPragmaGameLoopApi::JoinPartyWithInviteCode(
	const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
	const FString& InInviteCode)
{
	return Promisify(
		[this, &ExtPlayerJoinRequest, &InInviteCode](const FOnCompleteDelegate& OnComplete)
		{
			JoinPartyWithInviteCode(ExtPlayerJoinRequest, InInviteCode, OnComplete);
		});
}

void UPragmaGameLoopApi::JoinPartyWithInviteCode(
	const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
	const FString& InviteCode,
	const TMap<FString, int>& GameServerZoneToPing,
	const FOnCompleteDelegate& OnComplete)
{
	FPragmaError Error;
	if (!CanJoinParty(Error))
	{
		OnComplete.ExecuteIfBound(TPragmaResult<>(Error));
		return;
	}

	const auto Request = FPragma_Party_JoinWithInviteCodeV1Request{
		ExtPlayerJoinRequest,
		InviteCode,
		SdkConfig->GetGameClientVersion(),
		GameServerZoneToPing
	};
	PartyServiceRaw->JoinWithInviteCodeV1(Request,
		UPragmaPartyServiceRaw::FJoinWithInviteCodeV1Delegate::CreateWeakLambda(this,
			[this, OnComplete](
			TPragmaResult<FPragma_Party_JoinWithInviteCodeV1Response> Result,
			const FPragmaMessageMetadata& Metadata)
			{
				if (Result.IsFailure())
				{
					OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(Result.Error()));
					return;
				}

				PartyProxy->Initialize(MoveTemp(Result.Payload().Party), Metadata.SequenceNumber);
				GAME_LOOP_API_LOG(Log, "Successfully joined party with invite code.");
				OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
			}
		));
}

TFuture<TPragmaResult<>> UPragmaGameLoopApi::JoinPartyWithInviteCode(
	const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
	const FString& InviteCode,
	const TMap<FString, int>& GameServerZoneToPing)
{
	return Promisify(
		[this, &ExtPlayerJoinRequest, &InviteCode, &GameServerZoneToPing](const FOnCompleteDelegate& OnComplete)
		{
			JoinPartyWithInviteCode(ExtPlayerJoinRequest, InviteCode, GameServerZoneToPing, OnComplete);
		});
}

bool UPragmaGameLoopApi::CanJoinParty() const
{
	FPragmaError Error;
	return CanJoinParty(Error);
}

bool UPragmaGameLoopApi::CanJoinParty(FPragmaError& OutError) const
{
	return CanAcceptPartyInvite(OutError);
}

void UPragmaGameLoopApi::JoinPartyWithId(
	const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
	const FString& PartyId,
	const FOnCompleteDelegate& OnComplete)
{
	JoinPartyWithId(ExtPlayerJoinRequest, PartyId, {}, OnComplete);
}

TFuture<TPragmaResult<>> UPragmaGameLoopApi::JoinPartyWithId(
	const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
	const FString& InPartyId)
{
	return Promisify(
		[this, &ExtPlayerJoinRequest, &InPartyId](const FOnCompleteDelegate& OnComplete)
		{
			JoinPartyWithId(ExtPlayerJoinRequest, InPartyId, OnComplete);
		});
}

void UPragmaGameLoopApi::JoinPartyWithId(
	const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
	const FString& PartyId,
	const TMap<FString, int>& GameServerZoneToPing,
	const FOnCompleteDelegate& OnComplete)
{
	FPragmaError Error;
	if (!CanJoinParty(Error))
	{
		OnComplete.ExecuteIfBound(TPragmaResult<>(Error));
		return;
	}

	const auto Request = FPragma_Party_JoinWithPartyIdV1Request{
		ExtPlayerJoinRequest,
		SdkConfig->GetGameClientVersion(),
		PartyId,
		GameServerZoneToPing
	};
	PartyServiceRaw->JoinWithPartyIdV1(Request,
		UPragmaPartyServiceRaw::FJoinWithPartyIdV1Delegate::CreateWeakLambda(this,
			[this, OnComplete](
			TPragmaResult<FPragma_Party_JoinWithPartyIdV1Response> Result,
			const FPragmaMessageMetadata& Metadata)
			{
				if (Result.IsFailure())
				{
					OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(Result.Error()));
					return;
				}

				PartyProxy->Initialize(MoveTemp(Result.Payload().Party), Metadata.SequenceNumber);
				GAME_LOOP_API_LOG(Log, "Successfully joined party with invite code.");
				OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
			}
		));
}

TFuture<TPragmaResult<>> UPragmaGameLoopApi::JoinPartyWithId(
	const FPragma_Party_ExtPlayerJoinRequest& ExtPlayerJoinRequest,
	const FString& InPartyId,
	const TMap<FString, int>& GameServerZoneToPing)
{
	return Promisify(
		[this, &ExtPlayerJoinRequest, &InPartyId, &GameServerZoneToPing](const FOnCompleteDelegate& OnComplete)
		{
			JoinPartyWithId(ExtPlayerJoinRequest, InPartyId, GameServerZoneToPing, OnComplete);
		});
}

void UPragmaGameLoopApi::LeaveParty(const FOnCompleteDelegate& OnComplete)
{
	FPragmaError Error;
	if (!CanLeaveParty(Error))
	{
		OnComplete.ExecuteIfBound(TPragmaResult<>(Error));
		return;
	}

	StubbedLeaveParty(OnComplete);
}

void UPragmaGameLoopApi::StubbedLeaveParty(const FOnCompleteDelegate& OnComplete) const
{
	PartyProxy->TryReset(0);
	SessionService->StubbedClearGameAttribute(EPragma_GameSessionAttribute::PARTY_ID);
	OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
}


TFuture<TPragmaResult<>> UPragmaGameLoopApi::LeaveParty()
{
	return Promisify([this](const FOnCompleteDelegate& OnComplete) { LeaveParty(OnComplete); });
}

bool UPragmaGameLoopApi::CanLeaveParty() const
{
	FPragmaError Error;
	return CanLeaveParty(Error);
}

bool UPragmaGameLoopApi::CanLeaveParty(FPragmaError& OutError) const
{
	return IsInParty(OutError);
}

void UPragmaGameLoopApi::AssignPartyLeader(const FString& PlayerId, const FOnCompleteDelegate& OnComplete)
{
	FPragmaError Error;
	if (!CanAssignPartyLeader(PlayerId, Error))
	{
		OnComplete.ExecuteIfBound(TPragmaResult<>(Error));
		return;
	}

	const FPragma_Party_AssignPartyLeaderV1Request Request{PlayerId};
	PartyServiceRaw->AssignPartyLeaderV1(Request,
		UPragmaPartyServiceRaw::FAssignPartyLeaderV1Delegate::CreateWeakLambda(this,
			[this, OnComplete](const TPragmaResult<FPragma_Party_AssignPartyLeaderV1Response> Result,
			const FPragmaMessageMetadata& Metadata)
			{
				if (Result.IsFailure())
				{
					OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(Result.Error()));
					return;
				}

				GAME_LOOP_API_LOG(Log, "AssignPartyLeaderV1 -- Success.");
				PartyProxy->UpdateFrom(Result.Payload().Party, Metadata.SequenceNumber);
				OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
			}
		));
}

TFuture<TPragmaResult<>> UPragmaGameLoopApi::AssignPartyLeader(const FString& PlayerId)
{
	return Promisify([this, PlayerId](const FOnCompleteDelegate& OnComplete)
	{
		AssignPartyLeader(PlayerId, OnComplete);
	});
}

bool UPragmaGameLoopApi::CanAssignPartyLeader(const FString& PlayerId) const
{
	FPragmaError Error;
	return CanAssignPartyLeader(PlayerId, Error);
}

bool UPragmaGameLoopApi::CanAssignPartyLeader(const FString& PlayerId, FPragmaError& OutError) const
{
	if (!HasParty())
	{
		OutError = EPragma_PlatformError::PartyService_NotInParty;
		return false;
	}
	if (SessionNotInParty())
	{
		OutError = EPragma_PlatformError::PartyService_InvalidState;
		return false;
	}
	if (!IsLeaderOfParty(Player()->Id()))
	{
		OutError = EPragma_PlatformError::PartyService_PlayerNotLeader;
		return false;
	}
	if (IsLeaderOfParty(PlayerId))
	{
		OutError = EPragma_PlatformError::PartyService_PlayerIsLeader;
		return false;
	}
	return true;
}

void UPragmaGameLoopApi::KickPlayerFromParty(const FString& PlayerId, const FOnCompleteDelegate& OnComplete)
{
	FPragmaError Error;
	if (!CanKickPlayerFromParty(PlayerId, Error))
	{
		OnComplete.ExecuteIfBound(TPragmaResult<>(Error));
		return;
	}

	const FPragma_Party_KickV1Request Request{PlayerId};
	PartyServiceRaw->KickV1(Request,
		UPragmaPartyServiceRaw::FKickV1Delegate::CreateWeakLambda(this,
			[this, OnComplete](const TPragmaResult<FPragma_Party_KickV1Response> Result,
			const FPragmaMessageMetadata& Metadata)
			{
				if (Result.IsFailure())
				{
					OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(Result.Error()));
					return;
				}

				GAME_LOOP_API_LOG(Log, "KickPlayerV1 -- Success.");
				PartyProxy->UpdateFrom(Result.Payload().Party, Metadata.SequenceNumber);
				OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
			}
		));
}

TFuture<TPragmaResult<>> UPragmaGameLoopApi::KickPlayerFromParty(const FString& PlayerId)
{
	return Promisify([this, PlayerId](const FOnCompleteDelegate& OnComplete) { KickPlayerFromParty(PlayerId, OnComplete); });
}

bool UPragmaGameLoopApi::CanKickPlayerFromParty(const FString& PlayerId) const
{
	FPragmaError Error;
	return CanKickPlayerFromParty(PlayerId, Error);
}

bool UPragmaGameLoopApi::CanKickPlayerFromParty(const FString& PlayerId, FPragmaError& OutError) const
{
	if (Player()->Id() == PlayerId)
	{
		OutError = EPragma_PlatformError::PartyService_CanNotKickSelf;
		return false;
	}
	if (!HasParty())
	{
		OutError = EPragma_PlatformError::PartyService_NotInParty;
		return false;
	}
	if (SessionNotInParty())
	{
		OutError = EPragma_PlatformError::PartyService_InvalidState;
		return false;
	}
	if (!IsLeaderOfParty(Player()->Id()))
	{
		OutError = EPragma_PlatformError::PartyService_PlayerNotLeader;
		return false;
	}
	return true;
}

void UPragmaGameLoopApi::UpdatePartyPlayer(
	const FPragma_Party_ExtUpdatePartyPlayerRequest& RequestExt,
	const FOnCompleteDelegate& OnComplete)
{
	FPragmaError Error;
	if (!IsInParty(Error))
	{
		OnComplete.ExecuteIfBound(TPragmaResult<>(Error));
		return;
	}

	const FPragma_Party_UpdatePartyPlayerV1Request Request{RequestExt};
	PartyServiceRaw->UpdatePartyPlayerV1(Request,
		UPragmaPartyServiceRaw::FUpdatePartyPlayerV1Delegate::CreateWeakLambda(this,
			[this, OnComplete](TPragmaResult<FPragma_Party_UpdatePartyPlayerV1Response> Result,
			const FPragmaMessageMetadata& Metadata)
			{
				if (Result.IsFailure())
				{
					OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(Result.Error()));
					return;
				}
				GAME_LOOP_API_LOG(Log, "Loadout updated successfully.");
				PartyProxy->UpdateFrom(MoveTemp(Result.Payload().Party), Metadata.SequenceNumber);
				OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
			}
		));
}

TFuture<TPragmaResult<>> UPragmaGameLoopApi::UpdatePartyPlayer(const FPragma_Party_ExtUpdatePartyPlayerRequest& RequestExt)
{
	return Promisify([this, &RequestExt](const FOnCompleteDelegate& OnComplete)
	{
		UpdatePartyPlayer(RequestExt, OnComplete);
	});
}

bool UPragmaGameLoopApi::CanUpdatePartyPlayer() const
{
	FPragmaError Error;
	return CanUpdatePartyPlayer(Error);
}

bool UPragmaGameLoopApi::CanUpdatePartyPlayer(FPragmaError& OutError) const
{
	return IsInParty(OutError);
}

void UPragmaGameLoopApi::UpdateParty(const FPragma_Party_ExtUpdatePartyRequest& RequestExt,
	const FOnCompleteDelegate& OnComplete)
{
	FPragmaError Error;
	if (!IsInParty(Error))
	{
		OnComplete.ExecuteIfBound(TPragmaResult<>(Error));
		return;
	}

	const FPragma_Party_UpdatePartyV1Request Request{RequestExt};
	PartyServiceRaw->UpdatePartyV1(Request,
		UPragmaPartyServiceRaw::FUpdatePartyV1Delegate::CreateWeakLambda(this,
			[this, OnComplete](TPragmaResult<FPragma_Party_UpdatePartyV1Response> Result,
			const FPragmaMessageMetadata& Metadata)
			{
				if (Result.IsFailure())
				{
					OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(Result.Error()));
					return;
				}
				GAME_LOOP_API_LOG(Log, "Party updated successfully.");
				PartyProxy->UpdateFrom(MoveTemp(Result.Payload().Party), Metadata.SequenceNumber);
				OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
			}
		));
}

TFuture<TPragmaResult<>> UPragmaGameLoopApi::UpdateParty(const FPragma_Party_ExtUpdatePartyRequest& RequestExt)
{
	return Promisify([this, &RequestExt](const FOnCompleteDelegate& OnComplete)
	{
		UpdateParty(RequestExt, OnComplete);
	});
}

bool UPragmaGameLoopApi::CanUpdateParty() const
{
	FPragmaError Error;
	return CanUpdateParty(Error);
}

bool UPragmaGameLoopApi::CanUpdateParty(FPragmaError& OutError) const
{
	return IsInParty(OutError);
}

void UPragmaGameLoopApi::SetPartyPlayerGameServerZoneToPing(
	const TMap<FString, int32>& GameServerZoneToPing,
	const FOnCompleteDelegate& OnComplete)
{
	FPragmaError Error;
	if (!CanSetPartyPlayerGameServerZoneToPing(Error))
	{
		OnComplete.ExecuteIfBound(TPragmaResult<>(Error));
		return;
	}

	const FPragma_Party_SetGameServerZoneToPingV1Request Request{GameServerZoneToPing};
	PartyServiceRaw->SetGameServerZoneToPingV1(Request,
		UPragmaPartyServiceRaw::FSetGameServerZoneToPingV1Delegate::CreateWeakLambda(this,
			[this, OnComplete](TPragmaResult<FPragma_Party_SetGameServerZoneToPingV1Response> Result,
			const FPragmaMessageMetadata&)
			{
				if (Result.IsFailure())
				{
					OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(Result.Error()));
					return;
				}
				GAME_LOOP_API_LOG(Log, "GameServerZoneToPing set successfully.");
				OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
			}
		));
}

TFuture<TPragmaResult<>> UPragmaGameLoopApi::SetPartyPlayerGameServerZoneToPing(
	const TMap<FString, int32>& GameServerZoneToPing)
{
	return Promisify([this, GameServerZoneToPing](const FOnCompleteDelegate& OnComplete)
	{
		SetPartyPlayerGameServerZoneToPing(GameServerZoneToPing, OnComplete);
	});
}

bool UPragmaGameLoopApi::CanSetPartyPlayerGameServerZoneToPing() const
{
	FPragmaError Error;
	return CanSetPartyPlayerGameServerZoneToPing(Error);
}

bool UPragmaGameLoopApi::CanSetPartyPlayerGameServerZoneToPing(FPragmaError& OutError) const
{
	return IsInParty(OutError);
}

void UPragmaGameLoopApi::SetPartyPreferredGameServerZones(TArray<FString> GameServerZones,
	const FOnCompleteDelegate& OnComplete)
{
	FPragmaError Error;
	if (!CanSetPartyPreferredGameServerZones(Error))
	{
		OnComplete.ExecuteIfBound(TPragmaResult<>(Error));
		return;
	}

	const FPragma_Party_SetPreferredGameServerZonesV1Request Request{MoveTemp(GameServerZones)};
	PartyServiceRaw->SetPreferredGameServerZonesV1(Request,
		UPragmaPartyServiceRaw::FSetPreferredGameServerZonesV1Delegate::CreateWeakLambda(this,
			[this, OnComplete](TPragmaResult<FPragma_Party_SetPreferredGameServerZonesV1Response> Result,
			const FPragmaMessageMetadata& Metadata)
			{
				if (Result.IsFailure())
				{
					OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(Result.Error()));
					return;
				}
				GAME_LOOP_API_LOG(Log, "PreferredGameServerZones set successfully.");
				PartyProxy->UpdateFrom(MoveTemp(Result.Payload().Party), Metadata.SequenceNumber);
				OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
			}
		));
}

TFuture<TPragmaResult<>> UPragmaGameLoopApi::SetPartyPreferredGameServerZones(TArray<FString> GameServerZones)
{
	return Promisify([this, GameServerZones = MoveTemp(GameServerZones)](const FOnCompleteDelegate& OnComplete)
	{
		SetPartyPreferredGameServerZones(GameServerZones, OnComplete);
	});
}

bool UPragmaGameLoopApi::CanSetPartyPreferredGameServerZones() const
{
	FPragmaError Error;
	return CanSetPartyPreferredGameServerZones(Error);
}

bool UPragmaGameLoopApi::CanSetPartyPreferredGameServerZones(FPragmaError& OutError) const
{
	if (!IsInParty(OutError))
	{
		return false;
	}
	if (!IsLeaderOfParty(Player()->Id()))
	{
		OutError = EPragma_PlatformError::PartyService_PlayerNotLeader;
		return false;
	}
	return true;
}

void UPragmaGameLoopApi::SetPartyPlayerReady(const bool bIsReady, const FOnCompleteDelegate& OnComplete)
{
	FPragmaError Error;
	if (!CanSetPartyPlayerReady(Error))
	{
		OnComplete.ExecuteIfBound(TPragmaResult<>(Error));
		return;
	}

	const auto* LocalPartyPlayer = GetLocalPartyPlayer();
	if (LocalPartyPlayer == nullptr || LocalPartyPlayer->bIsReady == bIsReady)
	{
		OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
		return;
	}

	const FPragma_Party_SetReadyStateV1Request Request{bIsReady};
	PartyServiceRaw->SetReadyStateV1(Request,
		UPragmaPartyServiceRaw::FSetReadyStateV1Delegate::CreateWeakLambda(this,
			[this, OnComplete](const TPragmaResult<FPragma_Party_SetReadyStateV1Response> Result,
			const FPragmaMessageMetadata& Metadata)
			{
				if (Result.IsFailure())
				{
					OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(Result.Error()));
					return;
				}
				GAME_LOOP_API_LOG(Log, "SetReadyState -- Success.");
				PartyProxy->UpdateFrom(Result.Payload().Party, Metadata.SequenceNumber);
				OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
			}
		));
}

TFuture<TPragmaResult<>> UPragmaGameLoopApi::SetPartyPlayerReady(bool bIsReady)
{
	return Promisify(
		[this, &bIsReady](const FOnCompleteDelegate& OnComplete) { SetPartyPlayerReady(bIsReady, OnComplete); });
}

bool UPragmaGameLoopApi::CanSetPartyPlayerReady() const
{
	FPragmaError Error;
	return CanSetPartyPlayerReady(Error);
}

bool UPragmaGameLoopApi::CanSetPartyPlayerReady(FPragmaError& OutError) const
{
	return IsInParty(OutError);
} 

void UPragmaGameLoopApi::EnterMatchmaking(const FOnCompleteDelegate& OnComplete)
{
	FPragmaError Error;
	if (!CanEnterMatchmaking(Error))
	{
		OnComplete.ExecuteIfBound(TPragmaResult<>(Error));
		return;
	}

	const FPragma_Party_EnterMatchmakingV1Request Request{};
	PartyServiceRaw->EnterMatchmakingV1(Request,
		UPragmaPartyServiceRaw::FEnterMatchmakingV1Delegate::CreateWeakLambda(this,
			[this, OnComplete](const TPragmaResult<FPragma_Party_EnterMatchmakingV1Response> Result,
			const FPragmaMessageMetadata& Metadata)
			{
				if (Result.IsFailure())
				{
					OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(Result.Error()));
					return;
				}
				GAME_LOOP_API_LOG(Log, "Matchmaking entered successfully.");
				PartyProxy->UpdateFrom(Result.Payload().Party, Metadata.SequenceNumber);
				OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
			}
		));
}

TFuture<TPragmaResult<>> UPragmaGameLoopApi::EnterMatchmaking()
{
	return Promisify([this](const FOnCompleteDelegate& OnComplete) { EnterMatchmaking(OnComplete); });
}

bool UPragmaGameLoopApi::CanEnterMatchmaking() const
{
	FPragmaError Error;
	return CanEnterMatchmaking(Error);
}

bool UPragmaGameLoopApi::CanEnterMatchmaking(FPragmaError& OutError) const
{
	if (!IsInParty(OutError))
	{
		return false;
	}
	if (!IsLeaderOfParty(Player()->Id()))
	{
		OutError = EPragma_PlatformError::PartyService_PlayerNotLeader;
		return false;
	}
	if (!GetParty()->AreAllPlayersReady())
	{
		OutError = EPragma_PlatformError::PartyService_PlayersNotReady;
		return false;
	}
	return true;
}

void UPragmaGameLoopApi::LeaveMatchmaking(const FOnCompleteDelegate& OnComplete)
{
	FPragmaError Error;
	if (!CanLeaveMatchmaking(Error))
	{
		OnComplete.ExecuteIfBound(TPragmaResult<>(Error));
		return;
	}

	const FPragma_Matchmaking_LeaveMatchmakingV2Request Request{};
	MatchmakingServiceRaw->LeaveMatchmakingV2(Request,
		UPragmaMatchmakingServiceRaw::FLeaveMatchmakingV2Delegate::CreateWeakLambda(this,
			[this, OnComplete](const TPragmaResult<FPragma_Matchmaking_LeaveMatchmakingV2Response> Result,
			const FPragmaMessageMetadata&)
			{
				if (Result.IsFailure())
				{
					OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(Result.Error()));
					return;
				}
				GAME_LOOP_API_LOG(Log, "Matchmaking left successfully.");
				OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
			}
		));
}

TFuture<TPragmaResult<>> UPragmaGameLoopApi::LeaveMatchmaking()
{
	return Promisify([this](const FOnCompleteDelegate& OnComplete) { LeaveMatchmaking(OnComplete); });
}

bool UPragmaGameLoopApi::CanLeaveMatchmaking() const
{
	FPragmaError Error;
	return CanLeaveMatchmaking(Error);
}

bool UPragmaGameLoopApi::CanLeaveMatchmaking(FPragmaError& OutError) const
{
	if (!SessionService->GetAttribute(EPragma_GameSessionAttribute::MATCHMAKING_ID))
	{
		OutError = EPragma_PlatformError::PartyService_InvalidState;
		return false;
	}
	return true;
}

void UPragmaGameLoopApi::GetMatchmakingInfo(const FMatchmakingQueueInfoDelegate& MatchmakingQueueInfoDelegate)
{
	const FPragma_Matchmaking_GetMatchmakingInfoV2Request Request{};
	MatchmakingServiceRaw->GetMatchmakingInfoV2(Request,
		UPragmaMatchmakingServiceRaw::FGetMatchmakingInfoV2Delegate::CreateWeakLambda(this,
			[this, MatchmakingQueueInfoDelegate](TPragmaResult<FPragma_Matchmaking_GetMatchmakingInfoV2Response> Result,
			const FPragmaMessageMetadata&)
			{
				if (Result.IsFailure())
				{
					MatchmakingQueueInfoDelegate.ExecuteIfBound(
						TPragmaResult<TArray<FPragma_Matchmaking_QueueInfoV2>>::Failure(Result.Error()));
					return;
				}
				MatchmakingQueueInfoDelegate.ExecuteIfBound(
					TPragmaResult<TArray<FPragma_Matchmaking_QueueInfoV2>>{MoveTemp(Result.Payload().QueueInfo)});
			}
		));
}

void UPragmaGameLoopApi::DeclineReconnect(const FOnCompleteDelegate& OnComplete)
{
	FPragmaError Error;
	if (!CanDeclineReconnect(Error))
	{
		OnComplete.ExecuteIfBound(TPragmaResult<>(Error));
		return;
	}

	const auto Request = FPragma_GameInstance_DeclineReconnectV1Request{
		*SessionService->GetAttribute(EPragma_GameSessionAttribute::GAME_INSTANCE_ID)
	};
	GameInstanceServiceRaw->DeclineReconnectV1(Request,
		UPragmaGameInstanceServiceRaw::FDeclineReconnectV1Delegate::CreateWeakLambda(this,
			[this, OnComplete](const TPragmaResult<FPragma_GameInstance_DeclineReconnectV1Response> Result,
			const FPragmaMessageMetadata&)
			{
				if (Result.IsFailure())
				{
					OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(Result.Error()));
					return;
				}
				GAME_LOOP_API_LOG(Log, "Successfully declined match reconnect.");
				OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
			}
		));
}

TFuture<TPragmaResult<>> UPragmaGameLoopApi::DeclineReconnect()
{
	auto Promise = MakeShared<TPromise<TPragmaResult<>>>();
	DeclineReconnect(FOnCompleteDelegate::CreateLambda(
		[Promise](TPragmaResult<> Result)
		{
			Promise->SetValue(MoveTemp(Result));
		}));
	return Promise->GetFuture();
}

bool UPragmaGameLoopApi::CanDeclineReconnect() const
{
	FPragmaError Error;
	return CanDeclineReconnect(Error);
}

bool UPragmaGameLoopApi::CanDeclineReconnect(FPragmaError& OutError) const
{
	if (!SessionService->GetAttribute(EPragma_GameSessionAttribute::GAME_INSTANCE_ID))
	{
		OutError = EPragma_PlatformError::GameInstanceService_PlayerNotInGameInstance;
		return false;
	}
	return true;
}

void UPragmaGameLoopApi::GetHostConnectionDetails(const FHostConnectionDetailsDelegate& OnComplete)
{
	FPragmaError Error;
	if (!CanGetMatchConnectionDetails(Error))
	{
		OnComplete.ExecuteIfBound(TPragmaResult<FPragma_GameInstance_HostConnectionDetails>(Error));
		return;
	}

	const auto Request = FPragma_GameInstance_GetHostConnectionDetailsV1Request{
		*SessionService->GetAttribute(EPragma_GameSessionAttribute::GAME_INSTANCE_ID)
	};
	GameInstanceServiceRaw->GetHostConnectionDetailsV1(Request,
		UPragmaGameInstanceServiceRaw::FGetHostConnectionDetailsV1Delegate::CreateWeakLambda(this,
			[this, OnComplete](const TPragmaResult<FPragma_GameInstance_GetHostConnectionDetailsV1Response> Result,
			const FPragmaMessageMetadata&)
			{
				if (Result.IsFailure())
				{
					OnComplete.ExecuteIfBound(
						TPragmaResult<FPragma_GameInstance_HostConnectionDetails>::Failure(Result.Error()));
					return;
				}
				GAME_LOOP_API_LOG(Log, "Host connection details acquired.");
				OnComplete.ExecuteIfBound(TPragmaResult<FPragma_GameInstance_HostConnectionDetails>{
					Result.Payload().HostConnectionDetails
				});
			}
		));
}

TFuture<TPragmaResult<FPragma_GameInstance_HostConnectionDetails>> UPragmaGameLoopApi::GetHostConnectionDetails()
{
	auto Promise = MakeShared<TPromise<TPragmaResult<FPragma_GameInstance_HostConnectionDetails>>>();
	GetHostConnectionDetails(FHostConnectionDetailsDelegate::CreateLambda(
		[Promise](TPragmaResult<FPragma_GameInstance_HostConnectionDetails> Result)
		{
			Promise->SetValue(MoveTemp(Result));
		}));
	return Promise->GetFuture();
}

bool UPragmaGameLoopApi::CanGetMatchConnectionDetails() const
{
	FPragmaError Error;
	return CanGetMatchConnectionDetails(Error);
}

bool UPragmaGameLoopApi::CanGetMatchConnectionDetails(FPragmaError& OutError) const
{
	if (!SessionService->GetAttribute(EPragma_GameSessionAttribute::GAME_INSTANCE_ID))
	{
		OutError = EPragma_PlatformError::GameInstanceService_PlayerNotInGameInstance;
		return false;
	}
	return true;
}

////////////////////////////////////////////////////////////////
// Helper Functions.
////////////////////////////////////////////////////////////////

bool UPragmaGameLoopApi::HasParty() const
{
	return PartyProxy->HasParty();
}

bool UPragmaGameLoopApi::SessionNotInParty() const
{
	return SessionService->GetAttribute(EPragma_GameSessionAttribute::PARTY_ID) == nullptr;
}

bool UPragmaGameLoopApi::IsLeaderOfParty(FString PlayerId) const
{
	return GetParty()->GetAllLeaders().ContainsByPredicate([PlayerId](const UPragmaPartyPlayer* Player)
	{
		return Player->GetPlayerId() == PlayerId;
	});
}

bool UPragmaGameLoopApi::IsInParty(FPragmaError& OutError) const
{
	if (!HasParty())
	{
		OutError = EPragma_PlatformError::PartyService_NotInParty;
		return false;
	}
	if (SessionNotInParty())
	{
		OutError = EPragma_PlatformError::PartyService_InvalidState;
		return false;
	}
	return true;
}

const UPragmaPartyPlayer* UPragmaGameLoopApi::GetLocalPartyPlayer() const
{
	return HasParty() ? GetParty()->GetPlayer(Player()->Id()) : nullptr;
}

TOptional<FPragmaPartyInvite> UPragmaGameLoopApi::GetPartyInviteByInviteId(const FString& InviteId) const
{
	const auto Invite = PendingInvites.FindByPredicate([&InviteId](const auto& Invite) { return Invite.GetInviteId() == InviteId; });
	return Invite ? *Invite : TOptional<FPragmaPartyInvite>();
}

TOptional<FPragmaPartyInvite> UPragmaGameLoopApi::GetPartyInviteByInviterId(const FString& InviterId) const
{
	const auto Invite = PendingInvites.FindByPredicate([&InviterId](const auto& Invite) { return Invite.GetInviterId() == InviterId; });
	return Invite ? *Invite : TOptional<FPragmaPartyInvite>();
}

////////////////////////////////////////////////////////////////
// Notification Handlers.
////////////////////////////////////////////////////////////////

void UPragmaGameLoopApi::HandlePartyDetails(
	FPragma_Party_PartyDetailsV1Notification Notification,
	const FPragmaMessageMetadata& Metadata) const
{
	PartyProxy->UpdateFrom(MoveTemp(Notification.Party), Metadata.SequenceNumber);
}

void UPragmaGameLoopApi::HandleGameInstanceFailure(
	FPragma_GameInstance_GameInstanceStartFailureV1Notification Notification,
	const FPragmaMessageMetadata&) const
{
	OnFailedToAllocateGameInstance.Broadcast();
}

void UPragmaGameLoopApi::HandleMatchmakingFailure(
	FPragma_Matchmaking_MatchmakingFailureV1Notification Notification,
	const FPragmaMessageMetadata&) const
{
	OnMatchmakingFailed.Broadcast(Notification.Reason);
}

void UPragmaGameLoopApi::HandleHostConnectionDetailsReceived(
	FPragma_GameInstance_HostConnectionDetailsV1Notification Notification,
	const FPragmaMessageMetadata&) const
{
	OnHostConnectionDetailsReceived.Broadcast(MoveTemp(Notification.ConnectionDetails));
}

void UPragmaGameLoopApi::HandleRemovedFromGame(
	FPragma_GameInstance_RemovedFromGameV1Notification Notification,
	const FPragmaMessageMetadata&) const
{
	OnRemovedFromGame.Broadcast(MoveTemp(Notification.GameInstanceId), MoveTemp(Notification.Ext));
}

void UPragmaGameLoopApi::HandleGameInstanceReconnect(
	FPragma_GameInstance_GameInstanceReconnectV1Notification Notification,
	const FPragmaMessageMetadata&) const
{
	OnGameInstanceReconnect.Broadcast(MoveTemp(Notification.HostConnectionDetails));
}

void UPragmaGameLoopApi::HandleGameInstanceTermination(
	FPragma_GameInstance_GameInstanceTerminationV1Notification Notification,
	const FPragmaMessageMetadata&) const
{
	OnGameInstanceTerminated.Broadcast(MoveTemp(Notification.GameInstanceId), MoveTemp(Notification.Reason));
}

void UPragmaGameLoopApi::HandleGameEnded(
	FPragma_GameInstance_GameEndedV1Notification Notification,
	const FPragmaMessageMetadata&) const
{
	OnGameEnded.Broadcast(MoveTemp(Notification.GameInstanceId), MoveTemp(Notification.Ext));
}

void UPragmaGameLoopApi::HandleGameClientNotSupported(
	FPragma_Party_GameClientNotSupportedV1Notification Notification,
	const FPragmaMessageMetadata&) const
{
	OnGameClientVersionUpdateRequired.Broadcast();
}

void UPragmaGameLoopApi::HandlePartyClientVersionMismatch(
	FPragma_Party_PartyClientVersionMismatchV1Notification Notification,
	const FPragmaMessageMetadata&) const
{
	OnPartyClientVersionMismatch.Broadcast();
}

void UPragmaGameLoopApi::HandleRemovedFromParty(
	FPragma_Party_RemovedV1Notification Notification,
	const FPragmaMessageMetadata& Metadata) const
{
	if (PartyProxy->TryReset(Metadata.SequenceNumber))
	{
		switch (Notification.RemovalReason)
		{
		case EPragma_Party_RemovalReason::DISCONNECTED:
			GAME_LOOP_API_LOG(Verbose, "Removed from party; Disconnected from Pragma.");
			break;
		case EPragma_Party_RemovalReason::KICKED:
			GAME_LOOP_API_LOG(Verbose, "Removed from party; Kicked by leader.");
			break;
		case EPragma_Party_RemovalReason::REMOVAL_REASON_UNSPECIFIED:
		default:
			GAME_LOOP_API_LOG(Verbose, "Removed from party; Default handler.");
		}
	}
	OnRemovedFromParty.Broadcast(Notification.RemovalReason);
}

void UPragmaGameLoopApi::HandleInviteReceived(
	FPragma_Party_InviteReceivedV1Notification Notification,
	const FPragmaMessageMetadata&)
{
	const auto ExistingInvite = GetPartyInviteByInviterId(Notification.InviterInfo.PlayerId);
	if (ExistingInvite)
	{
		RemoveInviteById(ExistingInvite->GetInviteId());
		OnPartyInviteRevoked.Broadcast(ExistingInvite->GetInviteId());
	}
	GAME_LOOP_API_LOG(Verbose, "Received party invite from PragmaId '%s', inviteId: %s.",
		*Notification.InviterInfo.PlayerId,
		*Notification.InviteId);
	PendingInvites.Add(FPragmaPartyInvite(MoveTemp(Notification)));
	OnPartyInvitesChanged.Broadcast(PendingInvites);
	OnPartyInviteReceived.Broadcast(PendingInvites.Last());
}

void UPragmaGameLoopApi::HandleInviteResponse(
	FPragma_Party_InviteResponseV1Notification Notification,
	const FPragmaMessageMetadata&) const
{
	if (Notification.Accepted == true)
	{
		GAME_LOOP_API_LOG(Verbose, "Party invite: '%s' was accepted", *Notification.InviteId);
		OnPartyInviteAccepted.Broadcast(Notification.InviteId);
	}
	else
	{
		GAME_LOOP_API_LOG(Verbose, "Party invite: '%s' was declined", *Notification.InviteId);
		OnPartyInviteDeclined.Broadcast(Notification.InviteId);
	}
}

void UPragmaGameLoopApi::HandleAddedToGame(
		const FPragma_GameInstance_AddedToGameV1Notification Notification,
		const FPragmaMessageMetadata&) const
{
	OnAddedToGame.Broadcast(Notification.GameInstanceId, Notification.Ext);
}

void UPragmaGameLoopApi::HandleDisconnected()
{
	HardResetAllState();
}

////////////////////////////////////////////////////////////////
// Private Functions.
////////////////////////////////////////////////////////////////

void UPragmaGameLoopApi::RemoveInviteById(const FString& InviteId)
{
	const int32 Removed = PendingInvites.RemoveAll([&InviteId](const FPragmaPartyInvite& Invite)
	{
		return Invite.GetInviteId() == InviteId;
	});
	if (Removed > 0)
	{
		OnPartyInvitesChanged.Broadcast(PendingInvites);
	}
}

void UPragmaGameLoopApi::ResetParty() const
{
	PartyProxy->ForceReset();
}

void UPragmaGameLoopApi::HardResetAllState()
{
	ResetParty();

	if (PendingInvites.Num() > 0)
	{
		PendingInvites.Empty();
		OnPartyInvitesChanged.Broadcast(PendingInvites);
	}
}

TFuture<TPragmaResult<>> UPragmaGameLoopApi::Promisify(const TUniqueFunction<void(FOnCompleteDelegate)>& Func) const
{
	auto Promise = MakeShared<TPromise<TPragmaResult<>>>();
	Func(FOnCompleteDelegate::CreateLambda([Promise](TPragmaResult<> Result)
	{
		Promise->SetValue(MoveTemp(Result));
	}));
	return Promise->GetFuture();
}
