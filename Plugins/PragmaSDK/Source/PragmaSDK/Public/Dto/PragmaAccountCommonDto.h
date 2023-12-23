// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/Dto.h.mustache
// </auto-generated>
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Dto/PragmaTypesDto.h"
#include "Dto/PragmaOptionsDto.h"

#include "PragmaAccountCommonDto.generated.h"

// Forward declarations.
struct FPragma_Account_DisplayName;
struct FPragma_Account_PragmaAccountOverview;

//
// Source protos:
// pragma/account/accountCommon.proto
//


USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Account_DisplayName
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString DisplayName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString Discriminator;

};
PRAGMASDK_API bool operator==(const FPragma_Account_DisplayName& Lhs, const FPragma_Account_DisplayName& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Account_DisplayName& Lhs, const FPragma_Account_DisplayName& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Account_PragmaAccountOverview
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString PragmaSocialId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FPragma_Account_DisplayName DisplayName;

};
PRAGMASDK_API bool operator==(const FPragma_Account_PragmaAccountOverview& Lhs, const FPragma_Account_PragmaAccountOverview& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Account_PragmaAccountOverview& Lhs, const FPragma_Account_PragmaAccountOverview& Rhs);

