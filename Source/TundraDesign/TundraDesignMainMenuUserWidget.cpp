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

	UE_LOG(LogTemp, Display, TEXT("MainMenu - Logging in..."));
	const auto SteamToken = "MyFakeSteamTokenFromConfig";
	LogIn(SteamToken);
}

void UTundraDesignMainMenuUserWidget::LogIn(const FString& SteamToken)
{
	PragmaPlayer->LogIn(
		EPragma_Account_IdProvider::STEAM,
		SteamToken,
		Pragma::FPlayer::FLoggedInDelegate::CreateWeakLambda(this, [](const TPragmaResult<>& Result)
		{
			if (Result.IsSuccessful())
			{
				UE_LOG(LogTemp, Display, TEXT("MainMenu - Login succeeded."));
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("MainMenu - Login failed."));
			}
		}));
}
