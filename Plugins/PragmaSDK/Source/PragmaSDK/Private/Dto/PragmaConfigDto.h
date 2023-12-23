#pragma once

#include "CoreMinimal.h"

#include "PragmaConfigDto.generated.h"

//
// DTOs for configuration endpoints.
//

USTRUCT()
struct FPragmaBackendInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FString Host;
	
	UPROPERTY()
	int32 Port{0};
	
	UPROPERTY()
	FString Protocol;
	
	UPROPERTY()
	FString WebSocketProtocol;
};

USTRUCT()
struct FPragmaInfoV1Response
{
	GENERATED_BODY()

	UPROPERTY()
	FString Name;

	UPROPERTY()
	FString Version;
	
	UPROPERTY()
	FString BackendMode;

	UPROPERTY()
    FPragmaBackendInfo AuthenticateBackend;

    UPROPERTY()
    TMap<FString, FPragmaBackendInfo> Backends;
	
	UPROPERTY()
	FPragmaBackendInfo SocialBackend;
	
	UPROPERTY()
	FPragmaBackendInfo GameBackend;

	UPROPERTY()
	FString GameShardId;
};

USTRUCT()
struct FPragmaTypesV1Response
{
	GENERATED_BODY()

	// The body of this response is actually a key-value map of typename->int, so we manually process it as a json object.
};

USTRUCT()
struct FSdkCoreInfo
{
	GENERATED_BODY()

	UPROPERTY()
	bool Enabled{false};
};
