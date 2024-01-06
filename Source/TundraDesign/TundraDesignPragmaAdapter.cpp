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
	PragmaPlayer->GameLoopApi().OnPartyInviteAccepted.AddUObject(this, &UTundraDesignPragmaAdapter::HandlePragmaOnInviteAccepted);
	PragmaPlayer->GameLoopApi().OnLeftParty.AddUObject(this, &UTundraDesignPragmaAdapter::HandlePragmaOnLeftParty);
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
				FTundraDesignPlayer TundraPlayer;
				TundraPlayer.PlayerId = PragmaPlayer->Id();
				TundraPlayer.SocialId = PragmaPlayer->SocialId();
				TundraPlayer.Username = PragmaPlayer->FullDisplayName();
				OnTundraPlayerUpdated.Broadcast(TundraPlayer);
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("PragmaAdapter - Login failed."));
			}
		}));
}

FTundraDesignParty UTundraDesignPragmaAdapter::GetParty()
{
	if (!IsInParty())
	{
		return FTundraDesignParty{};
	}

	return ToTundraDesignParty(PragmaPlayer->GameLoopApi().GetParty());
}

bool UTundraDesignPragmaAdapter::IsInParty()
{
	return PragmaPlayer->GameLoopApi().HasParty();
}

void UTundraDesignPragmaAdapter::CreateParty(ETundraDesignGameMode GameMode)
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

void UTundraDesignPragmaAdapter::HandlePragmaOnJoinedParty(const UPragmaParty*) const
{
	OnJoinedParty.Broadcast();
}

void UTundraDesignPragmaAdapter::HandlePragmaOnPartyChanged(const UPragmaParty* PragmaParty) const
{
	OnPartyChanged.Broadcast(ToTundraDesignParty(PragmaParty));
}

void UTundraDesignPragmaAdapter::SendPartyInviteByUsername(FString Username)
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

void UTundraDesignPragmaAdapter::HandlePragmaOnLeftParty() const
{
	OnLeftParty.Broadcast();
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
