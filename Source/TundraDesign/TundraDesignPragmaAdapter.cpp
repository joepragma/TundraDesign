// Fill out your copyright notice in the Description page of Project Settings.

#include "TundraDesignPragmaAdapter.h"

#include "PragmaPlayer.h"
#include "PragmaResult.h"
#include "Dto/PragmaAccountRpcDto.h"

void UTundraDesignPragmaAdapter::Initialize(const Pragma::FPlayerPtr& InPragmaPlayer)
{
	PragmaPlayer = InPragmaPlayer;

	PragmaPlayer->GameLoopApi().OnJoinedParty.AddUObject(this, &UTundraDesignPragmaAdapter::HandlePragmaOnJoinedParty);
	PragmaPlayer->GameLoopApi().OnPartyChanged.AddUObject(this, &UTundraDesignPragmaAdapter::HandlePragmaOnPartyChanged);
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
		FOnCompleteDelegate::CreateWeakLambda(this, [](const TPragmaResult<>& Result)
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

void UTundraDesignPragmaAdapter::LeaveParty()
{
	UE_LOG(LogTemp, Display, TEXT("PragmaAdapter - Leaving party..."));
	PragmaPlayer->GameLoopApi().LeaveParty(
		FOnCompleteDelegate::CreateWeakLambda(this, [](const TPragmaResult<>& Result)
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
