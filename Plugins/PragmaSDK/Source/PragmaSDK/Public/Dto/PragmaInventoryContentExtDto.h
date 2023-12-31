// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/Dto.h.mustache
// </auto-generated>
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Dto/PragmaOptionsDto.h"
#include "Dto/PragmaPlaystationDto.h"
#include "Dto/PragmaXboxDto.h"
#include "Dto/PragmaDemoGrantsDto.h"
#include "Dto/PragmaMissionsDto.h"
#include "Dto/PragmaPetDto.h"

#include "PragmaInventoryContentExtDto.generated.h"

// Forward declarations.
struct FPragma_Inventory_DemoCustomEntitlement;
struct FPragma_Inventory_ExtEntitlement;
struct FPragma_Inventory_MissionSpec;
struct FPragma_Inventory_DemoMissionItemUpdate;
struct FPragma_Inventory_DemoRewardGrant;
struct FPragma_Inventory_PetUpdateSpec;
struct FPragma_Inventory_ExtUpdateEntry;
struct FPragma_Inventory_CookingCompleteSpec;
struct FPragma_Inventory_CookingSpec;
struct FPragma_Inventory_PetEvolutionSpec;
struct FPragma_Inventory_ExtCraftingEntry;
struct FPragma_Inventory_CookingCompleteRequirements;
struct FPragma_Inventory_PetEvolutionRequirements;
struct FPragma_Inventory_MissionRequirements;
struct FPragma_Inventory_ExtPurchaseRequirements;
struct FPragma_Inventory_ExtStackableSpec;
struct FPragma_Inventory_TrackingSpec;
struct FPragma_Inventory_GearSpec;
struct FPragma_Inventory_MissionsSpec;
struct FPragma_Inventory_IncompleteFoodSpec;
struct FPragma_Inventory_ExtInstancedSpec;

//
// Source protos:
// shared/inventoryContentExt.proto
//


USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_DemoCustomEntitlement
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString Id;

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_DemoCustomEntitlement& Lhs, const FPragma_Inventory_DemoCustomEntitlement& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_DemoCustomEntitlement& Lhs, const FPragma_Inventory_DemoCustomEntitlement& Rhs);

UENUM(BlueprintType, Category=Pragma)
enum class EPragma_Inventory_ExtEntitlement_DataType : uint8
{
	Invalid,
	DemoCustomEntitlement,
	PlaystationEntitlement,
	XboxEntitlement,
};

USTRUCT(BlueprintType, Category=Pragma)
struct PRAGMASDK_API FPragma_Inventory_ExtEntitlement_Data
{
	GENERATED_BODY()

	FPragma_Inventory_ExtEntitlement_Data();
	~FPragma_Inventory_ExtEntitlement_Data();

	FPragma_Inventory_ExtEntitlement_Data(const FPragma_Inventory_ExtEntitlement_Data& Other);
	FPragma_Inventory_ExtEntitlement_Data& operator=(const FPragma_Inventory_ExtEntitlement_Data& Other);

    bool Equals(const FPragma_Inventory_ExtEntitlement_Data& Other) const;

	void Reset();

	EPragma_Inventory_ExtEntitlement_DataType OneOfType() const { return Type; }

	const FPragma_Inventory_DemoCustomEntitlement& DemoCustomEntitlement() const;
	const FPragma_Playstation_PlaystationEntitlement& PlaystationEntitlement() const;
	const FPragma_Xbox_XboxEntitlement& XboxEntitlement() const;

    void SetDemoCustomEntitlement(const FPragma_Inventory_DemoCustomEntitlement& Value);
    void SetPlaystationEntitlement(const FPragma_Playstation_PlaystationEntitlement& Value);
    void SetXboxEntitlement(const FPragma_Xbox_XboxEntitlement& Value);

	static bool SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
	static bool DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);

private:
	UPROPERTY()
	EPragma_Inventory_ExtEntitlement_DataType Type { EPragma_Inventory_ExtEntitlement_DataType::Invalid };

	union
	{
		TTypeCompatibleBytes<FPragma_Inventory_DemoCustomEntitlement> DemoCustomEntitlement;
		TTypeCompatibleBytes<FPragma_Playstation_PlaystationEntitlement> PlaystationEntitlement;
		TTypeCompatibleBytes<FPragma_Xbox_XboxEntitlement> XboxEntitlement;
	} Union{};
};
bool operator==(const FPragma_Inventory_ExtEntitlement_Data& Lhs, const FPragma_Inventory_ExtEntitlement_Data& Rhs);
bool operator!=(const FPragma_Inventory_ExtEntitlement_Data& Lhs, const FPragma_Inventory_ExtEntitlement_Data& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_ExtEntitlement
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	FPragma_Inventory_ExtEntitlement_Data Data;

    PRAGMASDK_API static bool AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
    PRAGMASDK_API static bool AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);
};
PRAGMASDK_API bool operator==(const FPragma_Inventory_ExtEntitlement& Lhs, const FPragma_Inventory_ExtEntitlement& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_ExtEntitlement& Lhs, const FPragma_Inventory_ExtEntitlement& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_MissionSpec
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TMap<FString, int64> Objectives;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<FPragma_Ext_DemoItemGrant> DemoItemGrants;

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_MissionSpec& Lhs, const FPragma_Inventory_MissionSpec& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_MissionSpec& Lhs, const FPragma_Inventory_MissionSpec& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_DemoMissionItemUpdate
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString CatalogId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FPragma_Ext_ActiveMissionsUpdate MissionsUpdate;

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_DemoMissionItemUpdate& Lhs, const FPragma_Inventory_DemoMissionItemUpdate& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_DemoMissionItemUpdate& Lhs, const FPragma_Inventory_DemoMissionItemUpdate& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_DemoRewardGrant
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString TableId;

	UPROPERTY()
	int64 Count{0};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<FString> Tags;

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_DemoRewardGrant& Lhs, const FPragma_Inventory_DemoRewardGrant& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_DemoRewardGrant& Lhs, const FPragma_Inventory_DemoRewardGrant& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_PetUpdateSpec
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<FPragma_Ext_DemoItemGrant> DemoItemGrants;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<FPragma_Inventory_DemoRewardGrant> DemoRewardGrants;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FPragma_Inventory_DemoMissionItemUpdate DemoMissionItemUpdate;

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_PetUpdateSpec& Lhs, const FPragma_Inventory_PetUpdateSpec& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_PetUpdateSpec& Lhs, const FPragma_Inventory_PetUpdateSpec& Rhs);

UENUM(BlueprintType, Category=Pragma)
enum class EPragma_Inventory_ExtUpdateEntry_DataType : uint8
{
	Invalid,
	PetUpdateSpec,
};

USTRUCT(BlueprintType, Category=Pragma)
struct PRAGMASDK_API FPragma_Inventory_ExtUpdateEntry_Data
{
	GENERATED_BODY()

	FPragma_Inventory_ExtUpdateEntry_Data();
	~FPragma_Inventory_ExtUpdateEntry_Data();

	FPragma_Inventory_ExtUpdateEntry_Data(const FPragma_Inventory_ExtUpdateEntry_Data& Other);
	FPragma_Inventory_ExtUpdateEntry_Data& operator=(const FPragma_Inventory_ExtUpdateEntry_Data& Other);

    bool Equals(const FPragma_Inventory_ExtUpdateEntry_Data& Other) const;

	void Reset();

	EPragma_Inventory_ExtUpdateEntry_DataType OneOfType() const { return Type; }

	const FPragma_Inventory_PetUpdateSpec& PetUpdateSpec() const;

    void SetPetUpdateSpec(const FPragma_Inventory_PetUpdateSpec& Value);

	static bool SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
	static bool DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);

private:
	UPROPERTY()
	EPragma_Inventory_ExtUpdateEntry_DataType Type { EPragma_Inventory_ExtUpdateEntry_DataType::Invalid };

	union
	{
		TTypeCompatibleBytes<FPragma_Inventory_PetUpdateSpec> PetUpdateSpec;
	} Union{};
};
bool operator==(const FPragma_Inventory_ExtUpdateEntry_Data& Lhs, const FPragma_Inventory_ExtUpdateEntry_Data& Rhs);
bool operator!=(const FPragma_Inventory_ExtUpdateEntry_Data& Lhs, const FPragma_Inventory_ExtUpdateEntry_Data& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_ExtUpdateEntry
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	FPragma_Inventory_ExtUpdateEntry_Data Data;

    PRAGMASDK_API static bool AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
    PRAGMASDK_API static bool AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);
};
PRAGMASDK_API bool operator==(const FPragma_Inventory_ExtUpdateEntry& Lhs, const FPragma_Inventory_ExtUpdateEntry& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_ExtUpdateEntry& Lhs, const FPragma_Inventory_ExtUpdateEntry& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_CookingCompleteSpec
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString CatalogIdToCreate;

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_CookingCompleteSpec& Lhs, const FPragma_Inventory_CookingCompleteSpec& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_CookingCompleteSpec& Lhs, const FPragma_Inventory_CookingCompleteSpec& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_CookingSpec
{
	GENERATED_BODY()

	UPROPERTY()
	int64 CookTimeInSeconds{0};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString CatalogIdToCreate;

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_CookingSpec& Lhs, const FPragma_Inventory_CookingSpec& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_CookingSpec& Lhs, const FPragma_Inventory_CookingSpec& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_PetEvolutionSpec
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString TargetPetCatalogId;

	UPROPERTY()
	int64 BonusIncrement{0};

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_PetEvolutionSpec& Lhs, const FPragma_Inventory_PetEvolutionSpec& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_PetEvolutionSpec& Lhs, const FPragma_Inventory_PetEvolutionSpec& Rhs);

UENUM(BlueprintType, Category=Pragma)
enum class EPragma_Inventory_ExtCraftingEntry_DataType : uint8
{
	Invalid,
	PetEvolutionSpec,
	CookingSpec,
	CookingCompleteSpec,
};

USTRUCT(BlueprintType, Category=Pragma)
struct PRAGMASDK_API FPragma_Inventory_ExtCraftingEntry_Data
{
	GENERATED_BODY()

	FPragma_Inventory_ExtCraftingEntry_Data();
	~FPragma_Inventory_ExtCraftingEntry_Data();

	FPragma_Inventory_ExtCraftingEntry_Data(const FPragma_Inventory_ExtCraftingEntry_Data& Other);
	FPragma_Inventory_ExtCraftingEntry_Data& operator=(const FPragma_Inventory_ExtCraftingEntry_Data& Other);

    bool Equals(const FPragma_Inventory_ExtCraftingEntry_Data& Other) const;

	void Reset();

	EPragma_Inventory_ExtCraftingEntry_DataType OneOfType() const { return Type; }

	const FPragma_Inventory_PetEvolutionSpec& PetEvolutionSpec() const;
	const FPragma_Inventory_CookingSpec& CookingSpec() const;
	const FPragma_Inventory_CookingCompleteSpec& CookingCompleteSpec() const;

    void SetPetEvolutionSpec(const FPragma_Inventory_PetEvolutionSpec& Value);
    void SetCookingSpec(const FPragma_Inventory_CookingSpec& Value);
    void SetCookingCompleteSpec(const FPragma_Inventory_CookingCompleteSpec& Value);

	static bool SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
	static bool DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);

private:
	UPROPERTY()
	EPragma_Inventory_ExtCraftingEntry_DataType Type { EPragma_Inventory_ExtCraftingEntry_DataType::Invalid };

	union
	{
		TTypeCompatibleBytes<FPragma_Inventory_PetEvolutionSpec> PetEvolutionSpec;
		TTypeCompatibleBytes<FPragma_Inventory_CookingSpec> CookingSpec;
		TTypeCompatibleBytes<FPragma_Inventory_CookingCompleteSpec> CookingCompleteSpec;
	} Union{};
};
bool operator==(const FPragma_Inventory_ExtCraftingEntry_Data& Lhs, const FPragma_Inventory_ExtCraftingEntry_Data& Rhs);
bool operator!=(const FPragma_Inventory_ExtCraftingEntry_Data& Lhs, const FPragma_Inventory_ExtCraftingEntry_Data& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_ExtCraftingEntry
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	FPragma_Inventory_ExtCraftingEntry_Data Data;

    PRAGMASDK_API static bool AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
    PRAGMASDK_API static bool AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);
};
PRAGMASDK_API bool operator==(const FPragma_Inventory_ExtCraftingEntry& Lhs, const FPragma_Inventory_ExtCraftingEntry& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_ExtCraftingEntry& Lhs, const FPragma_Inventory_ExtCraftingEntry& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_CookingCompleteRequirements
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString ConsumedCatalogId;

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_CookingCompleteRequirements& Lhs, const FPragma_Inventory_CookingCompleteRequirements& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_CookingCompleteRequirements& Lhs, const FPragma_Inventory_CookingCompleteRequirements& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_PetEvolutionRequirements
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString RequiredPetCatalogId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString RequiredPlayerLocation;

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_PetEvolutionRequirements& Lhs, const FPragma_Inventory_PetEvolutionRequirements& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_PetEvolutionRequirements& Lhs, const FPragma_Inventory_PetEvolutionRequirements& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_MissionRequirements
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<FString> Missions;

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_MissionRequirements& Lhs, const FPragma_Inventory_MissionRequirements& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_MissionRequirements& Lhs, const FPragma_Inventory_MissionRequirements& Rhs);

UENUM(BlueprintType, Category=Pragma)
enum class EPragma_Inventory_ExtPurchaseRequirements_DataType : uint8
{
	Invalid,
	MissionRequirements,
	PetEvolutionRequirements,
	CookingCompleteRequirements,
};

USTRUCT(BlueprintType, Category=Pragma)
struct PRAGMASDK_API FPragma_Inventory_ExtPurchaseRequirements_Data
{
	GENERATED_BODY()

	FPragma_Inventory_ExtPurchaseRequirements_Data();
	~FPragma_Inventory_ExtPurchaseRequirements_Data();

	FPragma_Inventory_ExtPurchaseRequirements_Data(const FPragma_Inventory_ExtPurchaseRequirements_Data& Other);
	FPragma_Inventory_ExtPurchaseRequirements_Data& operator=(const FPragma_Inventory_ExtPurchaseRequirements_Data& Other);

    bool Equals(const FPragma_Inventory_ExtPurchaseRequirements_Data& Other) const;

	void Reset();

	EPragma_Inventory_ExtPurchaseRequirements_DataType OneOfType() const { return Type; }

	const FPragma_Inventory_MissionRequirements& MissionRequirements() const;
	const FPragma_Inventory_PetEvolutionRequirements& PetEvolutionRequirements() const;
	const FPragma_Inventory_CookingCompleteRequirements& CookingCompleteRequirements() const;

    void SetMissionRequirements(const FPragma_Inventory_MissionRequirements& Value);
    void SetPetEvolutionRequirements(const FPragma_Inventory_PetEvolutionRequirements& Value);
    void SetCookingCompleteRequirements(const FPragma_Inventory_CookingCompleteRequirements& Value);

	static bool SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
	static bool DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);

private:
	UPROPERTY()
	EPragma_Inventory_ExtPurchaseRequirements_DataType Type { EPragma_Inventory_ExtPurchaseRequirements_DataType::Invalid };

	union
	{
		TTypeCompatibleBytes<FPragma_Inventory_MissionRequirements> MissionRequirements;
		TTypeCompatibleBytes<FPragma_Inventory_PetEvolutionRequirements> PetEvolutionRequirements;
		TTypeCompatibleBytes<FPragma_Inventory_CookingCompleteRequirements> CookingCompleteRequirements;
	} Union{};
};
bool operator==(const FPragma_Inventory_ExtPurchaseRequirements_Data& Lhs, const FPragma_Inventory_ExtPurchaseRequirements_Data& Rhs);
bool operator!=(const FPragma_Inventory_ExtPurchaseRequirements_Data& Lhs, const FPragma_Inventory_ExtPurchaseRequirements_Data& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_ExtPurchaseRequirements
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	FPragma_Inventory_ExtPurchaseRequirements_Data Data;

    PRAGMASDK_API static bool AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
    PRAGMASDK_API static bool AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);
};
PRAGMASDK_API bool operator==(const FPragma_Inventory_ExtPurchaseRequirements& Lhs, const FPragma_Inventory_ExtPurchaseRequirements& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_ExtPurchaseRequirements& Lhs, const FPragma_Inventory_ExtPurchaseRequirements& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_ExtStackableSpec
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_ExtStackableSpec& Lhs, const FPragma_Inventory_ExtStackableSpec& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_ExtStackableSpec& Lhs, const FPragma_Inventory_ExtStackableSpec& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_TrackingSpec
{
	GENERATED_BODY()

	UPROPERTY()
	int64 Max{0};

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_TrackingSpec& Lhs, const FPragma_Inventory_TrackingSpec& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_TrackingSpec& Lhs, const FPragma_Inventory_TrackingSpec& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_GearSpec
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString BonusTableSpecId;

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_GearSpec& Lhs, const FPragma_Inventory_GearSpec& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_GearSpec& Lhs, const FPragma_Inventory_GearSpec& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_MissionsSpec
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TMap<FString, FPragma_Inventory_MissionSpec> MissionsById;

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_MissionsSpec& Lhs, const FPragma_Inventory_MissionsSpec& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_MissionsSpec& Lhs, const FPragma_Inventory_MissionsSpec& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_IncompleteFoodSpec
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_IncompleteFoodSpec& Lhs, const FPragma_Inventory_IncompleteFoodSpec& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_IncompleteFoodSpec& Lhs, const FPragma_Inventory_IncompleteFoodSpec& Rhs);

UENUM(BlueprintType, Category=Pragma)
enum class EPragma_Inventory_ExtInstancedSpec_DataType : uint8
{
	Invalid,
	PetSpec,
	IncompleteFoodSpec,
	MissionsSpec,
	GearSpec,
	TrackingSpec,
};

USTRUCT(BlueprintType, Category=Pragma)
struct PRAGMASDK_API FPragma_Inventory_ExtInstancedSpec_Data
{
	GENERATED_BODY()

	FPragma_Inventory_ExtInstancedSpec_Data();
	~FPragma_Inventory_ExtInstancedSpec_Data();

	FPragma_Inventory_ExtInstancedSpec_Data(const FPragma_Inventory_ExtInstancedSpec_Data& Other);
	FPragma_Inventory_ExtInstancedSpec_Data& operator=(const FPragma_Inventory_ExtInstancedSpec_Data& Other);

    bool Equals(const FPragma_Inventory_ExtInstancedSpec_Data& Other) const;

	void Reset();

	EPragma_Inventory_ExtInstancedSpec_DataType OneOfType() const { return Type; }

	const FPragma_Inventory_PetSpec& PetSpec() const;
	const FPragma_Inventory_IncompleteFoodSpec& IncompleteFoodSpec() const;
	const FPragma_Inventory_MissionsSpec& MissionsSpec() const;
	const FPragma_Inventory_GearSpec& GearSpec() const;
	const FPragma_Inventory_TrackingSpec& TrackingSpec() const;

    void SetPetSpec(const FPragma_Inventory_PetSpec& Value);
    void SetIncompleteFoodSpec(const FPragma_Inventory_IncompleteFoodSpec& Value);
    void SetMissionsSpec(const FPragma_Inventory_MissionsSpec& Value);
    void SetGearSpec(const FPragma_Inventory_GearSpec& Value);
    void SetTrackingSpec(const FPragma_Inventory_TrackingSpec& Value);

	static bool SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
	static bool DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);

private:
	UPROPERTY()
	EPragma_Inventory_ExtInstancedSpec_DataType Type { EPragma_Inventory_ExtInstancedSpec_DataType::Invalid };

	union
	{
		TTypeCompatibleBytes<FPragma_Inventory_PetSpec> PetSpec;
		TTypeCompatibleBytes<FPragma_Inventory_IncompleteFoodSpec> IncompleteFoodSpec;
		TTypeCompatibleBytes<FPragma_Inventory_MissionsSpec> MissionsSpec;
		TTypeCompatibleBytes<FPragma_Inventory_GearSpec> GearSpec;
		TTypeCompatibleBytes<FPragma_Inventory_TrackingSpec> TrackingSpec;
	} Union{};
};
bool operator==(const FPragma_Inventory_ExtInstancedSpec_Data& Lhs, const FPragma_Inventory_ExtInstancedSpec_Data& Rhs);
bool operator!=(const FPragma_Inventory_ExtInstancedSpec_Data& Lhs, const FPragma_Inventory_ExtInstancedSpec_Data& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_ExtInstancedSpec
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	FPragma_Inventory_ExtInstancedSpec_Data Data;

    PRAGMASDK_API static bool AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
    PRAGMASDK_API static bool AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);
};
PRAGMASDK_API bool operator==(const FPragma_Inventory_ExtInstancedSpec& Lhs, const FPragma_Inventory_ExtInstancedSpec& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_ExtInstancedSpec& Lhs, const FPragma_Inventory_ExtInstancedSpec& Rhs);

