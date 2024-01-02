// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TundraDesignPlayerController.generated.h"

class UTundraDesignPragmaAdapter;

UCLASS()
class TUNDRADESIGN_API ATundraDesignPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	ATundraDesignPlayerController(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, Category="TundraDesign")
	UTundraDesignPragmaAdapter* PragmaAdapter;
};
