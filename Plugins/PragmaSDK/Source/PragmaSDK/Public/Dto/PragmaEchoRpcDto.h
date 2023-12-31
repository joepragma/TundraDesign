// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/Dto.h.mustache
// </auto-generated>
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Dto/PragmaOptionsDto.h"

#include "PragmaEchoRpcDto.generated.h"

// Forward declarations.
struct FPragma_Echo_EchoV1Request;
struct FPragma_Echo_EchoV1Response;

//
// Source protos:
// demo/echo/echoRpc.proto
//


USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Echo_EchoV1Request
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString Message;

};
PRAGMASDK_API bool operator==(const FPragma_Echo_EchoV1Request& Lhs, const FPragma_Echo_EchoV1Request& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Echo_EchoV1Request& Lhs, const FPragma_Echo_EchoV1Request& Rhs);



USTRUCT(BlueprintType, Category=Pragma)
struct PRAGMASDK_API FPragma_Echo_EchoV1Response_Artifact
{
	GENERATED_BODY()

	FPragma_Echo_EchoV1Response_Artifact();
	~FPragma_Echo_EchoV1Response_Artifact();

	FPragma_Echo_EchoV1Response_Artifact(const FPragma_Echo_EchoV1Response_Artifact& Other);
	FPragma_Echo_EchoV1Response_Artifact& operator=(const FPragma_Echo_EchoV1Response_Artifact& Other);

	bool Equals(const FPragma_Echo_EchoV1Response_Artifact& Other) const;

	bool HasValue() const { return ValuePtr != nullptr; }
	void Reset();

	const FPragma_Echo_EchoV1Request& Value() const;

	void SetValue(const FPragma_Echo_EchoV1Request& Value);

	static bool SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
	static bool DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);

private:
	TUniquePtr<FPragma_Echo_EchoV1Request> ValuePtr;
};
bool operator==(const FPragma_Echo_EchoV1Response_Artifact& Lhs, const FPragma_Echo_EchoV1Response_Artifact& Rhs);
bool operator!=(const FPragma_Echo_EchoV1Response_Artifact& Lhs, const FPragma_Echo_EchoV1Response_Artifact& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Echo_EchoV1Response
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString ResponseMessage;

	UPROPERTY(Transient)
	FPragma_Echo_EchoV1Response_Artifact Artifact;

    PRAGMASDK_API static bool AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
    PRAGMASDK_API static bool AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);
};
PRAGMASDK_API bool operator==(const FPragma_Echo_EchoV1Response& Lhs, const FPragma_Echo_EchoV1Response& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Echo_EchoV1Response& Lhs, const FPragma_Echo_EchoV1Response& Rhs);

