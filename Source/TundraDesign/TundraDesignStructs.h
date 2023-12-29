#pragma once

#include "TundraDesignStructs.generated.h"

USTRUCT(BlueprintType)
struct FTundraDesignPlayer
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="TundraDesign")
	FString PlayerId;

	UPROPERTY(BlueprintReadOnly, Category="TundraDesign")
	FString SocialId;

	UPROPERTY(BlueprintReadOnly, Category="TundraDesign")
	FString Username;
};
