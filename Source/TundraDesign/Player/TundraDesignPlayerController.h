// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PragmaPtr.h"
#include "TundraDesignPlayerController.generated.h"

PRAGMA_FWD(FPlayer);

UCLASS()
class TUNDRADESIGN_API ATundraDesignPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	Pragma::FPlayerPtr Player;
};
