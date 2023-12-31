// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/Dto.h.mustache
// </auto-generated>
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"

#include "PragmaPartyRpcExtDto.generated.h"

// Forward declarations.
struct FPragma_Party_ExtPrivatePlayer;
struct FPragma_Party_ExtBroadcastPlayer;
struct FPragma_Party_ExtBroadcastParty;
struct FPragma_Party_ExtUpdatePartyRequest;
struct FPragma_Party_ExtPlayerJoinRequest;
struct FPragma_Party_ExtCreateRequest;
struct FPragma_Party_ExtUpdatePartyPlayerRequest;

// ************************************************************
// *** THIS FILE WAS STUBBED FOR THIS TUNDRA DESIGN PROJECT ***
// ************************************************************

//
// Source protos:
// shared/partyRpcExt.proto
//


//
// Enums.
//

UENUM(BlueprintType, Category=Pragma)
enum class EPragma_Party_GameMode : uint8
{
	GAME_MODE_UNSPECIFIED = 0,
	NORMAL = 1,
	RANKED = 2,
};

//
// Messages.
//

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Party_ExtPrivatePlayer
{
	GENERATED_BODY()
};
PRAGMASDK_API bool operator==(const FPragma_Party_ExtPrivatePlayer& Lhs, const FPragma_Party_ExtPrivatePlayer& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Party_ExtPrivatePlayer& Lhs, const FPragma_Party_ExtPrivatePlayer& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Party_ExtBroadcastPlayer
{
	GENERATED_BODY()
};
PRAGMASDK_API bool operator==(const FPragma_Party_ExtBroadcastPlayer& Lhs, const FPragma_Party_ExtBroadcastPlayer& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Party_ExtBroadcastPlayer& Lhs, const FPragma_Party_ExtBroadcastPlayer& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Party_ExtBroadcastParty
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	EPragma_Party_GameMode GameMode{static_cast<EPragma_Party_GameMode>(0)};
};
PRAGMASDK_API bool operator==(const FPragma_Party_ExtBroadcastParty& Lhs, const FPragma_Party_ExtBroadcastParty& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Party_ExtBroadcastParty& Lhs, const FPragma_Party_ExtBroadcastParty& Rhs);

UENUM(BlueprintType, Category=Pragma)
enum class EPragma_Party_ExtUpdatePartyRequest_UpdateType : uint8
{
	Invalid,
	NewGameMode,
};

USTRUCT(BlueprintType, Category=Pragma)
struct PRAGMASDK_API FPragma_Party_ExtUpdatePartyRequest_Update
{
	GENERATED_BODY()

	FPragma_Party_ExtUpdatePartyRequest_Update();
	~FPragma_Party_ExtUpdatePartyRequest_Update();

	FPragma_Party_ExtUpdatePartyRequest_Update(const FPragma_Party_ExtUpdatePartyRequest_Update& Other);
	FPragma_Party_ExtUpdatePartyRequest_Update& operator=(const FPragma_Party_ExtUpdatePartyRequest_Update& Other);

    bool Equals(const FPragma_Party_ExtUpdatePartyRequest_Update& Other) const;

	void Reset();

	EPragma_Party_ExtUpdatePartyRequest_UpdateType OneOfType() const { return Type; }

	const EPragma_Party_GameMode& NewGameMode() const;
	const int64& GameDurationInMillis() const;
	const bool& ShouldCauseMatchmakingException() const;

    void SetNewGameMode(const EPragma_Party_GameMode& Value);

	static bool SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
	static bool DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);

private:
	UPROPERTY()
	EPragma_Party_ExtUpdatePartyRequest_UpdateType Type { EPragma_Party_ExtUpdatePartyRequest_UpdateType::Invalid };

	union
	{
		TTypeCompatibleBytes<EPragma_Party_GameMode> NewGameMode;
	} Union{};
};
bool operator==(const FPragma_Party_ExtUpdatePartyRequest_Update& Lhs, const FPragma_Party_ExtUpdatePartyRequest_Update& Rhs);
bool operator!=(const FPragma_Party_ExtUpdatePartyRequest_Update& Lhs, const FPragma_Party_ExtUpdatePartyRequest_Update& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Party_ExtUpdatePartyRequest
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	FPragma_Party_ExtUpdatePartyRequest_Update Update;

    PRAGMASDK_API static bool AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
    PRAGMASDK_API static bool AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);
};
PRAGMASDK_API bool operator==(const FPragma_Party_ExtUpdatePartyRequest& Lhs, const FPragma_Party_ExtUpdatePartyRequest& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Party_ExtUpdatePartyRequest& Lhs, const FPragma_Party_ExtUpdatePartyRequest& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Party_ExtPlayerJoinRequest
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_Party_ExtPlayerJoinRequest& Lhs, const FPragma_Party_ExtPlayerJoinRequest& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Party_ExtPlayerJoinRequest& Lhs, const FPragma_Party_ExtPlayerJoinRequest& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Party_ExtCreateRequest
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	EPragma_Party_GameMode GameMode;

};
PRAGMASDK_API bool operator==(const FPragma_Party_ExtCreateRequest& Lhs, const FPragma_Party_ExtCreateRequest& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Party_ExtCreateRequest& Lhs, const FPragma_Party_ExtCreateRequest& Rhs);

UENUM(BlueprintType, Category=Pragma)
enum class EPragma_Party_ExtUpdatePartyPlayerRequest_UpdateType : uint8
{
	Invalid,
};

USTRUCT(BlueprintType, Category=Pragma)
struct PRAGMASDK_API FPragma_Party_ExtUpdatePartyPlayerRequest_Update
{
	GENERATED_BODY()

	FPragma_Party_ExtUpdatePartyPlayerRequest_Update();
	~FPragma_Party_ExtUpdatePartyPlayerRequest_Update();

	FPragma_Party_ExtUpdatePartyPlayerRequest_Update(const FPragma_Party_ExtUpdatePartyPlayerRequest_Update& Other);
	FPragma_Party_ExtUpdatePartyPlayerRequest_Update& operator=(const FPragma_Party_ExtUpdatePartyPlayerRequest_Update& Other);

    bool Equals(const FPragma_Party_ExtUpdatePartyPlayerRequest_Update& Other) const;

	void Reset();

	EPragma_Party_ExtUpdatePartyPlayerRequest_UpdateType OneOfType() const { return Type; }

	static bool SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
	static bool DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);

private:
	UPROPERTY()
	EPragma_Party_ExtUpdatePartyPlayerRequest_UpdateType Type { EPragma_Party_ExtUpdatePartyPlayerRequest_UpdateType::Invalid };

	union
	{
	} Union{};
};
bool operator==(const FPragma_Party_ExtUpdatePartyPlayerRequest_Update& Lhs, const FPragma_Party_ExtUpdatePartyPlayerRequest_Update& Rhs);
bool operator!=(const FPragma_Party_ExtUpdatePartyPlayerRequest_Update& Lhs, const FPragma_Party_ExtUpdatePartyPlayerRequest_Update& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Party_ExtUpdatePartyPlayerRequest
{
	GENERATED_BODY()

	UPROPERTY()
	int64 CurrentInventoryVersion{0};

	UPROPERTY(Transient)
	FPragma_Party_ExtUpdatePartyPlayerRequest_Update Update;

    PRAGMASDK_API static bool AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
    PRAGMASDK_API static bool AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);
};
PRAGMASDK_API bool operator==(const FPragma_Party_ExtUpdatePartyPlayerRequest& Lhs, const FPragma_Party_ExtUpdatePartyPlayerRequest& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Party_ExtUpdatePartyPlayerRequest& Lhs, const FPragma_Party_ExtUpdatePartyPlayerRequest& Rhs);

