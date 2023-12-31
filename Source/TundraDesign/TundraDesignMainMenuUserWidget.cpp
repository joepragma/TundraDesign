// Fill out your copyright notice in the Description page of Project Settings.

#include "TundraDesignMainMenuUserWidget.h"

#include "PragmaLocalPlayerSubsystem.h"
#include "PragmaPlayer.h"
#include "PragmaResult.h"
#include "Dto/PragmaAccountRpcDto.h"

void UTundraDesignMainMenuUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PragmaPlayer = GetOwningLocalPlayer()->GetSubsystem<UPragmaLocalPlayerSubsystem>()->Player();

	PragmaPlayer->GameLoopApi().OnJoinedParty.AddUObject(this, &UTundraDesignMainMenuUserWidget::HandlePragmaOnJoinedParty);
	PragmaPlayer->GameLoopApi().OnPartyChanged.AddUObject(this, &UTundraDesignMainMenuUserWidget::HandlePragmaOnPartyChanged);
	PragmaPlayer->GameLoopApi().OnLeftParty.AddUObject(this, &UTundraDesignMainMenuUserWidget::HandlePragmaOnLeftParty);

	TundraLogin();
}

void UTundraDesignMainMenuUserWidget::TundraLogin()
{
	UE_LOG(LogTemp, Display, TEXT("MainMenu - Logging in..."));
	const auto SteamToken = "MyFakeSteamTokenFromConfig";
	PragmaPlayer->LogIn(
		EPragma_Account_IdProvider::STEAM,
		SteamToken,
		Pragma::FPlayer::FLoggedInDelegate::CreateWeakLambda(this, [this](const TPragmaResult<>& Result)
		{
			if (Result.IsSuccessful())
			{
				UE_LOG(LogTemp, Display, TEXT("MainMenu - Login succeeded."));
				OnTundraLogin();
				FTundraDesignPlayer TundraPlayer;
				TundraPlayer.PlayerId = PragmaPlayer->Id();
				TundraPlayer.SocialId = PragmaPlayer->SocialId();
				TundraPlayer.Username = PragmaPlayer->FullDisplayName();
				OnTundraPlayerUpdated(TundraPlayer);
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("MainMenu - Login failed."));
			}
		}));
}

FTundraDesignParty UTundraDesignMainMenuUserWidget::GetParty()
{
	if (!IsInParty())
	{
		return FTundraDesignParty{};
	}

	return ToTundraDesignParty(PragmaPlayer->GameLoopApi().GetParty());
}

bool UTundraDesignMainMenuUserWidget::IsInParty()
{
	return PragmaPlayer->GameLoopApi().HasParty();
}

void UTundraDesignMainMenuUserWidget::CreateParty(ETundraDesignGameMode GameMode)
{
	UE_LOG(LogTemp, Display, TEXT("MainMenu - Creating party..."));
	const FPragma_Party_ExtCreateRequest ExtCreateParty{ToPragmaGameMode(GameMode)};
	const FPragma_Party_ExtPlayerJoinRequest ExtJoinParty{};
	PragmaPlayer->GameLoopApi().CreateParty(
		ExtCreateParty,
		ExtJoinParty,
		FOnCompleteDelegate::CreateWeakLambda(this, [](const TPragmaResult<>& Result)
		{
			if (Result.IsSuccessful())
			{
				UE_LOG(LogTemp, Display, TEXT("MainMenu - Create party succeeded."));
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("MainMenu - Create party failed."));
			}
		}));
}

void UTundraDesignMainMenuUserWidget::HandlePragmaOnJoinedParty(const UPragmaParty*)
{
	OnJoinedParty();
}

void UTundraDesignMainMenuUserWidget::HandlePragmaOnPartyChanged(const UPragmaParty* PragmaParty)
{
	OnPartyChanged(ToTundraDesignParty(PragmaParty));
}

void UTundraDesignMainMenuUserWidget::LeaveParty()
{
	UE_LOG(LogTemp, Display, TEXT("MainMenu - Leaving party..."));
	PragmaPlayer->GameLoopApi().LeaveParty(
		FOnCompleteDelegate::CreateWeakLambda(this, [](const TPragmaResult<>& Result)
		{
			if (Result.IsSuccessful())
			{
				UE_LOG(LogTemp, Display, TEXT("MainMenu - Leave party succeeded."));
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("MainMenu - Leave party failed."));
			}
		}));
}

void UTundraDesignMainMenuUserWidget::HandlePragmaOnLeftParty()
{
	OnLeftParty();
}
