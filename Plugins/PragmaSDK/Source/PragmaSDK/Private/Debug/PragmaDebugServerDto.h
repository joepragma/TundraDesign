#pragma once
#include "Dto/PragmaCoreDto.h"
#include "PragmaTypes.h"

#include "PragmaDebugServerDto.generated.h"

//
// DTOs for clients communicating with the DebugServer hosted by the Unreal client.
//

UENUM(BlueprintType, Category=Pragma)
enum class EPragmaDebugMessageInfoState : uint8
{
	Pending,
	ProcessedSuccess,
	ProcessedFailure
};

//
// Information on a request the Unreal client has sent to the Pragma backend.
//
USTRUCT()
struct FPragmaDebugRequestInfo
{
	GENERATED_BODY()

	UPROPERTY()
	int64 Timestamp{0};

	UPROPERTY()
	EPragmaBackendType BackendType{EPragmaBackendType::Game};
	
	UPROPERTY()
	EPragmaProtocolType ProtocolType{EPragmaProtocolType::Http};

	UPROPERTY()
	FString RequestTypeName;

	UPROPERTY()
	FString ResponseTypeName;
	
	UPROPERTY()
	EPragmaDebugMessageInfoState State{EPragmaDebugMessageInfoState::Pending};

	// This is stored as a json object and will be injected as "message" instead of a string to avoid escaped json.
	TSharedPtr<FJsonObject> Message;
};

USTRUCT()
struct FPragmaDebugMessageInfo
{
	GENERATED_BODY()

	UPROPERTY()
	int64 Timestamp{0};

	UPROPERTY()
	int32 SequenceNumber{0};

	UPROPERTY()
	FString ProcessingError;

	UPROPERTY()
	EPragmaDebugMessageInfoState State{EPragmaDebugMessageInfoState::Pending};

	// If this exists, it will be injected as the "message" during serialization.
	TSharedPtr<FJsonObject> Message;
};

USTRUCT()
struct FPragmaDebugRequests
{
	GENERATED_BODY()

	// Note that selected requests will be serialized here as "requests": [...].
	// They are manually injected into the json object so we don't need to escape the json.
};

USTRUCT()
struct FPragmaDebugServerMessages
{
	GENERATED_BODY()

	// Note that selected requests will be serialized here as "messages": [...].
	// They are manually injected into the json object so we don't need to escape the json.
};
