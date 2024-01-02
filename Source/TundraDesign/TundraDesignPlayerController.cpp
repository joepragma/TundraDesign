// Fill out your copyright notice in the Description page of Project Settings.

#include "TundraDesignPlayerController.h"

#include "PragmaLocalPlayerSubsystem.h"
#include "PragmaPlayer.h"
#include "PragmaResult.h"
#include "Dto/PragmaAccountRpcDto.h"

void ATundraDesignPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PragmaPlayer = GetLocalPlayer()->GetSubsystem<UPragmaLocalPlayerSubsystem>()->Player();

	PragmaPlayer->GameLoopApi().OnJoinedParty.AddUObject(this, &ATundraDesignPlayerController::HandlePragmaOnJoinedParty);
	PragmaPlayer->GameLoopApi().OnPartyChanged.AddUObject(this, &ATundraDesignPlayerController::HandlePragmaOnPartyChanged);
	PragmaPlayer->GameLoopApi().OnLeftParty.AddUObject(this, &ATundraDesignPlayerController::HandlePragmaOnLeftParty);

	TundraLogin();
}

void ATundraDesignPlayerController::TundraLogin()
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

FTundraDesignParty ATundraDesignPlayerController::GetParty()
{
	if (!IsInParty())
	{
		return FTundraDesignParty{};
	}

	return ToTundraDesignParty(PragmaPlayer->GameLoopApi().GetParty());
}

bool ATundraDesignPlayerController::IsInParty()
{
	return PragmaPlayer->GameLoopApi().HasParty();
}

void ATundraDesignPlayerController::CreateParty(ETundraDesignGameMode GameMode)
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

void ATundraDesignPlayerController::HandlePragmaOnJoinedParty(const UPragmaParty*) const
{
	OnJoinedParty.Broadcast();
}

void ATundraDesignPlayerController::HandlePragmaOnPartyChanged(const UPragmaParty* PragmaParty) const
{
	OnPartyChanged.Broadcast(ToTundraDesignParty(PragmaParty));
}

void ATundraDesignPlayerController::LeaveParty()
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

void ATundraDesignPlayerController::HandlePragmaOnLeftParty() const
{
	OnLeftParty.Broadcast();
}
