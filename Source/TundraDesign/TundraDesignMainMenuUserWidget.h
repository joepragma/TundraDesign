// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PragmaPtr.h"
#include "TundraDesignMainMenuUserWidget.generated.h"

PRAGMA_FWD(FPlayer);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoggedInAndDataGotten);

UCLASS()
class TUNDRADESIGN_API UTundraDesignMainMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintAssignable, Category="TundraDesign")
	FOnLoggedInAndDataGotten OnLoggedInAndDataGotten;

private:
	Pragma::FPlayerPtr PragmaPlayer;

	void LoginAndGetData();
};
