// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/Dto.h.mustache
// </auto-generated>
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Dto/PragmaTypesDto.h"
#include "Dto/PragmaOptionsDto.h"
#include "Dto/PragmaInventoryExtDto.h"
#include "Dto/PragmaMissionsDto.h"
#include "Dto/PragmaPetDto.h"

#include "PragmaInventoryRpcExtDto.generated.h"

// Forward declarations.
struct FPragma_Inventory_ExtCraftRequest;
struct FPragma_Inventory_TrackingItemUpdate;
struct FPragma_Inventory_ExtInstancedItemServerUpdate;
struct FPragma_Inventory_ExtInstancedItemUpdate;
struct FPragma_Inventory_PetEvolution;
struct FPragma_Inventory_ExtInstancedItemServerGrant;
struct FPragma_Inventory_ExtPurchaseRequest;

//
// Source protos:
// shared/inventoryRpcExt.proto
//


UENUM(BlueprintType, Category=Pragma)
enum class EPragma_Inventory_ExtCraftRequest_DataType : uint8
{
	Invalid,
	PetEvolutionRequest,
};

USTRUCT(BlueprintType, Category=Pragma)
struct PRAGMASDK_API FPragma_Inventory_ExtCraftRequest_Data
{
	GENERATED_BODY()

	FPragma_Inventory_ExtCraftRequest_Data();
	~FPragma_Inventory_ExtCraftRequest_Data();

	FPragma_Inventory_ExtCraftRequest_Data(const FPragma_Inventory_ExtCraftRequest_Data& Other);
	FPragma_Inventory_ExtCraftRequest_Data& operator=(const FPragma_Inventory_ExtCraftRequest_Data& Other);

    bool Equals(const FPragma_Inventory_ExtCraftRequest_Data& Other) const;

	void Reset();

	EPragma_Inventory_ExtCraftRequest_DataType OneOfType() const { return Type; }

	const FPragma_Inventory_PetEvolutionRequest& PetEvolutionRequest() const;

    void SetPetEvolutionRequest(const FPragma_Inventory_PetEvolutionRequest& Value);

	static bool SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
	static bool DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);

private:
	UPROPERTY()
	EPragma_Inventory_ExtCraftRequest_DataType Type { EPragma_Inventory_ExtCraftRequest_DataType::Invalid };

	union
	{
		TTypeCompatibleBytes<FPragma_Inventory_PetEvolutionRequest> PetEvolutionRequest;
	} Union{};
};
bool operator==(const FPragma_Inventory_ExtCraftRequest_Data& Lhs, const FPragma_Inventory_ExtCraftRequest_Data& Rhs);
bool operator!=(const FPragma_Inventory_ExtCraftRequest_Data& Lhs, const FPragma_Inventory_ExtCraftRequest_Data& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_ExtCraftRequest
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	FPragma_Inventory_ExtCraftRequest_Data Data;

    PRAGMASDK_API static bool AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
    PRAGMASDK_API static bool AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);
};
PRAGMASDK_API bool operator==(const FPragma_Inventory_ExtCraftRequest& Lhs, const FPragma_Inventory_ExtCraftRequest& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_ExtCraftRequest& Lhs, const FPragma_Inventory_ExtCraftRequest& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_TrackingItemUpdate
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString InstanceIdToTrack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString CatalogIdToTrack;

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_TrackingItemUpdate& Lhs, const FPragma_Inventory_TrackingItemUpdate& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_TrackingItemUpdate& Lhs, const FPragma_Inventory_TrackingItemUpdate& Rhs);

UENUM(BlueprintType, Category=Pragma)
enum class EPragma_Inventory_ExtInstancedItemServerUpdate_DataType : uint8
{
	Invalid,
	ActiveMissionsUpdate,
	PetUpdate,
	TrackingItemUpdate,
};

USTRUCT(BlueprintType, Category=Pragma)
struct PRAGMASDK_API FPragma_Inventory_ExtInstancedItemServerUpdate_Data
{
	GENERATED_BODY()

	FPragma_Inventory_ExtInstancedItemServerUpdate_Data();
	~FPragma_Inventory_ExtInstancedItemServerUpdate_Data();

	FPragma_Inventory_ExtInstancedItemServerUpdate_Data(const FPragma_Inventory_ExtInstancedItemServerUpdate_Data& Other);
	FPragma_Inventory_ExtInstancedItemServerUpdate_Data& operator=(const FPragma_Inventory_ExtInstancedItemServerUpdate_Data& Other);

    bool Equals(const FPragma_Inventory_ExtInstancedItemServerUpdate_Data& Other) const;

	void Reset();

	EPragma_Inventory_ExtInstancedItemServerUpdate_DataType OneOfType() const { return Type; }

	const FPragma_Ext_ActiveMissionsUpdate& ActiveMissionsUpdate() const;
	const FPragma_Inventory_PetUpdate& PetUpdate() const;
	const FPragma_Inventory_TrackingItemUpdate& TrackingItemUpdate() const;

    void SetActiveMissionsUpdate(const FPragma_Ext_ActiveMissionsUpdate& Value);
    void SetPetUpdate(const FPragma_Inventory_PetUpdate& Value);
    void SetTrackingItemUpdate(const FPragma_Inventory_TrackingItemUpdate& Value);

	static bool SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
	static bool DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);

private:
	UPROPERTY()
	EPragma_Inventory_ExtInstancedItemServerUpdate_DataType Type { EPragma_Inventory_ExtInstancedItemServerUpdate_DataType::Invalid };

	union
	{
		TTypeCompatibleBytes<FPragma_Ext_ActiveMissionsUpdate> ActiveMissionsUpdate;
		TTypeCompatibleBytes<FPragma_Inventory_PetUpdate> PetUpdate;
		TTypeCompatibleBytes<FPragma_Inventory_TrackingItemUpdate> TrackingItemUpdate;
	} Union{};
};
bool operator==(const FPragma_Inventory_ExtInstancedItemServerUpdate_Data& Lhs, const FPragma_Inventory_ExtInstancedItemServerUpdate_Data& Rhs);
bool operator!=(const FPragma_Inventory_ExtInstancedItemServerUpdate_Data& Lhs, const FPragma_Inventory_ExtInstancedItemServerUpdate_Data& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_ExtInstancedItemServerUpdate
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	FPragma_Inventory_ExtInstancedItemServerUpdate_Data Data;

    PRAGMASDK_API static bool AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
    PRAGMASDK_API static bool AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);
};
PRAGMASDK_API bool operator==(const FPragma_Inventory_ExtInstancedItemServerUpdate& Lhs, const FPragma_Inventory_ExtInstancedItemServerUpdate& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_ExtInstancedItemServerUpdate& Lhs, const FPragma_Inventory_ExtInstancedItemServerUpdate& Rhs);

UENUM(BlueprintType, Category=Pragma)
enum class EPragma_Inventory_ExtInstancedItemUpdate_DataType : uint8
{
	Invalid,
	PetUpdate,
};

USTRUCT(BlueprintType, Category=Pragma)
struct PRAGMASDK_API FPragma_Inventory_ExtInstancedItemUpdate_Data
{
	GENERATED_BODY()

	FPragma_Inventory_ExtInstancedItemUpdate_Data();
	~FPragma_Inventory_ExtInstancedItemUpdate_Data();

	FPragma_Inventory_ExtInstancedItemUpdate_Data(const FPragma_Inventory_ExtInstancedItemUpdate_Data& Other);
	FPragma_Inventory_ExtInstancedItemUpdate_Data& operator=(const FPragma_Inventory_ExtInstancedItemUpdate_Data& Other);

    bool Equals(const FPragma_Inventory_ExtInstancedItemUpdate_Data& Other) const;

	void Reset();

	EPragma_Inventory_ExtInstancedItemUpdate_DataType OneOfType() const { return Type; }

	const FPragma_Inventory_PetUpdate& PetUpdate() const;

    void SetPetUpdate(const FPragma_Inventory_PetUpdate& Value);

	static bool SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
	static bool DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);

private:
	UPROPERTY()
	EPragma_Inventory_ExtInstancedItemUpdate_DataType Type { EPragma_Inventory_ExtInstancedItemUpdate_DataType::Invalid };

	union
	{
		TTypeCompatibleBytes<FPragma_Inventory_PetUpdate> PetUpdate;
	} Union{};
};
bool operator==(const FPragma_Inventory_ExtInstancedItemUpdate_Data& Lhs, const FPragma_Inventory_ExtInstancedItemUpdate_Data& Rhs);
bool operator!=(const FPragma_Inventory_ExtInstancedItemUpdate_Data& Lhs, const FPragma_Inventory_ExtInstancedItemUpdate_Data& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_ExtInstancedItemUpdate
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	int32 Reserved{0};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<FString> Tags;

	UPROPERTY(Transient)
	FPragma_Inventory_ExtInstancedItemUpdate_Data Data;

    PRAGMASDK_API static bool AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
    PRAGMASDK_API static bool AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);
};
PRAGMASDK_API bool operator==(const FPragma_Inventory_ExtInstancedItemUpdate& Lhs, const FPragma_Inventory_ExtInstancedItemUpdate& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_ExtInstancedItemUpdate& Lhs, const FPragma_Inventory_ExtInstancedItemUpdate& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_PetEvolution
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FPragma_Inventory_Pet Pet;

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_PetEvolution& Lhs, const FPragma_Inventory_PetEvolution& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_PetEvolution& Lhs, const FPragma_Inventory_PetEvolution& Rhs);

UENUM(BlueprintType, Category=Pragma)
enum class EPragma_Inventory_ExtInstancedItemServerGrant_DataType : uint8
{
	Invalid,
	PetEvolution,
	IncompleteFood,
};

USTRUCT(BlueprintType, Category=Pragma)
struct PRAGMASDK_API FPragma_Inventory_ExtInstancedItemServerGrant_Data
{
	GENERATED_BODY()

	FPragma_Inventory_ExtInstancedItemServerGrant_Data();
	~FPragma_Inventory_ExtInstancedItemServerGrant_Data();

	FPragma_Inventory_ExtInstancedItemServerGrant_Data(const FPragma_Inventory_ExtInstancedItemServerGrant_Data& Other);
	FPragma_Inventory_ExtInstancedItemServerGrant_Data& operator=(const FPragma_Inventory_ExtInstancedItemServerGrant_Data& Other);

    bool Equals(const FPragma_Inventory_ExtInstancedItemServerGrant_Data& Other) const;

	void Reset();

	EPragma_Inventory_ExtInstancedItemServerGrant_DataType OneOfType() const { return Type; }

	const FPragma_Inventory_PetEvolution& PetEvolution() const;
	const FPragma_Inventory_IncompleteFood& IncompleteFood() const;

    void SetPetEvolution(const FPragma_Inventory_PetEvolution& Value);
    void SetIncompleteFood(const FPragma_Inventory_IncompleteFood& Value);

	static bool SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
	static bool DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);

private:
	UPROPERTY()
	EPragma_Inventory_ExtInstancedItemServerGrant_DataType Type { EPragma_Inventory_ExtInstancedItemServerGrant_DataType::Invalid };

	union
	{
		TTypeCompatibleBytes<FPragma_Inventory_PetEvolution> PetEvolution;
		TTypeCompatibleBytes<FPragma_Inventory_IncompleteFood> IncompleteFood;
	} Union{};
};
bool operator==(const FPragma_Inventory_ExtInstancedItemServerGrant_Data& Lhs, const FPragma_Inventory_ExtInstancedItemServerGrant_Data& Rhs);
bool operator!=(const FPragma_Inventory_ExtInstancedItemServerGrant_Data& Lhs, const FPragma_Inventory_ExtInstancedItemServerGrant_Data& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_ExtInstancedItemServerGrant
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	FPragma_Inventory_ExtInstancedItemServerGrant_Data Data;

    PRAGMASDK_API static bool AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
    PRAGMASDK_API static bool AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);
};
PRAGMASDK_API bool operator==(const FPragma_Inventory_ExtInstancedItemServerGrant& Lhs, const FPragma_Inventory_ExtInstancedItemServerGrant& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_ExtInstancedItemServerGrant& Lhs, const FPragma_Inventory_ExtInstancedItemServerGrant& Rhs);

UENUM(BlueprintType, Category=Pragma)
enum class EPragma_Inventory_ExtPurchaseRequest_DataType : uint8
{
	Invalid,
	PetBonus,
};

USTRUCT(BlueprintType, Category=Pragma)
struct PRAGMASDK_API FPragma_Inventory_ExtPurchaseRequest_Data
{
	GENERATED_BODY()

	FPragma_Inventory_ExtPurchaseRequest_Data();
	~FPragma_Inventory_ExtPurchaseRequest_Data();

	FPragma_Inventory_ExtPurchaseRequest_Data(const FPragma_Inventory_ExtPurchaseRequest_Data& Other);
	FPragma_Inventory_ExtPurchaseRequest_Data& operator=(const FPragma_Inventory_ExtPurchaseRequest_Data& Other);

    bool Equals(const FPragma_Inventory_ExtPurchaseRequest_Data& Other) const;

	void Reset();

	EPragma_Inventory_ExtPurchaseRequest_DataType OneOfType() const { return Type; }

	const EPragma_Inventory_PetBonus& PetBonus() const;

    void SetPetBonus(const EPragma_Inventory_PetBonus& Value);

	static bool SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
	static bool DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);

private:
	UPROPERTY()
	EPragma_Inventory_ExtPurchaseRequest_DataType Type { EPragma_Inventory_ExtPurchaseRequest_DataType::Invalid };

	union
	{
		TTypeCompatibleBytes<EPragma_Inventory_PetBonus> PetBonus;
	} Union{};
};
bool operator==(const FPragma_Inventory_ExtPurchaseRequest_Data& Lhs, const FPragma_Inventory_ExtPurchaseRequest_Data& Rhs);
bool operator!=(const FPragma_Inventory_ExtPurchaseRequest_Data& Lhs, const FPragma_Inventory_ExtPurchaseRequest_Data& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_ExtPurchaseRequest
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	FPragma_Inventory_ExtPurchaseRequest_Data Data;

    PRAGMASDK_API static bool AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
    PRAGMASDK_API static bool AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);
};
PRAGMASDK_API bool operator==(const FPragma_Inventory_ExtPurchaseRequest& Lhs, const FPragma_Inventory_ExtPurchaseRequest& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_ExtPurchaseRequest& Lhs, const FPragma_Inventory_ExtPurchaseRequest& Rhs);
