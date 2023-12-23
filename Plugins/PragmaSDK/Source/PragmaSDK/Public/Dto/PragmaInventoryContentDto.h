// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/Dto.h.mustache
// </auto-generated>
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Dto/PragmaInventoryCommonDto.h"
#include "Dto/PragmaOptionsDto.h"
#include "Dto/PragmaInventoryContentExtDto.h"

#include "PragmaInventoryContentDto.generated.h"

// Forward declarations.
struct FPragma_Inventory_SteamApp;
struct FPragma_Inventory_SteamInventoryItem;
struct FPragma_Inventory_TwitchDrop;
struct FPragma_Inventory_EpicEntitlement;
struct FPragma_Inventory_SteamEntitlement;
struct FPragma_Inventory_ProviderDetails;
struct FPragma_Inventory_ProviderEntitlement;
struct FPragma_Inventory_ItemBundle;
struct FPragma_Inventory_UpdateEntry;
struct FPragma_Inventory_CraftingEntry;
struct FPragma_Inventory_InstancedSpec;
struct FPragma_Inventory_StackableSpec;
struct FPragma_Inventory_ItemCatalog;

//
// Source protos:
// pragma/inventory/inventoryContent.proto
//


//
// Enums.
//

UENUM(BlueprintType, Category=Pragma)
enum class EPragma_Inventory_EpicOfferType : uint8
{
	UNKNOWN_OFFER_TYPE = 0,
	CONSUMABLE = 1,
	ADDON = 2,
	EDITION = 3,
	GAME = 4,
	DEMO = 5,
	DIGITAL_EXTRA = 6,
	SEASON_PASS = 7,
};

//
// Messages.
//

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_SteamApp
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	int32 AppId{0};

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_SteamApp& Lhs, const FPragma_Inventory_SteamApp& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_SteamApp& Lhs, const FPragma_Inventory_SteamApp& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_SteamInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	int32 ItemDefId{0};

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_SteamInventoryItem& Lhs, const FPragma_Inventory_SteamInventoryItem& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_SteamInventoryItem& Lhs, const FPragma_Inventory_SteamInventoryItem& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_TwitchDrop
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString RewardId;

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_TwitchDrop& Lhs, const FPragma_Inventory_TwitchDrop& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_TwitchDrop& Lhs, const FPragma_Inventory_TwitchDrop& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_EpicEntitlement
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString AudienceItemId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	EPragma_Inventory_EpicOfferType OfferType{static_cast<EPragma_Inventory_EpicOfferType>(0)};

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_EpicEntitlement& Lhs, const FPragma_Inventory_EpicEntitlement& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_EpicEntitlement& Lhs, const FPragma_Inventory_EpicEntitlement& Rhs);

UENUM(BlueprintType, Category=Pragma)
enum class EPragma_Inventory_SteamEntitlement_SourceType : uint8
{
	Invalid,
	InventoryItem,
	App,
};

USTRUCT(BlueprintType, Category=Pragma)
struct PRAGMASDK_API FPragma_Inventory_SteamEntitlement_Source
{
	GENERATED_BODY()

	FPragma_Inventory_SteamEntitlement_Source();
	~FPragma_Inventory_SteamEntitlement_Source();

	FPragma_Inventory_SteamEntitlement_Source(const FPragma_Inventory_SteamEntitlement_Source& Other);
	FPragma_Inventory_SteamEntitlement_Source& operator=(const FPragma_Inventory_SteamEntitlement_Source& Other);

    bool Equals(const FPragma_Inventory_SteamEntitlement_Source& Other) const;

	void Reset();

	EPragma_Inventory_SteamEntitlement_SourceType OneOfType() const { return Type; }

	const FPragma_Inventory_SteamInventoryItem& InventoryItem() const;
	const FPragma_Inventory_SteamApp& App() const;

    void SetInventoryItem(const FPragma_Inventory_SteamInventoryItem& Value);
    void SetApp(const FPragma_Inventory_SteamApp& Value);

	static bool SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
	static bool DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);

private:
	UPROPERTY()
	EPragma_Inventory_SteamEntitlement_SourceType Type { EPragma_Inventory_SteamEntitlement_SourceType::Invalid };

	union
	{
		TTypeCompatibleBytes<FPragma_Inventory_SteamInventoryItem> InventoryItem;
		TTypeCompatibleBytes<FPragma_Inventory_SteamApp> App;
	} Union{};
};
bool operator==(const FPragma_Inventory_SteamEntitlement_Source& Lhs, const FPragma_Inventory_SteamEntitlement_Source& Rhs);
bool operator!=(const FPragma_Inventory_SteamEntitlement_Source& Lhs, const FPragma_Inventory_SteamEntitlement_Source& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_SteamEntitlement
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	FPragma_Inventory_SteamEntitlement_Source Source;

    PRAGMASDK_API static bool AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
    PRAGMASDK_API static bool AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);
};
PRAGMASDK_API bool operator==(const FPragma_Inventory_SteamEntitlement& Lhs, const FPragma_Inventory_SteamEntitlement& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_SteamEntitlement& Lhs, const FPragma_Inventory_SteamEntitlement& Rhs);

UENUM(BlueprintType, Category=Pragma)
enum class EPragma_Inventory_ProviderDetails_DataType : uint8
{
	Invalid,
	Ext,
	SteamEntitlement,
	EpicEntitlement,
	TwitchDrop,
};

USTRUCT(BlueprintType, Category=Pragma)
struct PRAGMASDK_API FPragma_Inventory_ProviderDetails_Data
{
	GENERATED_BODY()

	FPragma_Inventory_ProviderDetails_Data();
	~FPragma_Inventory_ProviderDetails_Data();

	FPragma_Inventory_ProviderDetails_Data(const FPragma_Inventory_ProviderDetails_Data& Other);
	FPragma_Inventory_ProviderDetails_Data& operator=(const FPragma_Inventory_ProviderDetails_Data& Other);

    bool Equals(const FPragma_Inventory_ProviderDetails_Data& Other) const;

	void Reset();

	EPragma_Inventory_ProviderDetails_DataType OneOfType() const { return Type; }

	const FPragma_Inventory_ExtEntitlement& Ext() const;
	const FPragma_Inventory_SteamEntitlement& SteamEntitlement() const;
	const FPragma_Inventory_EpicEntitlement& EpicEntitlement() const;
	const FPragma_Inventory_TwitchDrop& TwitchDrop() const;

    void SetExt(const FPragma_Inventory_ExtEntitlement& Value);
    void SetSteamEntitlement(const FPragma_Inventory_SteamEntitlement& Value);
    void SetEpicEntitlement(const FPragma_Inventory_EpicEntitlement& Value);
    void SetTwitchDrop(const FPragma_Inventory_TwitchDrop& Value);

	static bool SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
	static bool DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);

private:
	UPROPERTY()
	EPragma_Inventory_ProviderDetails_DataType Type { EPragma_Inventory_ProviderDetails_DataType::Invalid };

	union
	{
		TTypeCompatibleBytes<FPragma_Inventory_ExtEntitlement> Ext;
		TTypeCompatibleBytes<FPragma_Inventory_SteamEntitlement> SteamEntitlement;
		TTypeCompatibleBytes<FPragma_Inventory_EpicEntitlement> EpicEntitlement;
		TTypeCompatibleBytes<FPragma_Inventory_TwitchDrop> TwitchDrop;
	} Union{};
};
bool operator==(const FPragma_Inventory_ProviderDetails_Data& Lhs, const FPragma_Inventory_ProviderDetails_Data& Rhs);
bool operator!=(const FPragma_Inventory_ProviderDetails_Data& Lhs, const FPragma_Inventory_ProviderDetails_Data& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_ProviderDetails
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	FPragma_Inventory_ProviderDetails_Data Data;

    PRAGMASDK_API static bool AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
    PRAGMASDK_API static bool AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);
};
PRAGMASDK_API bool operator==(const FPragma_Inventory_ProviderDetails& Lhs, const FPragma_Inventory_ProviderDetails& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_ProviderDetails& Lhs, const FPragma_Inventory_ProviderDetails& Rhs);

UENUM(BlueprintType, Category=Pragma)
enum class EPragma_Inventory_ProviderEntitlement_GrantType : uint8
{
	Invalid,
	Instanced,
	Stackable,
	Bundle,
};

USTRUCT(BlueprintType, Category=Pragma)
struct PRAGMASDK_API FPragma_Inventory_ProviderEntitlement_Grant
{
	GENERATED_BODY()

	FPragma_Inventory_ProviderEntitlement_Grant();
	~FPragma_Inventory_ProviderEntitlement_Grant();

	FPragma_Inventory_ProviderEntitlement_Grant(const FPragma_Inventory_ProviderEntitlement_Grant& Other);
	FPragma_Inventory_ProviderEntitlement_Grant& operator=(const FPragma_Inventory_ProviderEntitlement_Grant& Other);

    bool Equals(const FPragma_Inventory_ProviderEntitlement_Grant& Other) const;

	void Reset();

	EPragma_Inventory_ProviderEntitlement_GrantType OneOfType() const { return Type; }

	const FPragma_Inventory_InstancedItemServerGrant& Instanced() const;
	const FPragma_Inventory_StackableItemGrant& Stackable() const;
	const FPragma_Inventory_ItemBundleGrant& Bundle() const;

    void SetInstanced(const FPragma_Inventory_InstancedItemServerGrant& Value);
    void SetStackable(const FPragma_Inventory_StackableItemGrant& Value);
    void SetBundle(const FPragma_Inventory_ItemBundleGrant& Value);

	static bool SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
	static bool DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);

private:
	UPROPERTY()
	EPragma_Inventory_ProviderEntitlement_GrantType Type { EPragma_Inventory_ProviderEntitlement_GrantType::Invalid };

	union
	{
		TTypeCompatibleBytes<FPragma_Inventory_InstancedItemServerGrant> Instanced;
		TTypeCompatibleBytes<FPragma_Inventory_StackableItemGrant> Stackable;
		TTypeCompatibleBytes<FPragma_Inventory_ItemBundleGrant> Bundle;
	} Union{};
};
bool operator==(const FPragma_Inventory_ProviderEntitlement_Grant& Lhs, const FPragma_Inventory_ProviderEntitlement_Grant& Rhs);
bool operator!=(const FPragma_Inventory_ProviderEntitlement_Grant& Lhs, const FPragma_Inventory_ProviderEntitlement_Grant& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_ProviderEntitlement
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString Id;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<FPragma_Inventory_ProviderDetails> ProviderDetails;

	UPROPERTY(Transient)
	FPragma_Inventory_ProviderEntitlement_Grant Grant;

    PRAGMASDK_API static bool AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
    PRAGMASDK_API static bool AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);
};
PRAGMASDK_API bool operator==(const FPragma_Inventory_ProviderEntitlement& Lhs, const FPragma_Inventory_ProviderEntitlement& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_ProviderEntitlement& Lhs, const FPragma_Inventory_ProviderEntitlement& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_ItemBundle
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString Id;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<FPragma_Inventory_InstancedItemServerGrant> InstancedItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<FPragma_Inventory_StackableItemGrant> StackableItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<FString> Tags;

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_ItemBundle& Lhs, const FPragma_Inventory_ItemBundle& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_ItemBundle& Lhs, const FPragma_Inventory_ItemBundle& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_UpdateEntry
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FPragma_Inventory_ExtUpdateEntry Ext;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString Id;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TMap<FString, FPragma_Inventory_CostEntry> CostByCatalogId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<FString> Tags;

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_UpdateEntry& Lhs, const FPragma_Inventory_UpdateEntry& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_UpdateEntry& Lhs, const FPragma_Inventory_UpdateEntry& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_CraftingEntry
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FPragma_Inventory_ExtCraftingEntry Ext;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString Id;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TMap<FString, FPragma_Inventory_CostEntry> StackableCostByCatalogId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<FString> Tags;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FPragma_Inventory_PurchaseRequirements Requirements;

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_CraftingEntry& Lhs, const FPragma_Inventory_CraftingEntry& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_CraftingEntry& Lhs, const FPragma_Inventory_CraftingEntry& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_InstancedSpec
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString CatalogId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<FString> Tags;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FPragma_Inventory_ExtInstancedSpec Ext;

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_InstancedSpec& Lhs, const FPragma_Inventory_InstancedSpec& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_InstancedSpec& Lhs, const FPragma_Inventory_InstancedSpec& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_StackableSpec
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString CatalogId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString Name;

	UPROPERTY()
	int64 Limit{0};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<FString> Tags;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	bool RemoveIfNone{false};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FPragma_Inventory_ExtStackableSpec Ext;

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_StackableSpec& Lhs, const FPragma_Inventory_StackableSpec& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_StackableSpec& Lhs, const FPragma_Inventory_StackableSpec& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Inventory_ItemCatalog
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<FPragma_Inventory_StackableSpec> StackableEntries;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<FPragma_Inventory_InstancedSpec> InstancedEntries;

};
PRAGMASDK_API bool operator==(const FPragma_Inventory_ItemCatalog& Lhs, const FPragma_Inventory_ItemCatalog& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Inventory_ItemCatalog& Lhs, const FPragma_Inventory_ItemCatalog& Rhs);

