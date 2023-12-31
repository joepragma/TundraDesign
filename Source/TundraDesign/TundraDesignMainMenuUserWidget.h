// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PragmaPtr.h"
#include "TundraDesignTypes.h"
#include "TundraDesignMainMenuUserWidget.generated.h"

PRAGMA_FWD(FPlayer);

class UPragmaParty;

UCLASS()
class TUNDRADESIGN_API UTundraDesignMainMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	/******* PLAYER *******/

	void TundraLogin();

	UFUNCTION(BlueprintImplementableEvent, Category="TundraDesign")
	void OnTundraLogin();

	UFUNCTION(BlueprintImplementableEvent, Category="TundraDesign")
	void OnTundraPlayerUpdated(FTundraDesignPlayer TundraPlayer);

	/******* PARTY *******/

	UFUNCTION(BlueprintCallable, Category="TundraDesign")
	FTundraDesignParty GetParty();

	UFUNCTION(BlueprintCallable, Category="TundraDesign")
	bool IsInParty();

	UFUNCTION(BlueprintCallable, Category="TundraDesign")
	void CreateParty(ETundraDesignGameMode GameMode);

	void HandlePragmaOnJoinedParty(const UPragmaParty* PragmaParty);

	UFUNCTION(BlueprintImplementableEvent, Category="TundraDesign")
	void OnJoinedParty();

	void HandlePragmaOnPartyChanged(const UPragmaParty* PragmaParty);

	UFUNCTION(BlueprintImplementableEvent, Category="TundraDesign")
	void OnPartyChanged(FTundraDesignParty TundraParty);

	UFUNCTION(BlueprintCallable, Category="TundraDesign")
	void LeaveParty();

	void HandlePragmaOnLeftParty();

	UFUNCTION(BlueprintImplementableEvent, Category="TundraDesign")
	void OnLeftParty();

private:
	Pragma::FPlayerPtr PragmaPlayer;
};
