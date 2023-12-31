// Fill out your copyright notice in the Description page of Project Settings.

#include "TundraDesignPragmaAdapter.h"

#include "PragmaPlayer.h"
#include "PragmaResult.h"
#include "Pragma/Api/Player/PragmaGameLoopApi.h"
#include "Dto/PragmaAccountRpcDto.h"

void UTundraDesignPragmaAdapter::Initialize(const Pragma::FPlayerPtr& InPragmaPlayer)
{
	PragmaPlayer = InPragmaPlayer;

	PragmaPlayer->GameLoopApi().OnJoinedParty.AddUObject(this, &UTundraDesignPragmaAdapter::HandlePragmaOnJoinedParty);
	PragmaPlayer->GameLoopApi().OnPartyChanged.AddUObject(this, &UTundraDesignPragmaAdapter::HandlePragmaOnPartyChanged);
	PragmaPlayer->GameLoopApi().OnPartyInviteReceived.AddUObject(this, &UTundraDesignPragmaAdapter::HandlePragmaOnPartyInviteReceived);
	PragmaPlayer->GameLoopApi().OnPartyInviteAccepted.AddUObject(this, &UTundraDesignPragmaAdapter::HandlePragmaOnInviteAccepted);
	PragmaPlayer->GameLoopApi().OnLeftParty.AddUObject(this, &UTundraDesignPragmaAdapter::HandlePragmaOnLeftParty);
	PragmaPlayer->GameLoopApi().OnRemovedFromParty.AddUObject(this, &UTundraDesignPragmaAdapter::HandlePragmaOnRemovedFromParty);
}

void UTundraDesignPragmaAdapter::TundraLogin()
{
	UE_LOG(LogTemp, Display, TEXT("PragmaAdapter - Logging in..."));
	const auto SteamToken = "MyFakeSteamTokenFromConfig";
	PragmaPlayer->LogIn(
		EPragma_Account_IdProvider::STEAM,
		SteamToken,
		Pragma::FPlayer::FLoggedInDelegate::CreateWeakLambda(this, [this](const TPragmaResult<>& Result)
		{
			if (Result.IsSuccessful())
			{
				UE_LOG(LogTemp, Display, TEXT("PragmaAdapter - Login succeeded."));
				OnTundraLogin.Broadcast();
				OnTundraPlayerUpdated.Broadcast(ToTundraDesignPlayer(PragmaPlayer));
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("PragmaAdapter - Login failed."));
			}
		}));
}

FTundraDesignPlayer UTundraDesignPragmaAdapter::GetClientTundraPlayer() const
{
	return ToTundraDesignPlayer(PragmaPlayer);
}

bool UTundraDesignPragmaAdapter::IsInParty()
{
	return PragmaPlayer->GameLoopApi().HasParty();
}

FTundraDesignParty UTundraDesignPragmaAdapter::GetParty()
{
	if (!IsInParty())
	{
		return FTundraDesignParty{};
	}

	return ToTundraDesignParty(PragmaPlayer->GameLoopApi().GetParty());
}

FTundraDesignPartyPlayer UTundraDesignPragmaAdapter::GetClientPartyPlayer()
{
	if (!IsInParty())
	{
		return FTundraDesignPartyPlayer{};
	}

	return ToTundraDesignPartyPlayer(PragmaPlayer->GameLoopApi().GetLocalPartyPlayer());
}

void UTundraDesignPragmaAdapter::CreateParty(const ETundraDesignGameMode& GameMode)
{
	UE_LOG(LogTemp, Display, TEXT("PragmaAdapter - Creating party..."));
	const FPragma_Party_ExtCreateRequest ExtCreateParty{ToPragmaGameMode(GameMode)};
	constexpr FPragma_Party_ExtPlayerJoinRequest ExtJoinParty{};
	PragmaPlayer->GameLoopApi().CreateParty(
		ExtCreateParty,
		ExtJoinParty,
		UPragmaGameLoopApi::FOnCompleteDelegate::CreateWeakLambda(this, [](const TPragmaResult<>& Result)
		{
			if (Result.IsSuccessful())
			{
				UE_LOG(LogTemp, Display, TEXT("PragmaAdapter - Create party succeeded."));
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("PragmaAdapter - Create party failed."));
			}
		}));
}

void UTundraDesignPragmaAdapter::JoinPartyWithInviteCode(const FString& InviteCode)
{
	UE_LOG(LogTemp, Display, TEXT("PragmaAdapter - Joining party with invite code..."));
	constexpr FPragma_Party_ExtPlayerJoinRequest ExtJoinParty{};
	PragmaPlayer->GameLoopApi().JoinPartyWithInviteCode(
		ExtJoinParty,
		InviteCode,
		UPragmaGameLoopApi::FOnCompleteDelegate::CreateWeakLambda(this, [](const TPragmaResult<>& Result)
		{
			if (Result.IsSuccessful())
			{
				UE_LOG(LogTemp, Display, TEXT("PragmaAdapter - Join party with invite code succeeded."));
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("PragmaAdapter - Join party with invite code failed."));
			}
		}));
}

void UTundraDesignPragmaAdapter::HandlePragmaOnPartyInviteReceived(const FPragmaPartyInvite& PragmaPartyInvite) const
{
	OnPartyInviteReceived.Broadcast(ToTundraDesignPartyInvite(PragmaPartyInvite));
}

void UTundraDesignPragmaAdapter::ResponseToPartyInvite(const FString& InviteId, const bool Accept)
{
	UE_LOG(LogTemp, Display, TEXT("PragmaAdapter - Responding to party invite..."));
	constexpr FPragma_Party_ExtPlayerJoinRequest ExtJoinParty{};
	PragmaPlayer->GameLoopApi().RespondToPartyInvite(
		ExtJoinParty,
		InviteId,
		Accept,
		UPragmaGameLoopApi::FOnCompleteDelegate::CreateWeakLambda(this, [](const TPragmaResult<>& Result)
		{
			if (Result.IsSuccessful())
			{
				UE_LOG(LogTemp, Display, TEXT("PragmaAdapter - Respond to invite succeeded."));
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("PragmaAdapter - Respond to invite failed."));
			}
		}));
}

void UTundraDesignPragmaAdapter::HandlePragmaOnJoinedParty(const UPragmaParty*) const
{
	OnJoinedParty.Broadcast();
}

void UTundraDesignPragmaAdapter::HandlePragmaOnPartyChanged(const UPragmaParty* PragmaParty) const
{
	OnPartyChanged.Broadcast(ToTundraDesignParty(PragmaParty));
}

void UTundraDesignPragmaAdapter::SendPartyInviteByUsername(const FString& Username)
{
	FString DisplayName, Discriminator;
	Username.Split("#", &DisplayName, &Discriminator);

	UE_LOG(LogTemp, Display, TEXT("PragmaAdapter - Getting player id for username..."));
	PragmaPlayer->Account().GetPragmaPlayerIdForDisplayName(
		DisplayName,
		Discriminator,
		UPragmaAccountService::FGetPragmaPlayerIdDelegate::CreateWeakLambda(
			this, [this, Username](const TPragmaResult<FString>& GetPlayerIdResult)
			{
				if (GetPlayerIdResult.IsSuccessful())
				{
					UE_LOG(LogTemp, Display, TEXT("PragmaAdapter - Get player id for username succeeded."));
					FString PlayerId = GetPlayerIdResult.Payload();
					UE_LOG(LogTemp, Display, TEXT("PragmaAdapter - Sending party invite by player id..."));
					PragmaPlayer->GameLoopApi().SendPartyInvite(
						PlayerId,
						UPragmaGameLoopApi::FOnInviteSentDelegate::CreateWeakLambda(
							this, [this, PlayerId, Username](const TPragmaResult<FString>& SendPartyInviteResult)
							{
								if (SendPartyInviteResult.IsSuccessful())
								{
									UE_LOG(LogTemp, Display, TEXT("PragmaAdapter - Send party invite by player id succeeded."));
									FString InviteId = SendPartyInviteResult.Payload();
									FTundraDesignSentPartyInvite SentPartyInvite;
									SentPartyInvite.InviteId = InviteId;
									SentPartyInvite.InviteePlayerId = PlayerId;
									SentPartyInvite.InviteeUsername = Username;
									SentPartyInvites.Add(SentPartyInvite);
									OnSentPartyInvitesChanged.Broadcast(SentPartyInvites);
								}
								else
								{
									UE_LOG(LogTemp, Display, TEXT("PragmaAdapter - Send party invite by player id failed."));
								}
							}));
				}
				else
				{
					UE_LOG(LogTemp, Display, TEXT("PragmaAdapter - Get player id for username failed."));
				}
			}));
}

// ReSharper disable once CppPassValueParameterByConstReference
void UTundraDesignPragmaAdapter::HandlePragmaOnInviteAccepted(const FString InviteId)
{
	SentPartyInvites.RemoveAll([InviteId](const FTundraDesignSentPartyInvite& Item)
	{
		return Item.InviteId == InviteId;
	});
	OnSentPartyInvitesChanged.Broadcast(SentPartyInvites);
}

void UTundraDesignPragmaAdapter::LeaveParty()
{
	UE_LOG(LogTemp, Display, TEXT("PragmaAdapter - Leaving party..."));
	PragmaPlayer->GameLoopApi().LeaveParty(
		UPragmaGameLoopApi::FOnCompleteDelegate::CreateWeakLambda(this, [](const TPragmaResult<>& Result)
		{
			if (Result.IsSuccessful())
			{
				UE_LOG(LogTemp, Display, TEXT("PragmaAdapter - Leave party succeeded."));
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("PragmaAdapter - Leave party failed."));
			}
		}));
}

void UTundraDesignPragmaAdapter::HandlePragmaOnLeftParty()
{
	HandlePragmaOnRemovedFromParty(EPragma_Party_RemovalReason::LEFT);
}

void UTundraDesignPragmaAdapter::HandlePragmaOnRemovedFromParty(const EPragma_Party_RemovalReason PragmaRemovalReason)
{
	SentPartyInvites.Empty();
	OnSentPartyInvitesChanged.Broadcast(SentPartyInvites);
	OnLeftParty.Broadcast(ToTundraDesignLeftPartyReason(PragmaRemovalReason));
}

void UTundraDesignPragmaAdapter::DevCheatAcceptFirstSentPartyInvite()
{
	if (!IsInParty() || SentPartyInvites.Num() == 0) return;

	const FTundraDesignSentPartyInvite SentInvite = SentPartyInvites[0];

	FString DisplayName, Discriminator;
	SentInvite.InviteeUsername.Split("#", &DisplayName, &Discriminator);

	UE_LOG(LogTemp, Display, TEXT("PragmaAdapter - Triggering accept first sent party invite..."));
	PragmaPlayer->GameLoopApi().StubbedTriggerInviteAccepted(
		SentInvite.InviteId,
		SentInvite.InviteePlayerId,
		FString::Printf(TEXT("StubbedSocialId%d"), FMath::RandRange(0, 10000)),
		DisplayName,
		Discriminator
	);
}

void UTundraDesignPragmaAdapter::DevCheatLastPlayerLeaveParty()
{
	if (!IsInParty() || GetParty().Players.Num() <= 1) return;

	const FTundraDesignPartyPlayer LastPlayer = GetParty().Players.Last();

	UE_LOG(LogTemp, Display, TEXT("PragmaAdapter - Triggering last player leave party..."));
	PragmaPlayer->GameLoopApi().StubbedTriggerPlayerLeftParty(LastPlayer.PlayerId);
}

void UTundraDesignPragmaAdapter::DevCheatGetKickedFromParty()
{
	if (!IsInParty()) return;

	UE_LOG(LogTemp, Display, TEXT("PragmaAdapter - Triggering get kicked from party..."));
	PragmaPlayer->GameLoopApi().StubbedTriggerRemovedFromParty(GetParty().Id, EPragma_Party_RemovalReason::KICKED);
}

void UTundraDesignPragmaAdapter::DevCheatReceiveInviteToParty()
{
	UE_LOG(LogTemp, Display, TEXT("PragmaAdapter - Triggering receive invite to party..."));
	PragmaPlayer->GameLoopApi().StubbedTriggerInviteReceived();
}
