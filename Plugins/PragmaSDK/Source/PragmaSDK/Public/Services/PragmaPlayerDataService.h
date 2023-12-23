#pragma once
#include "PragmaPlayerApi.h"
#include "PragmaPlayerDataCache.h"
#include "PragmaServerApi.h"

#include "Dto/PragmaPlayerDataPartnerServiceRaw.h"
#include "Dto/PlayerDataSubServiceCollectionPlayer.h"
#include "Dto/PlayerDataSubServiceCollectionPartner.h"
#include "PragmaPlayerDataService.generated.h"

class UPragmaPlayerDataService;
struct FPragma_PlayerData_EchoResponse;
struct FPragma_PlayerData_AddNumberResponse;

enum class PlayerDataSyncError
{
	Unknown,
	DataUpToDateButDidntGetRequestedVersion
};

DECLARE_DELEGATE_OneParam(FOnForceGetPlayerDataComplete, TPragmaResult<FPragma_PlayerData_PlayerData>);
DECLARE_DELEGATE_OneParam(FOnSyncVersionComplete, TOptional<PlayerDataSyncError>);
UCLASS()
class PRAGMASDK_API UPragmaPlayerDataService final : public UPragmaPlayerApi, public PragmaPlayerData::PlayerDataOperationsSubServiceCollection
{
	GENERATED_BODY()

public:
	virtual void OnInitialized() override;

	void SetDependencies(UPragmaPlayerDataServiceRaw* PlayerDataRaw);
	void ClearCache() const;

#ifndef NOT_USING_PRAGMA_PLAYER_DATA
	void Request(FPragma_PlayerData_PlayerDataRequestProto_Payload Payload, FOnPlayerDataResult Delegate);
#endif

	TWeakPtr<FPlayerDataCache> GetCache() const { return PlayerDataCache; }

	// TODO: understand copy rules for unreal delegates wrt lifetime of objects captured by bound lambdas
	void SyncVersion(int Version, FOnSyncVersionComplete OnComplete);

	// This event is here for testing purposes only. Worth discussing if this gets used elsewhere
	DECLARE_MULTICAST_DELEGATE(FOnPlayerDataUpdated);
	FOnPlayerDataUpdated OnPlayerDataUpdated;

private:
	TSortedMap<int, TArray<FOnSyncVersionComplete>> PendingSyncs;

	void DoSyncVersion();
	void DispatchPendingSyncs();

	bool SyncVersionInFlight = false;

	void HandleOnPlayerDataUpdatedV1Notification(
		FPragma_PlayerData_PlayerDataUpdatedV1Notification Notification,
		const FPragmaMessageMetadata& /*PragmaMessageMetadata*/);
	UPROPERTY()
	UPragmaPlayerDataServiceRaw* PlayerDataServiceRaw;

	TSharedPtr<FPlayerDataCache> PlayerDataCache = MakeShared<FPlayerDataCache>();
};

UCLASS()
class PRAGMASDK_API UPragmaPlayerDataPartnerService final : public UPragmaServerApi, public PragmaPartnerPlayerData::PlayerDataOperationsSubServiceCollection
{
	GENERATED_BODY()

public:
	void SetDependencies(UPragmaPlayerDataPartnerServiceRaw* PlayerDataRaw);

#ifndef NOT_USING_PRAGMA_PLAYER_DATA
	void Request(const FString& PlayerId, FPragma_PlayerData_PlayerDataRequestProto_Payload Payload, FOnPlayerDataResult Delegate);
#endif

private:
	UPROPERTY()
	UPragmaPlayerDataPartnerServiceRaw* PlayerDataServiceRaw;
};
