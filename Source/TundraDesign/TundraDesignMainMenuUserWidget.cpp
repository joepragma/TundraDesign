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

	DoLogin();
}

void UTundraDesignMainMenuUserWidget::DoLogin()
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
				IsLoggedIn = true;
				Player.PlayerId = PragmaPlayer->Id();
				Player.SocialId = PragmaPlayer->SocialId();
				Player.Username = PragmaPlayer->FullDisplayName();
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("MainMenu - Login failed."));
			}
		}));
}
