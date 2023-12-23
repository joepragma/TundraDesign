// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/Dto.h.mustache
// </auto-generated>
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Dto/PragmaAccountCommonDto.h"
#include "Dto/PragmaTypesDto.h"
#include "Dto/PragmaOptionsDto.h"

#include "PragmaSessionCommonDto.generated.h"

// Forward declarations.
struct FPragma_PartnerSession;
struct FPragma_PlayerSession;

//
// Source protos:
// pragma/session/sessionCommon.proto
//


//
// Enums.
//

UENUM(BlueprintType, Category=Pragma)
enum class EPragma_SessionTerminationReason : uint8
{
	UNKNOWN_SESSION_TERMINATION_REASON = 0,
	AS_REQUESTED = 1,
	CLUSTER_SHUTDOWN = 2,
	KICKED = 3,
	USURPED = 4,
	EXPIRED = 5,
};

//
// Messages.
//

//
// Enums.
//

UENUM(BlueprintType, Category=Pragma)
enum class EPragma_GameSessionAttribute : uint8
{
	UNKNOWN_GAME_ATTRIBUTE = 0,
	ENABLE_GAME_RECONNECT = 1,
	PARTY_ID = 2,
	MATCHMAKING_ID = 3,
	GAME_INSTANCE_ID = 4,
};

//
// Messages.
//

//
// Enums.
//

UENUM(BlueprintType, Category=Pragma)
enum class EPragma_SocialSessionAttribute : uint8
{
	UNKNOWN_SOCIAL_ATTRIBUTE = 0,
	ENABLE_SOCIAL_RECONNECT = 1,
};

//
// Messages.
//

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PartnerSession
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString PragmaId;

};
PRAGMASDK_API bool operator==(const FPragma_PartnerSession& Lhs, const FPragma_PartnerSession& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PartnerSession& Lhs, const FPragma_PartnerSession& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerSession
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString PragmaId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString SessionId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FPragma_Account_DisplayName DisplayName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TMap<int32, FString> Attributes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	int32 Version{0};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString SocialId;

};
PRAGMASDK_API bool operator==(const FPragma_PlayerSession& Lhs, const FPragma_PlayerSession& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerSession& Lhs, const FPragma_PlayerSession& Rhs);
