// Fill out your copyright notice in the Description page of Project Settings.


#include "TundraDesignUserWidget.h"

#include "TundraDesignPlayerController.h"

void UTundraDesignUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TundraController = Cast<ATundraDesignPlayerController>(GetOwningLocalPlayer()->PlayerController);
}
