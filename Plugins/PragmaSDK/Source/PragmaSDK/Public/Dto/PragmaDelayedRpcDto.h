// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/Dto.h.mustache
// </auto-generated>
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Dto/PragmaOptionsDto.h"

#include "PragmaDelayedRpcDto.generated.h"

// Forward declarations.
struct FPragma_Delayed_DelayedV1Response;
struct FPragma_Delayed_DelayedV1Request;

//
// Source protos:
// demo/stress/delayedRpc.proto
//


USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Delayed_DelayedV1Response
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	int32 DelayedFor{0};

};
PRAGMASDK_API bool operator==(const FPragma_Delayed_DelayedV1Response& Lhs, const FPragma_Delayed_DelayedV1Response& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Delayed_DelayedV1Response& Lhs, const FPragma_Delayed_DelayedV1Response& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Delayed_DelayedV1Request
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_Delayed_DelayedV1Request& Lhs, const FPragma_Delayed_DelayedV1Request& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Delayed_DelayedV1Request& Lhs, const FPragma_Delayed_DelayedV1Request& Rhs);
