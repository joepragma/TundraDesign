// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PragmaPtr.h"
#include "TundraDesignTypes.h"
#include "TundraDesignPragmaAdapter.generated.h"

PRAGMA_FWD(FPlayer);

class UPragmaParty;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTundraLogin);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTundraPlayerUpdated, FTundraDesignPlayer, TundraPlayer);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJoinedParty);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPartyChanged, FTundraDesignParty, TundraParty);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftParty);

UCLASS()
class TUNDRADESIGN_API UTundraDesignPragmaAdapter : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(const Pragma::FPlayerPtr& InPragmaPlayer);
	
	/******* PLAYER *******/

	void TundraLogin();

	UPROPERTY(BlueprintAssignable, Category="TundraDesign")
	FOnTundraLogin OnTundraLogin;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTundraLogin);

	UPROPERTY(BlueprintAssignable, Category="TundraDesign")
	FOnTundraPlayerUpdated OnTundraPlayerUpdated;

	/******* PARTY *******/

	UFUNCTION(BlueprintCallable, Category="TundraDesign")
	FTundraDesignParty GetParty();

	UFUNCTION(BlueprintCallable, Category="TundraDesign")
	bool IsInParty();

	UFUNCTION(BlueprintCallable, Category="TundraDesign")
	void CreateParty(ETundraDesignGameMode GameMode);

	UPROPERTY(BlueprintAssignable, Category="TundraDesign")
	FOnJoinedParty OnJoinedParty;
	void HandlePragmaOnJoinedParty(const UPragmaParty* PragmaParty) const;

	UPROPERTY(BlueprintAssignable, Category="TundraDesign")
	FOnPartyChanged OnPartyChanged;
	void HandlePragmaOnPartyChanged(const UPragmaParty* PragmaParty) const;

	UFUNCTION(BlueprintCallable, Category="TundraDesign")
	void LeaveParty();

	UPROPERTY(BlueprintAssignable, Category="TundraDesign")
	FOnLeftParty OnLeftParty;
	void HandlePragmaOnLeftParty() const;

private:
	Pragma::FPlayerPtr PragmaPlayer;
};
