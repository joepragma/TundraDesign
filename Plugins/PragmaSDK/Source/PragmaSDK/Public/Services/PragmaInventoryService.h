#pragma once
#include "PragmaPlayerApi.h"
#include "PragmaResult.h"
#include "Dto/PragmaInventoryRpcDto.h"
#include "Dto/PragmaInventoryServiceRaw.h"
#include "Inventory/PragmaInventoryCache.h"
#include "Dto/PragmaGameDataServiceRaw.h"

#include "PragmaInventoryService.generated.h"

class UPragmaGameDataService;

struct FInventoryStackableResults
{
	// If the cache has failed when doing its last refresh, this will be false
	bool LastRefreshSuccess{false};
	TSharedRef<TArray<FPragma_Inventory_StackableItem>> Items;
};

struct FInventoryInstancedResults
{
	// If the cache has failed when doing its last refresh, this will be false
	bool LastRefreshSuccess{false};
	TSharedRef<TArray<FPragma_Inventory_InstancedItem>> Items;
};

struct FInventoryFull
{
	bool LastRefreshSuccess{false};
	TSharedRef<TArray<FPragma_Inventory_InstancedItem>> Instanced;
	TSharedRef<TArray<FPragma_Inventory_StackableItem>> Stackable;

	TSharedRef<const TMap<FString, TArray<FPragma_Inventory_InstancedItem>>> InstancedByCatalogId;
	TSharedRef<const TMap<FString, TArray<FPragma_Inventory_StackableItem>>> StackablesByCatalogId;
	TSharedRef<const TMap<FString, FPragma_Inventory_StackableItem>> StackableByInstanceId;
	TSharedRef<const TMap<FString, FPragma_Inventory_InstancedItem>> InstancedByInstanceId;
};

struct FInventoryResults
{
	FInventoryFull Full;
	FPragma_Inventory_UpdateSummaryV0 Delta;
};

struct FPragmaSyncEntitlementsResults
{
	TArray<FPragma_Inventory_ProviderEntitlementV1> FulfilledEntitlements;
	TArray<FPragma_Inventory_ProviderEntitlementV1> UnfulfilledEntitlements;
};

DECLARE_DELEGATE_OneParam(FOnCompleteInventoryCallDelegate, TPragmaResult<FInventoryResults>);
DECLARE_DELEGATE_OneParam(FOnCompleteFinalizeInProgressOrdersDelegate, TPragmaResult<TArray<FString>>);
DECLARE_DELEGATE_OneParam(FOnCompleteSyncEntitlementsDelegate, TPragmaResult<FPragmaSyncEntitlementsResults>);
DECLARE_DELEGATE_OneParam(FOnCompleteInventoryCacheCallDelegate, FInventoryFull);
DECLARE_DELEGATE_OneParam(FOnCompleteInstancedItemsDelegate, FInventoryInstancedResults);
DECLARE_DELEGATE_OneParam(FOnCompleteStackableItemsDelegate, FInventoryStackableResults);
DECLARE_DELEGATE_OneParam(FOnWaitForRefresh, bool);

UCLASS()
class PRAGMASDK_API UPragmaInventoryService final : public UPragmaPlayerApi
{
	GENERATED_BODY()

public:
	void SetDependencies(UPragmaInventoryServiceRaw* InventoryRaw, 
	    UPragmaGameDataService* InGameDataService, 
	    TSharedPtr<FPragmaInventoryCache> InCache);
	virtual void OnInitialized() override;
	
	// Access to this service's Raw API methods. Methods on the Raw API bypass any logic or caching provided by this service.
	// All capabilities should be available on this service, use the Raw API only for custom functionality.
	UPragmaInventoryServiceRaw& Raw() const;

	TSharedRef<const TArray<FPragma_Inventory_InstancedItem>> GetInstancedItemsByCatalogId(const FString& CatalogId) const;
	TSharedRef<const TArray<FPragma_Inventory_StackableItem>> GetStackableItemsByCatalogId(const FString& CatalogId) const;
	TSharedRef<const FPragma_Inventory_InstancedItem> GetInstancedItemByInstanceId(const FString& InstanceId) const;
	TSharedRef<const FPragma_Inventory_StackableItem> GetStackableItemByInstanceId(const FString& InstanceId) const;

	DECLARE_DELEGATE_OneParam(FOnCompleteDelegate, TPragmaResult<> /* Result */);

	DECLARE_EVENT_OneParam(UPragmaInventoryService, FInventoryUpdatedEvent, FInventoryFull);
	FInventoryUpdatedEvent OnInventoryUpdated;

	void UpdateItemsV0(const FPragma_Inventory_UpdateItemsV0Request& Payload, FOnCompleteInventoryCallDelegate Delegate);
	void InitializeOrderV1(const FPragma_Inventory_InitializeOrderV1Request& Payload, FOnCompleteDelegate Delegate) const;
	void FinalizeOrderV1(const FPragma_Inventory_FinalizeOrderV1Request& Payload, FOnCompleteInventoryCallDelegate Delegate);
	void FinalizeInProgressOrdersV1(const FPragma_Inventory_FinalizeInProgressOrdersV1Request& Payload, FOnCompleteFinalizeInProgressOrdersDelegate
		Delegate);
	void CancelOrderV1(const FPragma_Inventory_CancelOrderV1Request& Payload, FOnCompleteDelegate Delegate);
	void SyncEntitlements(FOnCompleteSyncEntitlementsDelegate Delegate) const;
	void SyncEntitlementsWithProviderData(const FPragma_Inventory_ProviderData& ProviderData, FOnCompleteSyncEntitlementsDelegate Delegate) const;
	void StorePurchaseV4(const FPragma_Inventory_StorePurchaseV4Request& Payload, FOnCompleteInventoryCallDelegate Delegate);
	void UpdateItemV4(const FPragma_Inventory_UpdateItemV4Request& Payload, FOnCompleteInventoryCallDelegate Delegate);
	void CraftV1(const FPragma_Inventory_CraftV1Request& Payload, FOnCompleteInventoryCallDelegate Delegate);

	void DeletePlayerInventory(FOnCompleteInventoryCallDelegate Delegate);

	FInventoryFull GetInventory() const;
	void ForceGetInventory(const FOnCompleteInventoryCacheCallDelegate& OnComplete);

	void ClearCache() const;

private:
	void GetInstancedItemsFromCache(const FOnCompleteInstancedItemsDelegate& OnComplete, const bool RefreshSuccessful) const;
	void GetStackableItemsFromCache(const FOnCompleteStackableItemsDelegate& OnComplete, const bool RefreshSuccessful) const;
	void WaitOnInventoryRefreshDelegate(FOnWaitForRefresh Work) const;

	void SyncEntitlements(const FPragma_Inventory_SyncEntitlementsV1Request& Payload, FOnCompleteSyncEntitlementsDelegate Delegate) const;

	void BroadcastInventoryUpdated() const;

	UPROPERTY()
	UPragmaInventoryServiceRaw* InventoryServiceRaw;
	UPROPERTY()
	UPragmaGameDataService* GameDataService;
	TSharedPtr<FPragmaInventoryCache> Cache;

	void HandleInventoryUpdatedV1Notification(
		FPragma_Inventory_InventoryUpdatedV1Notification Pragma_Inventory_InventoryUpdatedV1Notification,
		const FPragmaMessageMetadata& PragmaMessageMetadata) const;
	void HandleResetClientInventoryCacheVersionV1Notification(
		FPragma_Inventory_ResetClientInventoryCacheVersionV1Notification Notification,
		const FPragmaMessageMetadata& PragmaMessageMetadata) const;


	TSharedPtr<FPragmaInventoryCache> CreateDefaultCache();

	template<typename ResponseType>
	void UpdateCacheAndReturnInventory(ResponseType Result, FOnCompleteInventoryCallDelegate Delegate);
};

struct FFinalizeOrderV1ResponseWrapper
{
	bool IsSuccessful() const
	{
		return Result.IsSuccessful();
	}

	FPragma_Inventory_UpdateItemsResponse& Payload()
	{
		return Result.Payload().MtxResponse;
	}

	FPragmaError Error() const
	{
		return Result.Error();
	}

	TPragmaResult<FPragma_Inventory_FinalizeOrderV1Response> Result;
};
