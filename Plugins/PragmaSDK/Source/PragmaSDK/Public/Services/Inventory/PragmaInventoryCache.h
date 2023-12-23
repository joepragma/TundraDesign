#pragma once
#include "PragmaResult.h"
#include "Dto/PragmaInventoryCommonDto.h"

struct FPragma_Inventory_InventoryFullV1Data;
struct FPragma_Inventory_InventorySegmentV1Data;

DECLARE_DELEGATE_OneParam(FRefreshOnCompleteDelegate, TPragmaResult<FPragma_Inventory_InventoryFullV1Data>);
DECLARE_DELEGATE_OneParam(FOnCompleteDelegate, TPragmaResult<>);
DECLARE_DELEGATE_OneParam(FRefresher, FRefreshOnCompleteDelegate)

const int64 UNINITIALIZED_VERSION = -1;

class PRAGMASDK_API FPragmaInventoryCache
{
public:
	virtual ~FPragmaInventoryCache() = default;
	TSharedRef<TArray<FPragma_Inventory_InstancedItem>> GetInstancedItemsByCatalogId(const FString& CatalogId);
	TSharedRef<TArray<FPragma_Inventory_StackableItem>> GetStackableItemsByCatalogId(const FString& CatalogId);

	explicit FPragmaInventoryCache(FRefresher Refresher);

	void Init(const FOnCompleteDelegate& OnComplete);
	virtual void UpdateCacheWithSegment(
		FPragma_Inventory_InventorySegmentV1Data Segment,
	    const FOnCompleteDelegate& OnComplete);
	virtual void UpdateCacheWithFull(FPragma_Inventory_InventoryFullV1Data Full);
	virtual void ClearCache();
	void ResetVersion();

	int64 GetVersion() const;

	int32 InstancedItemCount() const;
	bool ContainsInstancedItem(const FString& InstanceId) const;
	TSharedRef<TArray<FPragma_Inventory_InstancedItem>> GetInstancedItems() const;

	int32 StackableItemCount() const;
	bool ContainsStackableItem(const FString& InstanceId) const;
	TSharedRef<TArray<FPragma_Inventory_StackableItem>> GetStackableItems() const;
	TSharedRef<TMap<FString, TArray<FPragma_Inventory_InstancedItem>>> GetInstancedByCatalogId() const;
	TSharedRef<TMap<FString, TArray<FPragma_Inventory_StackableItem>>> GetStackablesByCatalogId() const;
	TSharedRef<TMap<FString, FPragma_Inventory_StackableItem>> GetStackableByInstanceId() const;
	TSharedRef<TMap<FString, FPragma_Inventory_InstancedItem>> GetInstancedByInstanceId() const;

	bool WasLastCacheRefreshSuccessful() const;
	bool IsInitialized() const;
	bool IsCacheRefreshing() const;

	// Fired any time the cache has been refreshed.
	DECLARE_EVENT_OneParam(FPragmaInventoryCache, FInventoryRefreshed, bool /*bRefreshSuccessful*/);

	FInventoryRefreshed OnInventoryRefreshed;

	// Fired any time the cache is updated at all from any source.
	DECLARE_EVENT(FPragmaInventoryCache, FInventoryUpdatedEvent);

	virtual void RefreshCache(const FOnCompleteDelegate& OnComplete);

	FInventoryUpdatedEvent OnInventoryUpdated;

private:
	FRefresher RefreshCacheCallback;
	bool bIsInitialized{false};
	int64 Version{UNINITIALIZED_VERSION};
	TMap<FString, TArray<FPragma_Inventory_InstancedItem>> InstancedByCatalogId;
	TMap<FString, TArray<FPragma_Inventory_StackableItem>> StackablesByCatalogId;

	void AddItemsToCacheAndUpdateVersion(
		const int64 NewVersion,
		const TArray<FPragma_Inventory_InstancedItem> InstancedItems,
		const TArray<FPragma_Inventory_StackableItem> StackableItems);

protected:
	TMap<FString, FPragma_Inventory_InstancedItem> InstancedItemMap;
	TMap<FString, FPragma_Inventory_StackableItem> StackableItemMap;
	bool bLastCacheRefreshSuccess{false};
	bool bIsCacheRefreshing{false};
	void BuildItemsByCatalogIdMaps();
	void BuildInstancedItemsByCatalogId();
	void BuildStackableItemsByCatalogId();
};
