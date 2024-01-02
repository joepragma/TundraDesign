// Fill out your copyright notice in the Description page of Project Settings.

#include "TundraDesignPlayerController.h"

#include "PragmaLocalPlayerSubsystem.h"
#include "TundraDesignPragmaAdapter.h"

ATundraDesignPlayerController::ATundraDesignPlayerController(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	PragmaAdapter = NewObject<UTundraDesignPragmaAdapter>();
}

void ATundraDesignPlayerController::BeginPlay()
{
	Super::BeginPlay();

	const auto PragmaPlayer = GetLocalPlayer()->GetSubsystem<UPragmaLocalPlayerSubsystem>()->Player();
	PragmaAdapter->Initialize(PragmaPlayer);

	PragmaAdapter->TundraLogin();
}
