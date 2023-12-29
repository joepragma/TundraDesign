// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PragmaPtr.h"
#include "TundraDesignStructs.h"
#include "TundraDesignMainMenuUserWidget.generated.h"

PRAGMA_FWD(FPlayer);

UCLASS()
class TUNDRADESIGN_API UTundraDesignMainMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UPROPERTY(BlueprintReadOnly, Category="TundraDesign")
	bool IsLoggedIn{false};

	UPROPERTY(BlueprintReadOnly, Category="TundraDesign")
	FTundraDesignPlayer Player{};

private:
	Pragma::FPlayerPtr PragmaPlayer;

	void DoLogin();
};
