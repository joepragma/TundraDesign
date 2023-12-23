// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/Dto.h.mustache
// </auto-generated>
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Dto/PragmaResponseDto.h"
#include "Dto/PragmaTypesDto.h"
#include "Dto/PragmaOptionsDto.h"

#include "PragmaExternalDto.generated.h"

// Forward declarations.
struct FPragma_ExternalRequest;
struct FPragma_ExternalNotification;
struct FPragma_ExternalResponse;
struct FPragma_ExternalMessage;

//
// Source protos:
// pragma/external.proto
//


USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_ExternalRequest
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	int32 RequestId{0};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	int32 Type{0};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString Payload;

};
PRAGMASDK_API bool operator==(const FPragma_ExternalRequest& Lhs, const FPragma_ExternalRequest& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_ExternalRequest& Lhs, const FPragma_ExternalRequest& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_ExternalNotification
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	int32 Type{0};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString Payload;

};
PRAGMASDK_API bool operator==(const FPragma_ExternalNotification& Lhs, const FPragma_ExternalNotification& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_ExternalNotification& Lhs, const FPragma_ExternalNotification& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_ExternalResponse
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	int32 RequestId{0};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString TraceId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	int32 Type{0};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString Payload;

};
PRAGMASDK_API bool operator==(const FPragma_ExternalResponse& Lhs, const FPragma_ExternalResponse& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_ExternalResponse& Lhs, const FPragma_ExternalResponse& Rhs);

UENUM(BlueprintType, Category=Pragma)
enum class EPragma_ExternalMessage_TypeType : uint8
{
	Invalid,
	Response,
	Notification,
	ServiceError,
};

USTRUCT(BlueprintType, Category=Pragma)
struct PRAGMASDK_API FPragma_ExternalMessage_Type
{
	GENERATED_BODY()

	FPragma_ExternalMessage_Type();
	~FPragma_ExternalMessage_Type();

	FPragma_ExternalMessage_Type(const FPragma_ExternalMessage_Type& Other);
	FPragma_ExternalMessage_Type& operator=(const FPragma_ExternalMessage_Type& Other);

    bool Equals(const FPragma_ExternalMessage_Type& Other) const;

	void Reset();

	EPragma_ExternalMessage_TypeType OneOfType() const { return Type; }

	const FPragma_ExternalResponse& Response() const;
	const FPragma_ExternalNotification& Notification() const;
	const FPragma_ServiceError& ServiceError() const;

    void SetResponse(const FPragma_ExternalResponse& Value);
    void SetNotification(const FPragma_ExternalNotification& Value);
    void SetServiceError(const FPragma_ServiceError& Value);

	static bool SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
	static bool DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);

private:
	UPROPERTY()
	EPragma_ExternalMessage_TypeType Type { EPragma_ExternalMessage_TypeType::Invalid };

	union
	{
		TTypeCompatibleBytes<FPragma_ExternalResponse> Response;
		TTypeCompatibleBytes<FPragma_ExternalNotification> Notification;
		TTypeCompatibleBytes<FPragma_ServiceError> ServiceError;
	} Union{};
};
bool operator==(const FPragma_ExternalMessage_Type& Lhs, const FPragma_ExternalMessage_Type& Rhs);
bool operator!=(const FPragma_ExternalMessage_Type& Lhs, const FPragma_ExternalMessage_Type& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_ExternalMessage
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	int32 SequenceNumber{0};

	UPROPERTY(Transient)
	FPragma_ExternalMessage_Type Type;

    PRAGMASDK_API static bool AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
    PRAGMASDK_API static bool AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);
};
PRAGMASDK_API bool operator==(const FPragma_ExternalMessage& Lhs, const FPragma_ExternalMessage& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_ExternalMessage& Lhs, const FPragma_ExternalMessage& Rhs);

