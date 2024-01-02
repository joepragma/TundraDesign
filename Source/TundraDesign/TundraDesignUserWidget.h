// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TundraDesignUserWidget.generated.h"

class ATundraDesignPlayerController;

UCLASS()
class TUNDRADESIGN_API UTundraDesignUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

public:
	UPROPERTY(BlueprintReadOnly, Category="TundraDesign")
	ATundraDesignPlayerController* TundraController;
};
