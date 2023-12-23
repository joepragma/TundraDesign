#include "Services/Inventory/PragmaInventoryCache.h"

#include "PragmaLog.h"
#include "Dto/PragmaInventoryRpcDto.h"


TSharedRef<TArray<FPragma_Inventory_InstancedItem>> FPragmaInventoryCache::GetInstancedItemsByCatalogId(
	const FString& CatalogId)
{
	TSharedRef<TArray<FPragma_Inventory_InstancedItem>> Result = MakeShared<TArray<FPragma_Inventory_InstancedItem>>();
	if (InstancedByCatalogId.Contains(CatalogId))
	{
		*Result = InstancedByCatalogId[CatalogId];
	}
	return Result;
}

TSharedRef<TArray<FPragma_Inventory_StackableItem>> FPragmaInventoryCache::GetStackableItemsByCatalogId(
	const FString& CatalogId)
{
	TSharedRef<TArray<FPragma_Inventory_StackableItem>> Result = MakeShared<TArray<FPragma_Inventory_StackableItem>>();
	if (StackablesByCatalogId.Contains(CatalogId))
	{
		*Result = StackablesByCatalogId[CatalogId];
	}

	return Result;
}

TSharedRef<TMap<FString, FPragma_Inventory_StackableItem>> FPragmaInventoryCache::GetStackableByInstanceId() const
{
	TSharedRef<TMap<FString, FPragma_Inventory_StackableItem>> Result = MakeShared<TMap<
		FString, FPragma_Inventory_StackableItem>>();
	*Result = StackableItemMap;
	return Result;
}

TSharedRef<TMap<FString, FPragma_Inventory_InstancedItem>> FPragmaInventoryCache::GetInstancedByInstanceId() const
{
	TSharedRef<TMap<FString, FPragma_Inventory_InstancedItem>> Result = MakeShared<TMap<
		FString, FPragma_Inventory_InstancedItem>>();
	*Result = InstancedItemMap;
	return Result;
}

FPragmaInventoryCache::FPragmaInventoryCache(FRefresher Refresher)
	: RefreshCacheCallback(Refresher)
{
}

int64 FPragmaInventoryCache::GetVersion() const
{
	return Version;
}

void FPragmaInventoryCache::Init(const FOnCompleteDelegate& OnComplete)
{
	if (!bIsInitialized)
	{
		bIsInitialized = true;
		RefreshCache(OnComplete);
	}
	else
	{
		OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
	}
}

bool FPragmaInventoryCache::IsInitialized() const
{
	return bIsInitialized;
}

void FPragmaInventoryCache::RefreshCache(const FOnCompleteDelegate& OnComplete)
{
	bIsCacheRefreshing = true;
	RefreshCacheCallback.ExecuteIfBound(FRefreshOnCompleteDelegate::CreateLambda(
		[this, OnComplete](TPragmaResult<FPragma_Inventory_InventoryFullV1Data> Result)
		{
			bIsCacheRefreshing = false;
			if (Result.IsSuccessful())
			{
				FPragma_Inventory_InventoryFullV1Data& InventoryFull = Result.Payload();
				UpdateCacheWithFull(InventoryFull);
				bLastCacheRefreshSuccess = true;
				OnInventoryRefreshed.Broadcast(true);
				OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
			}
			else
			{
				bLastCacheRefreshSuccess = false;
				OnInventoryRefreshed.Broadcast(false);
				OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(Result.Error()));
			}
		}
	));
}

bool FPragmaInventoryCache::IsCacheRefreshing() const
{
	return bIsCacheRefreshing;
}

void FPragmaInventoryCache::AddItemsToCacheAndUpdateVersion(
	const int64 NewVersion,
	TArray<FPragma_Inventory_InstancedItem> InstancedItems,
	TArray<FPragma_Inventory_StackableItem> StackableItems)
{
	Version = NewVersion;
	for (auto& Item : InstancedItems)
	{
		InstancedItemMap.Emplace(Item.InstanceId, MoveTemp(Item));
	}
	for (auto& Item : StackableItems)
	{
		StackableItemMap.Emplace(Item.InstanceId, MoveTemp(Item));
	}
	BuildItemsByCatalogIdMaps();
}

void FPragmaInventoryCache::BuildInstancedItemsByCatalogId()
{
	InstancedByCatalogId.Empty();
	for (auto Pair : InstancedItemMap)
	{
		if (InstancedByCatalogId.Contains(Pair.Value.CatalogId))
		{
			InstancedByCatalogId[Pair.Value.CatalogId].Add(Pair.Value);
		}
		else
		{
			InstancedByCatalogId.Emplace(Pair.Value.CatalogId, TArray<FPragma_Inventory_InstancedItem>{Pair.Value});
		}
	}
}

void FPragmaInventoryCache::BuildStackableItemsByCatalogId()
{
	StackablesByCatalogId.Empty();
	for (auto Pair : StackableItemMap)
	{
		if (StackablesByCatalogId.Contains(Pair.Value.CatalogId))
		{
			StackablesByCatalogId[Pair.Value.CatalogId].Add(Pair.Value);
		}
		else
		{
			StackablesByCatalogId.Emplace(Pair.Value.CatalogId, TArray<FPragma_Inventory_StackableItem>{Pair.Value});
		}
	}
}

void FPragmaInventoryCache::BuildItemsByCatalogIdMaps()
{
	BuildInstancedItemsByCatalogId();
	BuildStackableItemsByCatalogId();
}

void FPragmaInventoryCache::UpdateCacheWithSegment(
	FPragma_Inventory_InventorySegmentV1Data Segment,
	const FOnCompleteDelegate& OnComplete)
{
	if (Segment.Version == Version)
	{
		OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
	}
	else if (Segment.PreviousVersion != Version)
	{
		this->RefreshCache(OnComplete);
	}
	else if (Segment.Version < Version)
	{
		OnComplete.ExecuteIfBound(TPragmaResult<>::Failure("Unexpected segment version."));
	}
	else
	{
		for (const auto& Id : Segment.RemovedInstanced)
		{
			InstancedItemMap.Remove(Id);
		}
		for (const auto& Id : Segment.RemovedStackables)
		{
			StackableItemMap.Remove(Id);
		}
		AddItemsToCacheAndUpdateVersion(Segment.Version, Segment.Instanced, Segment.Stackables);
		OnInventoryUpdated.Broadcast();
		OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
	}
}

void FPragmaInventoryCache::UpdateCacheWithFull(FPragma_Inventory_InventoryFullV1Data Full)
{
	if (Version >= Full.Version)
	{
		return;
	}
	InstancedItemMap.Empty();
	StackableItemMap.Empty();
	AddItemsToCacheAndUpdateVersion(Full.Version, MoveTemp(Full.Instanced), MoveTemp(Full.Stackables));
	OnInventoryUpdated.Broadcast();
}

void FPragmaInventoryCache::ClearCache()
{
	InstancedItemMap.Empty();
	StackableItemMap.Empty();
	InstancedByCatalogId.Empty();
	StackablesByCatalogId.Empty();
	Version = UNINITIALIZED_VERSION;
	OnInventoryUpdated.Broadcast();
}

void FPragmaInventoryCache::ResetVersion()
{
	Version = UNINITIALIZED_VERSION;
}


bool FPragmaInventoryCache::ContainsInstancedItem(const FString& InstanceId) const
{
	return InstancedItemMap.Contains(*InstanceId);
}

int32 FPragmaInventoryCache::InstancedItemCount() const
{
	return InstancedItemMap.Num();
}

bool FPragmaInventoryCache::ContainsStackableItem(const FString& InstanceId) const
{
	return StackableItemMap.Contains(*InstanceId);
}

int32 FPragmaInventoryCache::StackableItemCount() const
{
	return StackableItemMap.Num();
}

TSharedRef<TArray<FPragma_Inventory_StackableItem>> FPragmaInventoryCache::GetStackableItems() const
{
	TSharedRef<TArray<FPragma_Inventory_StackableItem>> Result = MakeShared<TArray<FPragma_Inventory_StackableItem>>();
	StackableItemMap.GenerateValueArray(Result.Get());
	return Result;
}

TSharedRef<TMap<FString, TArray<FPragma_Inventory_InstancedItem>>> FPragmaInventoryCache::
GetInstancedByCatalogId() const
{
	TSharedRef<TMap<FString, TArray<FPragma_Inventory_InstancedItem>>> RefMap = MakeShared<TMap<
		FString, TArray<FPragma_Inventory_InstancedItem>>>();
	*RefMap = InstancedByCatalogId;
	return RefMap;
}

TSharedRef<TMap<FString, TArray<FPragma_Inventory_StackableItem>>> FPragmaInventoryCache::
GetStackablesByCatalogId() const
{
	TSharedRef<TMap<FString, TArray<FPragma_Inventory_StackableItem>>> RefMap = MakeShared<TMap<
		FString, TArray<FPragma_Inventory_StackableItem>>>();
	*RefMap = StackablesByCatalogId;
	return RefMap;
}

TSharedRef<TArray<FPragma_Inventory_InstancedItem>> FPragmaInventoryCache::GetInstancedItems() const
{
	TSharedRef<TArray<FPragma_Inventory_InstancedItem>> Result = MakeShared<TArray<FPragma_Inventory_InstancedItem>>();
	InstancedItemMap.GenerateValueArray(Result.Get());
	return Result;
}

bool FPragmaInventoryCache::WasLastCacheRefreshSuccessful() const
{
	return bLastCacheRefreshSuccess;
}
