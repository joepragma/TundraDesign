#include "Services/PragmaPlayerDataService.h"
#include "PragmaServiceImplMacros.h"
#include "Dto/PragmaPlayerDataPartnerServiceRaw.h"
#include "Dto/PragmaPlayerDataServiceRaw.h"

void UPragmaPlayerDataService::OnInitialized()
{
	PlayerDataServiceRaw->OnPlayerDataUpdatedV1.AddUObject(this, &UPragmaPlayerDataService::HandleOnPlayerDataUpdatedV1Notification);
}

void UPragmaPlayerDataService::SetDependencies(UPragmaPlayerDataServiceRaw* PlayerDataRaw)
{
	PlayerDataServiceRaw = PlayerDataRaw;
#ifndef NOT_USING_PRAGMA_PLAYER_DATA
	CreateSubServices(PragmaPlayerData::FPlayerDataRequestDelegate::CreateWeakLambda(this, [this](FPragma_PlayerData_PlayerDataRequestProto_Payload Payload, FOnPlayerDataResult Delegate) {
		Request(MoveTemp(Payload), MoveTemp(Delegate));
	}));
#endif
}

void UPragmaPlayerDataService::ClearCache() const
{
	PlayerDataCache->Clear();
}

#ifndef NOT_USING_PRAGMA_PLAYER_DATA
void UPragmaPlayerDataService::Request(FPragma_PlayerData_PlayerDataRequestProto_Payload Payload, FOnPlayerDataResult Delegate)
{
	if (!PlayerDataServiceRaw)
		return;

	FPragma_PlayerData_DoOperationV1Request Request{};
	Request.Ext.Payload = MoveTemp(Payload);

	PlayerDataServiceRaw->DoOperationV1(Request, UPragmaPlayerDataServiceRaw::FDoOperationV1Delegate::CreateWeakLambda(this, [this, Delegate = MoveTemp(Delegate)](TPragmaResult<FPragma_PlayerData_DoOperationV1Response> OperationResponse, const FPragmaMessageMetadata& _) mutable {
		if (OperationResponse.IsFailure())
		{
			const PlayerDataResult UnknownError{ TInPlaceType<PlayerDataError>{}, PlayerDataError::Unknown };
			Delegate.Execute(UnknownError);
		}
		else
		{
			PlayerDataResult SuccessResult{ TInPlaceType<FPragma_PlayerData_PlayerDataResponseProto_Payload>{}, MoveTemp(OperationResponse.Payload().Ext.Payload) };
			if (!PlayerDataCache->CanProcessSummary(OperationResponse.Payload().UpdateSummary))
			{
				auto Version = OperationResponse.Payload().UpdateSummary.Version;
				SyncVersion(Version, FOnSyncVersionComplete::CreateWeakLambda(this, [this, Version, SuccessResult = MoveTemp(SuccessResult), Delegate = MoveTemp(Delegate)](TOptional<PlayerDataSyncError> Error) {
					if (Error)
					{
						const PlayerDataResult FailedToUpdateError{ TInPlaceType<PlayerDataError>{}, PlayerDataError::FailedToUpdateCache };
						Delegate.Execute(FailedToUpdateError);
					}
					else
					{
						ensureMsgf(PlayerDataCache->HasVersion(Version), TEXT("PlayerDataService::SyncVersion returned success, but didn't populate cache with the requested version."));
						Delegate.Execute(SuccessResult);
					}
				}));
			}
			else
			{
				PlayerDataCache->UpdateFromSummary(OperationResponse.Payload().UpdateSummary);
				Delegate.Execute(SuccessResult);
			}
		}
	}));
}
#endif

void UPragmaPlayerDataService::SyncVersion(int Version, FOnSyncVersionComplete OnComplete)
{
	if (PlayerDataCache->HasVersion(Version))
	{
		(void)OnComplete.ExecuteIfBound({});
		return;
	}

	auto& DelegatesForVersion = PendingSyncs.FindOrAdd(Version);
	DelegatesForVersion.Emplace(MoveTemp(OnComplete));
	DoSyncVersion();
}

void UPragmaPlayerDataService::DoSyncVersion()
{
	if (SyncVersionInFlight)
		return;

	SyncVersionInFlight = true;
	auto CurrentVersion = PlayerDataCache->GetVersion();

	FPragma_PlayerData_GetV1Request Request;
	if (CurrentVersion)
		Request.CurrentVersion.SetValue(*CurrentVersion);
	PlayerDataServiceRaw->GetV1(Request, UPragmaPlayerDataServiceRaw::FGetV1Delegate::CreateWeakLambda(this, [this, CurrentVersion](TPragmaResult<FPragma_PlayerData_GetV1Response> Response, const FPragmaMessageMetadata&) {
		SyncVersionInFlight = false;
		const TOptional<int> ResponseVersion = Response.IsSuccessful() ? Response.Payload().PlayerData.Version : TOptional<int>();
		const bool			 bSameVersionCameBack = CurrentVersion.IsSet() && CurrentVersion == ResponseVersion;
		if (bSameVersionCameBack || Response.IsFailure())
		{
			const auto SyncError = bSameVersionCameBack ? PlayerDataSyncError::DataUpToDateButDidntGetRequestedVersion : PlayerDataSyncError::Unknown;
			for (auto& VersionToDelegates : PendingSyncs)
			{
				for (auto& Delegate : VersionToDelegates.Value)
					(void)Delegate.ExecuteIfBound(SyncError);
			}
			PendingSyncs.Empty();
		}
		else
		{
			const auto& PlayerData = Response.Payload().PlayerData;
			if (!PlayerDataCache->HasVersion(PlayerData.Version))
			{
				PlayerDataCache->UpdateFromFull(PlayerData);
				OnPlayerDataUpdated.Broadcast();
				DispatchPendingSyncs();
			}
		}
		if (PendingSyncs.Num() != 0)
			DoSyncVersion();
	}));
}

void UPragmaPlayerDataService::DispatchPendingSyncs()
{
	TArray<int> Versions;
	PendingSyncs.GetKeys(Versions);
	for (auto Version : Versions)
	{
		if (!PlayerDataCache->HasVersion(Version))
			break;
		for (auto& Delegate : PendingSyncs[Version])
			(void)Delegate.ExecuteIfBound({});
		PendingSyncs.Remove(Version);
	}
}

void UPragmaPlayerDataService::HandleOnPlayerDataUpdatedV1Notification(FPragma_PlayerData_PlayerDataUpdatedV1Notification Notification, const FPragmaMessageMetadata& /*PragmaMessageMetadata*/)
{
	if (PlayerDataCache->HasVersion(Notification.UpdateSummary.Version))
	{
		return;
	}

	if (PlayerDataCache->CanProcessSummary(Notification.UpdateSummary))
	{
		PlayerDataCache->UpdateFromSummary(Notification.UpdateSummary);
		OnPlayerDataUpdated.Broadcast();
	}
	else
	{
		int Version = Notification.UpdateSummary.Version;
		SyncVersion(Version, FOnSyncVersionComplete::CreateWeakLambda(this, [this, Version](TOptional<PlayerDataSyncError> Error) {
			if (Error)
			{
				ensureMsgf(PlayerDataCache->HasVersion(Version), TEXT("Failed to apply notification, what should happen here?"));
			}
			else
			{
				ensureMsgf(PlayerDataCache->HasVersion(Version), TEXT("PlayerDataService::SyncVersion returned success, but didn't populate cache with the requested version."));
			}
		}));
	}
}

void UPragmaPlayerDataPartnerService::SetDependencies(UPragmaPlayerDataPartnerServiceRaw* PlayerDataRaw)
{
	PlayerDataServiceRaw = PlayerDataRaw;
#ifndef NOT_USING_PRAGMA_PLAYER_DATA
	CreateSubServices(PragmaPartnerPlayerData::FPlayerDataRequestDelegate::CreateWeakLambda(this, [this](const FString& PlayerId, FPragma_PlayerData_PlayerDataRequestProto_Payload Payload, FOnPlayerDataResult Delegate) {
		Request(PlayerId, MoveTemp(Payload), MoveTemp(Delegate));
	}));
#endif
}

#ifndef NOT_USING_PRAGMA_PLAYER_DATA
void UPragmaPlayerDataPartnerService::Request(const FString& PlayerId, FPragma_PlayerData_PlayerDataRequestProto_Payload Payload, FOnPlayerDataResult Delegate)
{
	if (!PlayerDataServiceRaw)
		return;

	FPragma_PlayerData_DoOperationPartnerV1Request Request{ PlayerId };
	Request.Ext.Payload = MoveTemp(Payload);

	PlayerDataServiceRaw->DoOperationPartnerV1(Request, UPragmaPlayerDataPartnerServiceRaw::FDoOperationPartnerV1Delegate::CreateWeakLambda(this, [this, Delegate = MoveTemp(Delegate)](TPragmaResult<FPragma_PlayerData_DoOperationPartnerV1Response> OperationResponse, const FPragmaMessageMetadata& _) {
		if (OperationResponse.IsFailure())
		{
			const PlayerDataResult UnknownError{ TInPlaceType<PlayerDataError>{}, PlayerDataError::Unknown };
			Delegate.Execute(UnknownError);
		}
		else
		{
			const PlayerDataResult SuccessResult{ TInPlaceType<FPragma_PlayerData_PlayerDataResponseProto_Payload>{}, MoveTemp(OperationResponse.Payload().Ext.Payload) };
			Delegate.Execute(SuccessResult);
		}
	}));
}
#endif
