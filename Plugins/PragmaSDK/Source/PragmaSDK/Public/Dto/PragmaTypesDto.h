// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/Dto.h.mustache
// </auto-generated>
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Dto/PragmaOptionsDto.h"

#include "PragmaTypesDto.generated.h"

// Forward declarations.
struct FPragma_Services;
struct FPragma_IntAny;
struct FPragma_Fixed128;
struct FPragma_ServiceInstance;
struct FPragma_ServicesInstances;

//
// Source protos:
// pragma/types.proto
//


USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Services
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<FString> Services;

};
PRAGMASDK_API bool operator==(const FPragma_Services& Lhs, const FPragma_Services& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Services& Lhs, const FPragma_Services& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_IntAny
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	int32 Type{0};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString Bytes;

};
PRAGMASDK_API bool operator==(const FPragma_IntAny& Lhs, const FPragma_IntAny& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_IntAny& Lhs, const FPragma_IntAny& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Fixed128
{
	GENERATED_BODY()

	UPROPERTY()
	uint64 Lsb{0};

	UPROPERTY()
	uint64 Msb{0};

};
PRAGMASDK_API bool operator==(const FPragma_Fixed128& Lhs, const FPragma_Fixed128& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Fixed128& Lhs, const FPragma_Fixed128& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_ServiceInstance
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString InstanceId;

};
PRAGMASDK_API bool operator==(const FPragma_ServiceInstance& Lhs, const FPragma_ServiceInstance& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_ServiceInstance& Lhs, const FPragma_ServiceInstance& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_ServicesInstances
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<FPragma_ServiceInstance> Instances;

};
PRAGMASDK_API bool operator==(const FPragma_ServicesInstances& Lhs, const FPragma_ServicesInstances& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_ServicesInstances& Lhs, const FPragma_ServicesInstances& Rhs);

