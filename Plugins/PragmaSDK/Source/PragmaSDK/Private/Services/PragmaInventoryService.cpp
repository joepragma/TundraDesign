#include "Services/PragmaInventoryService.h"
#include "Dto/PragmaInventoryServiceRaw.h"
#include "Services/PragmaGameDataService.h"
#include "Dto/PragmaGameDataServiceRaw.h"
#include "PragmaPlayer.h"

void UPragmaInventoryService::SetDependencies(
	UPragmaInventoryServiceRaw* InventoryRaw,
	UPragmaGameDataService* InGameDataService,
	TSharedPtr<FPragmaInventoryCache> InCache)
{
	InventoryServiceRaw = InventoryRaw;
	GameDataService = InGameDataService;
	if (InCache != nullptr)
	{
		Cache = MoveTemp(InCache);
	}
}

void UPragmaInventoryService::OnInitialized()
{
	if (Cache == nullptr)
	{
		Cache = CreateDefaultCache();
	}
	Cache->OnInventoryUpdated.AddUObject(this, &UPragmaInventoryService::BroadcastInventoryUpdated);
	InventoryServiceRaw->OnInventoryUpdatedV1.AddUObject(this,
		&UPragmaInventoryService::HandleInventoryUpdatedV1Notification);
	InventoryServiceRaw->OnResetClientInventoryCacheVersionV1.AddUObject(this,
		&UPragmaInventoryService::HandleResetClientInventoryCacheVersionV1Notification);
}

UPragmaInventoryServiceRaw& UPragmaInventoryService::Raw() const
{
	checkf(InventoryServiceRaw != nullptr, TEXT("You need to call SetDependencies before running this."));
	return *InventoryServiceRaw;
}

TSharedPtr<FPragmaInventoryCache> UPragmaInventoryService::CreateDefaultCache()
{
	auto DefaultCache = MakeShared<FPragmaInventoryCache>(FRefresher::CreateLambda(
		[this]
	(FRefreshOnCompleteDelegate OnComplete)
		{
			constexpr FPragma_Inventory_GetInventoryV2Request Request{};
			InventoryServiceRaw->GetInventoryV2(Request,
				UPragmaInventoryServiceRaw::FGetInventoryV2Delegate::CreateLambda(
					[OnComplete]
				(TPragmaResult<FPragma_Inventory_GetInventoryV2Response> Result, const FPragmaMessageMetadata& Metadata)
					{
						if (Result.IsFailure())
						{
							OnComplete.ExecuteIfBound(
								TPragmaResult<FPragma_Inventory_InventoryFullV1Data>::Failure(Result.Error()));
						}
						else
						{
							OnComplete.ExecuteIfBound(
								TPragmaResult<FPragma_Inventory_InventoryFullV1Data>(MoveTemp(Result.Payload().Full)));
						}
					}
				));
		}
	));

	TSharedPtr<FDelegateHandle> Handle = MakeShared<FDelegateHandle>();
	*Handle = Player()->OnConnected.AddWeakLambda(this, [this, DefaultCache, Handle]()
	{
		DefaultCache->Init(FOnCompleteDelegate::CreateLambda([](TPragmaResult<>)
		{
		}));
		Player()->OnConnected.Remove(*Handle);
	});
	return DefaultCache;
}

TSharedRef<const TArray<FPragma_Inventory_InstancedItem>> UPragmaInventoryService::GetInstancedItemsByCatalogId(
	const FString& CatalogId) const
{
	return Cache->GetInstancedItemsByCatalogId(CatalogId);
}

TSharedRef<const TArray<FPragma_Inventory_StackableItem>> UPragmaInventoryService::GetStackableItemsByCatalogId(
	const FString& CatalogId) const
{
	return Cache->GetStackableItemsByCatalogId(CatalogId);
}

TSharedRef<const FPragma_Inventory_InstancedItem> UPragmaInventoryService::GetInstancedItemByInstanceId(
	const FString& InstanceId) const
{
	TSharedRef<FPragma_Inventory_InstancedItem> ItemRef = MakeShared<FPragma_Inventory_InstancedItem>();
	*ItemRef = Cache->GetInstancedByInstanceId().Get()[InstanceId];
	return ItemRef;
}

TSharedRef<const FPragma_Inventory_StackableItem> UPragmaInventoryService::GetStackableItemByInstanceId(
	const FString& InstanceId) const
{
	TSharedRef<FPragma_Inventory_StackableItem> ItemRef = MakeShared<FPragma_Inventory_StackableItem>();
	*ItemRef = Cache->GetStackableByInstanceId().Get()[InstanceId];
	return ItemRef;
}

void UPragmaInventoryService::UpdateItemsV0(const FPragma_Inventory_UpdateItemsV0Request& Payload,
	FOnCompleteInventoryCallDelegate Delegate)
{
	Raw().UpdateItemsV0(Payload, UPragmaInventoryServiceRaw::FUpdateItemsV0Delegate::CreateLambda(
		[this, Delegate = MoveTemp(Delegate)]
	(TPragmaResult<FPragma_Inventory_UpdateItemsV0Response> Result, const FPragmaMessageMetadata& _)
		{
			UpdateCacheAndReturnInventory<TPragmaResult<FPragma_Inventory_UpdateItemsV0Response>>(MoveTemp(Result),
				Delegate);
		}
	));
}

void UPragmaInventoryService::InitializeOrderV1(const FPragma_Inventory_InitializeOrderV1Request& Payload,
	FOnCompleteDelegate Delegate) const
{
	Raw().InitializeOrderV1(Payload, UPragmaInventoryServiceRaw::FInitializeOrderV1Delegate::CreateLambda(
		[this, Delegate = MoveTemp(Delegate)]
	(TPragmaResult<FPragma_Inventory_InitializeOrderV1Response> Result, const FPragmaMessageMetadata& _)
		{
			if (Result.IsFailure())
			{
				Delegate.ExecuteIfBound(TPragmaResult<>::Failure(Result.Error()));
			}
			else
			{
				Delegate.ExecuteIfBound(TPragmaResult<>::Success());
			}
		}
	));
}

void UPragmaInventoryService::FinalizeOrderV1(const FPragma_Inventory_FinalizeOrderV1Request& Payload,
	FOnCompleteInventoryCallDelegate Delegate)
{
	Raw().FinalizeOrderV1(Payload, UPragmaInventoryServiceRaw::FFinalizeOrderV1Delegate::CreateLambda(
		[this, Delegate = MoveTemp(Delegate)]
	(TPragmaResult<FPragma_Inventory_FinalizeOrderV1Response> Result, const FPragmaMessageMetadata& _)
		{
			UpdateCacheAndReturnInventory<FFinalizeOrderV1ResponseWrapper>(
				FFinalizeOrderV1ResponseWrapper{MoveTemp(Result)}, Delegate);
		}
	));
}

void UPragmaInventoryService::FinalizeInProgressOrdersV1(
	const FPragma_Inventory_FinalizeInProgressOrdersV1Request& Payload,
	FOnCompleteFinalizeInProgressOrdersDelegate Delegate)
{
	Raw().FinalizeInProgressOrdersV1(Payload,
		UPragmaInventoryServiceRaw::FFinalizeInProgressOrdersV1Delegate::CreateLambda(
			[this, Delegate = MoveTemp(Delegate)]
		(TPragmaResult<FPragma_Inventory_FinalizeInProgressOrdersV1Response> Result, const FPragmaMessageMetadata& _)
			{
				if (Result.IsSuccessful())
				{
					if (Result.Payload().GrantedBundleIds.Num() > 0)
					{
						Cache->ClearCache();
						Cache->RefreshCache(FOnCompleteDelegate::CreateLambda(
							[Result = MoveTemp(Result), Delegate = Delegate](TPragmaResult<> CacheRefreshResult)
							{
								Delegate.ExecuteIfBound(TPragmaResult<TArray<FString>>{
									Result.Payload().GrantedBundleIds
								});
							}
						));
					}
					else
					{
						Delegate.ExecuteIfBound(TPragmaResult<TArray<FString>>{
							MoveTemp(Result.Payload().GrantedBundleIds)
						});
					}
				}
				else
				{
					Delegate.ExecuteIfBound(TPragmaResult<TArray<FString>>::Failure(Result.Error()));
				}
			}
		));
}

void UPragmaInventoryService::CancelOrderV1(const FPragma_Inventory_CancelOrderV1Request& Payload,
	FOnCompleteDelegate Delegate)
{
	Raw().CancelOrderV1(Payload, UPragmaInventoryServiceRaw::FCancelOrderV1Delegate::CreateLambda(
		[this, Delegate = MoveTemp(Delegate)]
	(TPragmaResult<FPragma_Inventory_CancelOrderV1Response> Result, const FPragmaMessageMetadata& _)
		{
			if (Result.IsFailure())
			{
				Delegate.ExecuteIfBound(TPragmaResult<>::Failure(Result.Error()));
			}
			else
			{
				Delegate.ExecuteIfBound(TPragmaResult<>::Success());
			}
		}
	));
}

void UPragmaInventoryService::SyncEntitlements(const FPragma_Inventory_SyncEntitlementsV1Request& Payload,
	FOnCompleteSyncEntitlementsDelegate Delegate) const
{
	Raw().SyncEntitlementsV1(Payload, UPragmaInventoryServiceRaw::FSyncEntitlementsV1Delegate::CreateLambda(
		[this, Delegate = MoveTemp(Delegate)]
	(TPragmaResult<FPragma_Inventory_SyncEntitlementsV1Response> Result, const FPragmaMessageMetadata&)
		{
			if (Result.IsFailure())
			{
				Delegate.ExecuteIfBound(TPragmaResult<FPragmaSyncEntitlementsResults>::Failure(Result.Error()));
			}
			else
			{
				FPragmaSyncEntitlementsResults Results;
				Results.FulfilledEntitlements = MoveTemp(Result.Payload().Fulfilled);
				Results.UnfulfilledEntitlements = MoveTemp(Result.Payload().Unfulfilled);

				if (Results.FulfilledEntitlements.Num() == 0)
				{
					Delegate.ExecuteIfBound(TPragmaResult<FPragmaSyncEntitlementsResults>(Results));
				}
				else
				{
					Cache->ClearCache();
					Cache->RefreshCache(FOnCompleteDelegate::CreateLambda(
							[Results = MoveTemp(Results), Delegate = Delegate](TPragmaResult<>)
							{
								Delegate.ExecuteIfBound(TPragmaResult<FPragmaSyncEntitlementsResults>(Results));
							})
					);
				}
			}
		}
	));
}


void UPragmaInventoryService::SyncEntitlementsWithProviderData(const FPragma_Inventory_ProviderData& ProviderData,
	const FOnCompleteSyncEntitlementsDelegate Delegate) const
{
	const auto Request = FPragma_Inventory_SyncEntitlementsV1Request{
		TArray<FPragma_Inventory_ProviderData>{ProviderData}
	};
	SyncEntitlements(Request, Delegate);
}


void UPragmaInventoryService::SyncEntitlements(const FOnCompleteSyncEntitlementsDelegate Delegate) const
{
	const auto Request = FPragma_Inventory_SyncEntitlementsV1Request{TArray<FPragma_Inventory_ProviderData>{}};
	SyncEntitlements(Request, Delegate);
}

void UPragmaInventoryService::StorePurchaseV4(const FPragma_Inventory_StorePurchaseV4Request& Payload,
	FOnCompleteInventoryCallDelegate Delegate)
{
	Raw().StorePurchaseV4(Payload, UPragmaInventoryServiceRaw::FStorePurchaseV4Delegate::CreateLambda(
		[this, Delegate = MoveTemp(Delegate)]
	(TPragmaResult<FPragma_Inventory_StorePurchaseV4Response> Result, const FPragmaMessageMetadata& _)
		{
			UpdateCacheAndReturnInventory<TPragmaResult<FPragma_Inventory_StorePurchaseV4Response>>(MoveTemp(Result),
				Delegate);
		}
	));
}

void UPragmaInventoryService::UpdateItemV4(const FPragma_Inventory_UpdateItemV4Request& Payload,
	FOnCompleteInventoryCallDelegate Delegate)
{
	Raw().UpdateItemV4(Payload, UPragmaInventoryServiceRaw::FUpdateItemV4Delegate::CreateLambda(
		[this, Delegate = MoveTemp(Delegate)]
	(TPragmaResult<FPragma_Inventory_UpdateItemV4Response> Result, const FPragmaMessageMetadata& _)
		{
			UpdateCacheAndReturnInventory<TPragmaResult<FPragma_Inventory_UpdateItemV4Response>>(MoveTemp(Result),
				Delegate);
		}
	));
}

void UPragmaInventoryService::CraftV1(const FPragma_Inventory_CraftV1Request& Payload,
	FOnCompleteInventoryCallDelegate Delegate)
{
	Raw().CraftV1(Payload, UPragmaInventoryServiceRaw::FCraftV1Delegate::CreateLambda(
		[this, Delegate = MoveTemp(Delegate)]
	(TPragmaResult<FPragma_Inventory_CraftV1Response> Result, const FPragmaMessageMetadata& _)
		{
			if (Result.IsFailure())
			{
				Delegate.ExecuteIfBound(TPragmaResult<FInventoryResults>::Failure(Result.Error()));
				return;
			}
			if (Result.Payload().Errors.Num() > 0)
			{
				PRAGMA_LOG(Error, "UPragmaInventoryService::CraftV1 - response contained invalid craft errors - %s",
					*FString::Join(Result.Payload().Errors, TEXT(", ")));
				Delegate.ExecuteIfBound(
					TPragmaResult<FInventoryResults>::Failure(EPragma_SdkError::InventoryService_CraftFailed));
				return;
			}
			UpdateCacheAndReturnInventory<TPragmaResult<FPragma_Inventory_CraftV1Response>>(MoveTemp(Result), Delegate);
		}
	));
}

void UPragmaInventoryService::DeletePlayerInventory(
	FOnCompleteInventoryCallDelegate Delegate)
{
	Raw().DeletePlayerInventoryV3(FPragma_Inventory_DeletePlayerInventoryV3Request(),
		UPragmaInventoryServiceRaw::FDeletePlayerInventoryV3Delegate::CreateLambda(
			[this, Delegate = MoveTemp(Delegate)]
		(TPragmaResult<FPragma_Inventory_DeletePlayerInventoryV3Response> Result, const FPragmaMessageMetadata& _)
			{
				Cache->ClearCache();
				FInventoryFull Full = GetInventory();

				Delegate.ExecuteIfBound(TPragmaResult<FInventoryResults>(FInventoryResults{
					MoveTemp(Full), FPragma_Inventory_UpdateSummaryV0()
				}));
			}
		));
}

template <typename ResponseType>
void UPragmaInventoryService::UpdateCacheAndReturnInventory(ResponseType Result,
	FOnCompleteInventoryCallDelegate Delegate)
{
	if (Result.IsSuccessful())
	{
		auto Delta = MoveTemp(Result.Payload().Delta);
		Cache->UpdateCacheWithSegment(MoveTemp(Result.Payload().Segment), FOnCompleteDelegate::CreateLambda(
			[this, Delta = MoveTemp(Delta), Delegate = MoveTemp(Delegate)]
		(TPragmaResult<> CacheUpdateResult) mutable
			{
				FInventoryFull Full = GetInventory();
				Delegate.ExecuteIfBound(TPragmaResult<FInventoryResults>(FInventoryResults{
					MoveTemp(Full), MoveTemp(Delta)
				}));
			}));
	}
	else
	{
		Delegate.ExecuteIfBound(TPragmaResult<FInventoryResults>::Failure(Result.Error()));
	}
}

void UPragmaInventoryService::WaitOnInventoryRefreshDelegate(FOnWaitForRefresh Work) const
{
	const auto WaitForRefresh = MakeShared<FDelegateHandle>();
	*WaitForRefresh = Cache->OnInventoryRefreshed.AddLambda(
		[this, WaitForRefresh, Work]
	(const bool RefreshSucceeded)
		{
			bool _ = Work.ExecuteIfBound(RefreshSucceeded);
			Cache->OnInventoryRefreshed.Remove(*WaitForRefresh);
		}
	);
}

void UPragmaInventoryService::BroadcastInventoryUpdated() const
{
	OnInventoryUpdated.Broadcast(GetInventory());
}

void UPragmaInventoryService::HandleInventoryUpdatedV1Notification(
	FPragma_Inventory_InventoryUpdatedV1Notification Notification,
	const FPragmaMessageMetadata&) const
{
	Cache->UpdateCacheWithSegment(MoveTemp(Notification.Segment), {});
}

void UPragmaInventoryService::HandleResetClientInventoryCacheVersionV1Notification(
	FPragma_Inventory_ResetClientInventoryCacheVersionV1Notification,
	const FPragmaMessageMetadata&) const
{
	Cache->ResetVersion();
}

void UPragmaInventoryService::GetInstancedItemsFromCache(const FOnCompleteInstancedItemsDelegate& OnComplete,
	const bool RefreshSuccessful) const
{
	const TSharedRef<TArray<FPragma_Inventory_InstancedItem>> Result = Cache->GetInstancedItems();
	bool _ = OnComplete.ExecuteIfBound(FInventoryInstancedResults{RefreshSuccessful, Result});
}

void UPragmaInventoryService::GetStackableItemsFromCache(const FOnCompleteStackableItemsDelegate& OnComplete,
	const bool RefreshSuccessful) const
{
	const TSharedRef<TArray<FPragma_Inventory_StackableItem>> Result = Cache->GetStackableItems();
	bool _ = OnComplete.ExecuteIfBound(FInventoryStackableResults{RefreshSuccessful, Result});
}

FInventoryFull UPragmaInventoryService::GetInventory() const
{
	return FInventoryFull{
		Cache->WasLastCacheRefreshSuccessful(),
		Cache->GetInstancedItems(),
		Cache->GetStackableItems(),
		Cache->GetInstancedByCatalogId(),
		Cache->GetStackablesByCatalogId(),
		Cache->GetStackableByInstanceId(),
		Cache->GetInstancedByInstanceId()
	};
}

void UPragmaInventoryService::ForceGetInventory(const FOnCompleteInventoryCacheCallDelegate& OnComplete)
{
	if (Cache->IsCacheRefreshing())
	{
		WaitOnInventoryRefreshDelegate(FOnWaitForRefresh::CreateLambda([this, OnComplete](bool) {
			bool _ = OnComplete.ExecuteIfBound(GetInventory());
		}));
	}
	else
	{
		Cache->ClearCache();
		Cache->RefreshCache(FOnCompleteDelegate::CreateLambda(
			[this, OnComplete](TPragmaResult<>) {
				bool _ = OnComplete.ExecuteIfBound(GetInventory());
			}));
	}
}
void UPragmaInventoryService::ClearCache() const
{
	Cache->ClearCache();
}
