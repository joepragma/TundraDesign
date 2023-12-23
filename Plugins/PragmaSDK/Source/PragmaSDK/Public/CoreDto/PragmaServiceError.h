#pragma once

#include "CoreMinimal.h"

#include "Dto/PragmaErrorsDto.h"
#include "Dto/PragmaErrorsExtDto.h"
#include "Dto/PragmaResponseDto.h"

#include "PragmaServiceError.generated.h"


USTRUCT()
struct FPragmaServiceError
{
	GENERATED_BODY()

	// Client-side tracking of request to response.
	UPROPERTY()
	int32 RequestId{0};

	UPROPERTY()
	EPragma_ResponseStatusCode Status{EPragma_ResponseStatusCode::ERROR_UNKNOWN};

	UPROPERTY()
	EPragma_PlatformError Error{EPragma_PlatformError::UNKNOWN_ERROR};

	UPROPERTY()
	EPragma_ExtError ExtError{EPragma_ExtError::UNKNOWN_EXT_ERROR};

	// UUID used for tracing e.g. in the backend logs.
	UPROPERTY()
	FString TraceUuid;

	// Debug details only filled out in non-prod builds.
	UPROPERTY()
	TArray<FString> DebugDetails;
};