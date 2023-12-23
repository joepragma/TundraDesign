#pragma once

#include "PragmaResult.h"
#include "Async/Future.h"
#include "PragmaServerApi.h"
#include "PragmaTimerManager.h"
#include "PragmaMessageMetadata.h"
#include "Dto/PragmaFleetPartnerServiceRaw.h"
#include "Dto/PragmaGameInstanceRpcDto.h"
#include "Dto/PragmaMatchCapacityPartnerServiceRaw.h"
#include "Dto/PragmaGameInstancePartnerServiceRaw.h"

#include "PragmaMatchApi.generated.h"

class IPragmaTimerManager;
struct FPragma_GameInstance_MatchKeepAliveV1Request;

UCLASS()
class PRAGMASDK_API UPragmaMatchApi final : public UPragmaServerApi
{
	GENERATED_BODY()

public:
	void SetDependencies(
		UPragmaGameInstancePartnerServiceRaw* InGameInstanceRaw,
		UPragmaFleetPartnerServiceRaw* InFleetRaw,
		UPragmaMatchCapacityPartnerServiceRaw* InMatchCapacityRaw,
		TSharedPtr<IPragmaTimerManager> TimerManager);

	////////////////////////////////////////////////////////////////
	// Events
	////////////////////////////////////////////////////////////////
	
	//
	// Event that is fired to provide game instance data. Do all necessary work to start a new game instance, then
	// call ConnectPlayers to continue.
	//
	DECLARE_EVENT_OneParam(UPragmaMatchApi, FGameStartEvent, FPragma_GameInstance_GameStart /* GameStart */);
	FGameStartEvent OnGameStart;

	//
	// Event that is fired when an error occurs while attempting to start a game instance. The game server will not
	// continue reporting capacity or receiving new game instances until StartReportCapacityPolling is called again.
	//
	DECLARE_EVENT_OneParam(UPragmaMatchApi, FOnGameStartFailedEvent, FPragmaError /* Error */);
	FOnGameStartFailedEvent OnGameStartFailed;

	//
	// Event that is fired if the platform fails to process a keep alive request. Keep alive polling will stop.
	// Calling EndGame will not cause this event to fire.
	//
	DECLARE_EVENT_TwoParams(UPragmaMatchApi, FKeepAliveFailedEvent, FString /* GameInstanceId */, FPragmaError /* Error */);
	FKeepAliveFailedEvent OnKeepAliveFailed;

	//
	// Event that is fired if the platform has sent new players to the game server to add to the game. Upon
	// receiving, the game server should invoke ConnectMorePlayers with prepared details for the provided new
	// players for the provided game instance id.
	//
	DECLARE_EVENT_TwoParams(UPragmaMatchApi, FAddPlayersEvent, FString /* GameInstanceId */,
		TArray<FPragma_GameInstance_GameServerPlayer> /* GameServerPlayerArray */);
	FAddPlayersEvent OnAddPlayers;

	//
	// Event that is fired when the given game instance enters matchmaking.
	//
	DECLARE_EVENT_OneParam(UPragmaMatchApi, FEnteredMatchmakingEvent, FString /* GameInstanceId */);
	FEnteredMatchmakingEvent OnGameInstanceEnteredMatchmaking;

	//
	// Event that is fired when the given game instance enters matchmaking.
	//
	DECLARE_EVENT_OneParam(UPragmaMatchApi, FLeftMatchmakingEvent, FString /* GameInstanceId */);
	FLeftMatchmakingEvent OnGameInstanceLeftMatchmaking;

	//
	// Event that is fired from the platform when a player has declined to reconnect to the game instance they were in.
	// Includes the game instance ID and player ID respectively.
	//
	DECLARE_EVENT_TwoParams(UPragmaMatchApi, FPlayerDeclinedReconnectEvent, FString /* GameInstanceId */, FString /* PlayerId */);
	FPlayerDeclinedReconnectEvent OnPlayerDeclinedReconnect;

	////////////////////////////////////////////////////////////////
	// Actions
	////////////////////////////////////////////////////////////////

	//
	// Request the game start data for a game instance. Use this method to start a game when Pragma does not manage your game server capacity.
	//
	void RequestStartGame(FString& GameInstanceId);

	//
	// Start a background task that polls the platform with ReportCapacity RPC calls.
	//
	// When a game is allocated to this game server, its game data will be passed onto the OnGameStart event.
	// On any error, the error will be passed onto the OnTemporaryErrorEvent event and no more ReportCapacity RPCs will be sent.
	//
	// The MaxGameInstanceCount value should match the gameCapacityPerServer value configured in the platform ServerPoolManagementPolicy
	// assigned for this server.
	//
	// Set the timeout for how long to wait in seconds for the first game allocation. If the timeout is reached without a game allocation,
	// a OnGameStartFailed event will be triggered. A timeout <= 0 will poll forever.
    // 
	void StartReportCapacityPolling(FString ServerId, FString ServerPoolId, float Timeout, int32 MaxGameInstanceCount = 1);
	
	//
	// Note: Use the other StartReportCapacityPolling overload after migrating to FleetService on the platform.
	//
	void StartReportCapacityPolling(FString ServerId, FString GameVersion, FString GameServerZone, float Timeout, int32 MaxGameInstanceCount = 1);
	
	//
	// Release one unit of game capacity. If the used capacity is currently full, this will result in ReportCapacity RPC calls telling
	// the platform it is available for a new game.
	//
	void ReportFreedCapacity();

	//
	// Update the server's max game instance count used in ReportCapacity polling.
	//
	void UpdateMaxGameInstanceCount(int32 MaxGameInstanceCount);

	//
	// Connect the initial players for a game instance.
	//
	void ConnectPlayers(
		const FString& GameInstanceId,
        const TArray<FPragma_GameInstance_PlayerConnectionDetails>& PlayerConnectionDetails,
        const FString& Hostname,
        const int Port,
        UPragmaGameInstancePartnerServiceRaw::FConnectPlayersV1Delegate Delegate);
	TFuture<TPragmaResult<FPragma_GameInstance_ConnectPlayersV1Response>> ConnectPlayers(
		const FString& GameInstanceId,
		const TArray<FPragma_GameInstance_PlayerConnectionDetails>& PlayerConnectionDetails,
		const FString& Hostname,
		const int Port);

	//
	// Connect more players for a game instance.
	//
	void ConnectMorePlayers(
		const FString& GameInstanceId,
		const TArray<FPragma_GameInstance_PlayerConnectionDetails>& PlayerConnectionDetailsArray,
		const FString& Hostname,
		const int Port,
		UPragmaGameInstancePartnerServiceRaw::FConnectMorePlayersV1Delegate Delegate);
	TFuture<TPragmaResult<FPragma_GameInstance_ConnectMorePlayersV1Response>> ConnectMorePlayers(
		const FString& GameInstanceId,
		const TArray<FPragma_GameInstance_PlayerConnectionDetails>& PlayerConnectionDetailsArray,
		const FString& Hostname,
		const int Port);

	//
	// Verify a player's connection token.
	//
	void VerifyPlayer(
		const FString& GameInstanceId,
		const FString& PlayerId,
		const FString& ConnectionToken,
		UPragmaGameInstancePartnerServiceRaw::FVerifyPlayerPartnerV1Delegate Delegate);
	TFuture<TPragmaResult<FPragma_GameInstance_VerifyPlayerPartnerV1Response>> VerifyPlayer(
		const FString& GameInstanceId,
		const FString& PlayerId,
		const FString& ConnectionToken);

	//
	// Update data for a game instance.
	//
	void UpdateGameInstance(
		const FString& GameInstanceId,
        const FPragma_GameInstance_ExtUpdateGameInstanceRequest& RequestExt,
        const UPragmaGameInstancePartnerServiceRaw::FUpdateGameInstanceV1Delegate& Delegate);
	TFuture<TPragmaResult<FPragma_GameInstance_UpdateGameInstanceV1Response>> UpdateGameInstance(
		const FString& GameInstanceId,
		const FPragma_GameInstance_ExtUpdateGameInstanceRequest& RequestExt);

	//
	// Remove players from a game instance.
	//
	void RemovePlayers(
		const FString& GameInstanceId,
		const TArray<FPragma_GameInstance_PlayerToRemove>& Players,
		const FPragma_GameInstance_ExtRemovePlayersRequest& Ext,
		UPragmaGameInstancePartnerServiceRaw::FRemovePlayersV1Delegate Delegate);
	TFuture<TPragmaResult<FPragma_GameInstance_RemovePlayersV1Response>> RemovePlayers(
		const FString& GameInstanceId,
		const TArray<FPragma_GameInstance_PlayerToRemove>& Players,
		const FPragma_GameInstance_ExtRemovePlayersRequest& Ext);

	//
	// End a game instance.
	//
	void EndGame(
		const FString& GameInstanceId,
		const TArray<FPragma_GameInstance_PlayerGameResult>& PlayerGameResults,
		const FPragma_GameInstance_ExtEndGameRequest& Ext,
		UPragmaGameInstancePartnerServiceRaw::FEndGameV1Delegate Delegate);
	TFuture<TPragmaResult<FPragma_GameInstance_EndGameV1Response>> EndGame(
		const FString& GameInstanceId,
		const TArray<FPragma_GameInstance_PlayerGameResult>& PlayerGameResults,
		const FPragma_GameInstance_ExtEndGameRequest Ext);

	//
	// Enter a game instance into matchmaking to find more players.
	//
	void EnterMatchmaking(
		const FString& GameInstanceId,
	    UPragmaGameInstancePartnerServiceRaw::FEnterMatchmakingV1Delegate Delegate);
	TFuture<TPragmaResult<FPragma_GameInstance_EnterMatchmakingV1Response>> EnterMatchmaking(
		const FString& GameInstanceId);

	//
	// Remove a game instance from matchmaking.
	//
	void LeaveMatchmaking(
		const FString& GameInstanceId,
	    UPragmaGameInstancePartnerServiceRaw::FLeaveMatchmakingV1Delegate Delegate);
	TFuture<TPragmaResult<FPragma_GameInstance_LeaveMatchmakingV1Response>> LeaveMatchmaking(
		const FString& GameInstanceId);

	// ~~~ Used for testing ~~~
	FPragmaTimerHandle& GetReportCapacityTimerHandle();

private:
	struct FTimerInfo
	{
		FPragmaTimerHandle Handle;
		FPragmaTimerDelegate Delegate;
	};

	struct FLocalGameInstanceInfo
	{
		FTimerInfo TimerInfo;
	};

	UPROPERTY()
	UPragmaGameInstancePartnerServiceRaw* GameInstanceServiceRaw = nullptr;
	UPROPERTY()
	UPragmaFleetPartnerServiceRaw* FleetRaw = nullptr;
	UPROPERTY()
	UPragmaMatchCapacityPartnerServiceRaw* MatchCapacityRaw = nullptr;

	TMap<FString, FLocalGameInstanceInfo> GameInstanceIdToLocalInfo;
	TSharedPtr<IPragmaTimerManager> TimerManager;
	int64 KeepAliveDelay{0};
	FPragma_Fleet_ReportCapacityV1Request ReportCapacityRequest;
	UPragmaFleetPartnerServiceRaw::FReportCapacityV1Delegate ReportCapacityResponseDelegate;
	FPragma_MatchCapacity_ReportCapacityV2Request ReportCapacityMatchRequest;
	UPragmaMatchCapacityPartnerServiceRaw::FReportCapacityV2Delegate ReportCapacityMatchResponseDelegate;
	FPragmaTimerHandle ReportCapacityTimerHandle;
	float ReportCapacityTimeout{0.f};
	float SecondsPolling{0.f};
	bool HasEverBeenAllocatedAGame{false};
	bool UseMatchCapacityService{false};

	////////////////////////////////////////////////////////////////
	// Notification Handlers
	////////////////////////////////////////////////////////////////

	void HandleGameInstanceEnteredMatchmaking(
		FPragma_GameInstance_GameInstanceEnteredMatchmakingV1Notification Notification,
		const FPragmaMessageMetadata& Metadata) const;
	void HandleGameInstanceLeftMatchmaking(
		FPragma_GameInstance_GameInstanceLeftMatchmakingV1Notification Notification,
		const FPragmaMessageMetadata& Metadata) const;
	void HandleAddPlayers(
		FPragma_GameInstance_AddPlayersV1Notification Notification,
		const FPragmaMessageMetadata& Metadata) const;
	void HandlePlayerDeclinedReconnect(
		FPragma_GameInstance_PlayerDeclinedReconnectV1Notification Notification,
		const FPragmaMessageMetadata& Metadata) const;

	////////////////////////////////////////////////////////////////
	// Private Functions
	////////////////////////////////////////////////////////////////
	
	void KeepAliveLoop(FString GameInstanceId);
	void CleanUpKeepAliveLoop(FString GameInstanceId, FTimerInfo& Info);
	void ReportCapacity() const;
	void ReportMatchCapacity() const;
	void HandleReportCapacityResponse(TPragmaResult<FPragma_Fleet_ReportCapacityV1Response> ReportCapacityResult,
		const FPragmaMessageMetadata& Metadata);
	void HandleReportCapacityMatchResponse(TPragmaResult<FPragma_MatchCapacity_ReportCapacityV2Response> ReportCapacityResult,
		const FPragmaMessageMetadata& Metadata);
	void ContinueReportCapacityPolling(const float NextHeartbeatSeconds);
	void StopReportCapacityPollingWithError(const FPragmaError& Error) const;
};
