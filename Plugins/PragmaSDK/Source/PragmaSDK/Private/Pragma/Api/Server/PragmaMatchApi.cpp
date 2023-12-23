#include "Pragma/Api/Server/PragmaMatchApi.h"

#include "Dto/PragmaFleetPartnerServiceRaw.h"
#include "Pragma/Api/Player/PragmaGameLoopApi.h"
#include "Services/PragmaSessionService.h"

void UPragmaMatchApi::SetDependencies(
	UPragmaGameInstancePartnerServiceRaw* InGameInstanceRaw,
	UPragmaFleetPartnerServiceRaw* InFleetRaw,
	UPragmaMatchCapacityPartnerServiceRaw* InMatchCapacityRaw,
	TSharedPtr<IPragmaTimerManager> InTimerManager)
{
	GameInstanceServiceRaw = InGameInstanceRaw;
	FleetRaw = InFleetRaw;
	MatchCapacityRaw = InMatchCapacityRaw;
	TimerManager = MoveTemp(InTimerManager);

	GameInstanceServiceRaw->OnGameInstanceEnteredMatchmakingV1.AddUObject(this,
		&UPragmaMatchApi::HandleGameInstanceEnteredMatchmaking);
	GameInstanceServiceRaw->OnGameInstanceLeftMatchmakingV1.AddUObject(this,
		&UPragmaMatchApi::HandleGameInstanceLeftMatchmaking);
	GameInstanceServiceRaw->OnAddPlayersV1.AddUObject(this,
		&UPragmaMatchApi::HandleAddPlayers);
	GameInstanceServiceRaw->OnPlayerDeclinedReconnectV1.AddUObject(this,
		&UPragmaMatchApi::HandlePlayerDeclinedReconnect);
}

////////////////////////////////////////////////////////////////
// Actions.
////////////////////////////////////////////////////////////////

void UPragmaMatchApi::StartReportCapacityPolling(FString ServerId, FString ServerPoolId, float Timeout, int32 MaxGameInstanceCount)
{
	ReportCapacityRequest.ServerId = MoveTemp(ServerId);
	ReportCapacityRequest.ServerPoolId = MoveTemp(ServerPoolId);
	ReportCapacityRequest.UsedCapacity = 0;
	ReportCapacityRequest.TotalCapacity = MaxGameInstanceCount;
	ReportCapacityResponseDelegate.BindUObject(this, &UPragmaMatchApi::HandleReportCapacityResponse);
	ReportCapacityTimeout = Timeout;
	SecondsPolling = 0.f;

	ReportCapacity();
}

// Note: Use the other StartReportCapacityPolling overload after migrating to FleetService on the platform.
void UPragmaMatchApi::StartReportCapacityPolling(FString ServerId, FString GameVersion, FString GameServerZone, const float Timeout, const int32 MaxGameInstanceCount)
{
	UseMatchCapacityService = true;

	ReportCapacityMatchRequest.ServerId = MoveTemp(ServerId);
	ReportCapacityMatchRequest.GameVersion = MoveTemp(GameVersion);
	ReportCapacityMatchRequest.GameServerZone = MoveTemp(GameServerZone);
	ReportCapacityMatchRequest.UsedCapacity = 0;
	ReportCapacityMatchRequest.TotalCapacity = MaxGameInstanceCount;
	ReportCapacityMatchResponseDelegate.BindUObject(this, &UPragmaMatchApi::HandleReportCapacityMatchResponse);
	ReportCapacityTimeout = Timeout;
	SecondsPolling = 0.f;

	ReportMatchCapacity();
}

void UPragmaMatchApi::ReportFreedCapacity()
{
	if (UseMatchCapacityService)
	{
		ReportCapacityMatchRequest.UsedCapacity -= 1;
		if(ReportCapacityMatchRequest.UsedCapacity < 0) { ReportCapacityMatchRequest.UsedCapacity = 0; }
		PRAGMA_LOG(VeryVerbose, "Releasing one unit of used capacity. New capacity: %d/%d", ReportCapacityMatchRequest.UsedCapacity, ReportCapacityMatchRequest.TotalCapacity);
	} else
	{
		ReportCapacityRequest.UsedCapacity -= 1;
		if(ReportCapacityRequest.UsedCapacity < 0) { ReportCapacityRequest.UsedCapacity = 0; }
		PRAGMA_LOG(VeryVerbose, "Releasing one unit of used capacity. New capacity: %d/%d", ReportCapacityRequest.UsedCapacity, ReportCapacityRequest.TotalCapacity);
	}
}

void UPragmaMatchApi::UpdateMaxGameInstanceCount(const int32 MaxGameInstanceCount)
{
	if (UseMatchCapacityService)
	{
		ReportCapacityMatchRequest.TotalCapacity = MaxGameInstanceCount;
		PRAGMA_LOG(VeryVerbose, "Updating max game instance count to %d. New capacity: %d/%d", MaxGameInstanceCount, ReportCapacityMatchRequest.UsedCapacity, ReportCapacityMatchRequest.TotalCapacity);
	} else
	{
		ReportCapacityRequest.TotalCapacity = MaxGameInstanceCount;
		PRAGMA_LOG(VeryVerbose, "Updating max game instance count to %d. New capacity: %d/%d", MaxGameInstanceCount, ReportCapacityRequest.UsedCapacity, ReportCapacityRequest.TotalCapacity);
	}
}

void UPragmaMatchApi::ConnectPlayers(const FString& GameInstanceId,
	const TArray<FPragma_GameInstance_PlayerConnectionDetails>& PlayerConnectionDetails,
	const FString& Hostname,
	const int Port,
	UPragmaGameInstancePartnerServiceRaw::FConnectPlayersV1Delegate Delegate)
{
	const auto Request = FPragma_GameInstance_ConnectPlayersV1Request {
		GameInstanceId,
		Hostname,
		Port,
		PlayerConnectionDetails
	};
	GameInstanceServiceRaw->ConnectPlayersV1(Request,
		UPragmaGameInstancePartnerServiceRaw::FConnectPlayersV1Delegate::CreateWeakLambda(
			this,
			[this, Request, Delegate](const TPragmaResult<FPragma_GameInstance_ConnectPlayersV1Response>& Result,
			const FPragmaMessageMetadata& Metadata) {
				if (Result.IsSuccessful())
				{
					KeepAliveDelay = Result.Payload().KeepAliveIntervalMillis;
					if (Result.Payload().EnableKeepAlive)
					{
						KeepAliveLoop(Request.GameInstanceId);
					}
				}
				auto _ = Delegate.ExecuteIfBound(Result, Metadata);
			}
		));
}

TFuture<TPragmaResult<FPragma_GameInstance_ConnectPlayersV1Response>> UPragmaMatchApi::ConnectPlayers(
	const FString& GameInstanceId,
	const TArray<FPragma_GameInstance_PlayerConnectionDetails>& PlayerConnectionDetails,
	const FString& Hostname,
	const int Port)
{
	auto Promise = MakeShared<TPromise<TPragmaResult<FPragma_GameInstance_ConnectPlayersV1Response>>>();
	ConnectPlayers(GameInstanceId, PlayerConnectionDetails, Hostname, Port,  UPragmaGameInstancePartnerServiceRaw::FConnectPlayersV1Delegate::CreateWeakLambda(this,
		[this, Promise](TPragmaResult<FPragma_GameInstance_ConnectPlayersV1Response> Result,
		const FPragmaMessageMetadata&)
		{
			Promise->SetValue(MoveTemp(Result));
		}));
	return Promise->GetFuture();
}

void UPragmaMatchApi::ConnectMorePlayers(
	const FString& GameInstanceId,
	const TArray<FPragma_GameInstance_PlayerConnectionDetails>& PlayerConnectionDetailsArray,
	const FString& Hostname,
	const int Port,
	UPragmaGameInstancePartnerServiceRaw::FConnectMorePlayersV1Delegate Delegate)
{
	const auto Request = FPragma_GameInstance_ConnectMorePlayersV1Request{
		GameInstanceId, Hostname, Port, PlayerConnectionDetailsArray
	};
	GameInstanceServiceRaw->ConnectMorePlayersV1(Request,
		UPragmaGameInstancePartnerServiceRaw::FConnectMorePlayersV1Delegate::CreateWeakLambda(this,
			[this, Delegate](const TPragmaResult<FPragma_GameInstance_ConnectMorePlayersV1Response>& Result,
			const FPragmaMessageMetadata& Metadata)
			{
				auto _ = Delegate.ExecuteIfBound(Result, Metadata);
			}
		));
}

TFuture<TPragmaResult<FPragma_GameInstance_ConnectMorePlayersV1Response>>
UPragmaMatchApi::ConnectMorePlayers(
	const FString& GameInstanceId,
	const TArray<FPragma_GameInstance_PlayerConnectionDetails>& PlayerConnectionDetailsArray,
	const FString& Hostname,
	const int Port)
{
	auto Promise = MakeShared<TPromise<TPragmaResult<FPragma_GameInstance_ConnectMorePlayersV1Response>>>();
	ConnectMorePlayers(GameInstanceId, PlayerConnectionDetailsArray, Hostname, Port,
		UPragmaGameInstancePartnerServiceRaw::FConnectMorePlayersV1Delegate::CreateWeakLambda(
			this, [this, Promise](TPragmaResult<FPragma_GameInstance_ConnectMorePlayersV1Response> Result,
			const FPragmaMessageMetadata&)
			{
				Promise->SetValue(MoveTemp(Result));
			}));
	return Promise->GetFuture();
}

void UPragmaMatchApi::VerifyPlayer(
	const FString& GameInstanceId,
	const FString& PlayerId,
	const FString& ConnectionToken,
	UPragmaGameInstancePartnerServiceRaw::FVerifyPlayerPartnerV1Delegate Delegate)
{
	const auto Request = FPragma_GameInstance_VerifyPlayerPartnerV1Request{
		GameInstanceId, PlayerId, ConnectionToken
	};
	GameInstanceServiceRaw->VerifyPlayerPartnerV1(Request,
		UPragmaGameInstancePartnerServiceRaw::FVerifyPlayerPartnerV1Delegate::CreateWeakLambda(this,
			[this, Delegate](const TPragmaResult<FPragma_GameInstance_VerifyPlayerPartnerV1Response>& Result,
			const FPragmaMessageMetadata& Metadata)
			{
				auto _ = Delegate.ExecuteIfBound(Result, Metadata);
			}
		));
}

TFuture<TPragmaResult<FPragma_GameInstance_VerifyPlayerPartnerV1Response>>
UPragmaMatchApi::VerifyPlayer(const FString& GameInstanceId, const FString& PlayerId, const FString& ConnectionToken)
{
	auto Promise = MakeShared<TPromise<TPragmaResult<FPragma_GameInstance_VerifyPlayerPartnerV1Response>>>();
	VerifyPlayer(GameInstanceId, PlayerId, ConnectionToken,
		UPragmaGameInstancePartnerServiceRaw::FVerifyPlayerPartnerV1Delegate::CreateWeakLambda(
			this, [this, Promise](TPragmaResult<FPragma_GameInstance_VerifyPlayerPartnerV1Response> Result,
			const FPragmaMessageMetadata&)
			{
				Promise->SetValue(MoveTemp(Result));
			}));
	return Promise->GetFuture();
}

void UPragmaMatchApi::UpdateGameInstance(const FString& GameInstanceId,
	const FPragma_GameInstance_ExtUpdateGameInstanceRequest& RequestExt,
	const UPragmaGameInstancePartnerServiceRaw::FUpdateGameInstanceV1Delegate& Delegate)
{
	const auto Request = FPragma_GameInstance_UpdateGameInstanceV1Request {GameInstanceId, RequestExt};
	GameInstanceServiceRaw->UpdateGameInstanceV1(Request,
		UPragmaGameInstancePartnerServiceRaw::FUpdateGameInstanceV1Delegate::CreateWeakLambda(
			this,
			[this, Delegate](const TPragmaResult<FPragma_GameInstance_UpdateGameInstanceV1Response>& Result,
			const FPragmaMessageMetadata& Metadata) {
				auto _ = Delegate.ExecuteIfBound(Result, Metadata);
			}
		));
}

TFuture<TPragmaResult<FPragma_GameInstance_UpdateGameInstanceV1Response>> UPragmaMatchApi::UpdateGameInstance(
	const FString& GameInstanceId,
	const FPragma_GameInstance_ExtUpdateGameInstanceRequest& RequestExt)
{
	auto Promise = MakeShared<TPromise<TPragmaResult<FPragma_GameInstance_UpdateGameInstanceV1Response>>>();
	UpdateGameInstance(GameInstanceId, RequestExt,  UPragmaGameInstancePartnerServiceRaw::FUpdateGameInstanceV1Delegate::CreateWeakLambda(this,
		[this, Promise](TPragmaResult<FPragma_GameInstance_UpdateGameInstanceV1Response> Result,
		const FPragmaMessageMetadata&)
		{
			Promise->SetValue(MoveTemp(Result));
		}));
	return Promise->GetFuture();
}

void UPragmaMatchApi::RemovePlayers(
	const FString& GameInstanceId,
	const TArray<FPragma_GameInstance_PlayerToRemove>& Players,
	const FPragma_GameInstance_ExtRemovePlayersRequest& Ext,
	UPragmaGameInstancePartnerServiceRaw::FRemovePlayersV1Delegate Delegate)
{
	const auto Request = FPragma_GameInstance_RemovePlayersV1Request{GameInstanceId, Players, Ext};
	GameInstanceServiceRaw->RemovePlayersV1(Request,
		UPragmaGameInstancePartnerServiceRaw::FRemovePlayersV1Delegate::CreateWeakLambda(
			this,
			[this, Delegate](const TPragmaResult<FPragma_GameInstance_RemovePlayersV1Response>& Result,
			const FPragmaMessageMetadata& Metadata)
			{
				auto _ = Delegate.ExecuteIfBound(Result, Metadata);
			}
		));
}

TFuture<TPragmaResult<FPragma_GameInstance_RemovePlayersV1Response>> UPragmaMatchApi::RemovePlayers(
	const FString& GameInstanceId,
	const TArray<FPragma_GameInstance_PlayerToRemove>& Players,
	const FPragma_GameInstance_ExtRemovePlayersRequest& Ext)
{
	auto Promise = MakeShared<TPromise<TPragmaResult<FPragma_GameInstance_RemovePlayersV1Response>>>();
	RemovePlayers(GameInstanceId, Players, Ext,
		UPragmaGameInstancePartnerServiceRaw::FRemovePlayersV1Delegate::CreateWeakLambda(this,
			[this, Promise](TPragmaResult<FPragma_GameInstance_RemovePlayersV1Response> Result,
			const FPragmaMessageMetadata&)
			{
				Promise->SetValue(MoveTemp(Result));
			}));
	return Promise->GetFuture();
}

void UPragmaMatchApi::EndGame(
	const FString& GameInstanceId,
	const TArray<FPragma_GameInstance_PlayerGameResult>& PlayerGameResults,
	const FPragma_GameInstance_ExtEndGameRequest& Ext,
	UPragmaGameInstancePartnerServiceRaw::FEndGameV1Delegate Delegate)
{
	auto Request = FPragma_GameInstance_EndGameV1Request{
		GameInstanceId,
		PlayerGameResults,
		Ext
	};
	GameInstanceServiceRaw->EndGameV1(Request,
		UPragmaGameInstancePartnerServiceRaw::FEndGameV1Delegate::CreateWeakLambda(
			this,
			[this, Request, Delegate](const TPragmaResult<FPragma_GameInstance_EndGameV1Response>& Result,
			const FPragmaMessageMetadata& Metadata)
			{
				if (Result.IsSuccessful())
				{
					FLocalGameInstanceInfo* LocalGameInstanceInfo = GameInstanceIdToLocalInfo.Find(Request.GameInstanceId);
					if (LocalGameInstanceInfo != nullptr)
					{
						CleanUpKeepAliveLoop(Request.GameInstanceId, LocalGameInstanceInfo->TimerInfo);
					}
				}
				auto _ = Delegate.ExecuteIfBound(Result, Metadata);
			}
		));
}

TFuture<TPragmaResult<FPragma_GameInstance_EndGameV1Response>> UPragmaMatchApi::EndGame(
	const FString& GameInstanceId,
	const TArray<FPragma_GameInstance_PlayerGameResult>& PlayerGameResults,
	const FPragma_GameInstance_ExtEndGameRequest Ext)
{
	auto Promise = MakeShared<TPromise<TPragmaResult<FPragma_GameInstance_EndGameV1Response>>>();
	EndGame(GameInstanceId, PlayerGameResults, Ext,
		UPragmaGameInstancePartnerServiceRaw::FEndGameV1Delegate::CreateWeakLambda(this,
			[this, Promise](TPragmaResult<FPragma_GameInstance_EndGameV1Response> Result,
			const FPragmaMessageMetadata&)
			{
				Promise->SetValue(MoveTemp(Result));
			}));
	return Promise->GetFuture();
}

void UPragmaMatchApi::EnterMatchmaking(
	const FString& GameInstanceId,
	UPragmaGameInstancePartnerServiceRaw::FEnterMatchmakingV1Delegate Delegate)
{
	const auto Request = FPragma_GameInstance_EnterMatchmakingV1Request{GameInstanceId};
	GameInstanceServiceRaw->EnterMatchmakingV1(Request,
		UPragmaGameInstancePartnerServiceRaw::FEnterMatchmakingV1Delegate::CreateWeakLambda(this,
			[this, Delegate](const TPragmaResult<FPragma_GameInstance_EnterMatchmakingV1Response>& Result,
			const FPragmaMessageMetadata& Metadata)
			{
				auto _ = Delegate.ExecuteIfBound(Result, Metadata);
			}
		));
}

TFuture<TPragmaResult<FPragma_GameInstance_EnterMatchmakingV1Response>>
UPragmaMatchApi::EnterMatchmaking(const FString& GameInstanceId)
{
	auto Promise = MakeShared<TPromise<TPragmaResult<FPragma_GameInstance_EnterMatchmakingV1Response>>>();
	EnterMatchmaking(GameInstanceId,
		UPragmaGameInstancePartnerServiceRaw::FEnterMatchmakingV1Delegate::CreateWeakLambda(
			this, [this, Promise](TPragmaResult<FPragma_GameInstance_EnterMatchmakingV1Response> Result,
			const FPragmaMessageMetadata&)
			{
				Promise->SetValue(MoveTemp(Result));
			}));
	return Promise->GetFuture();
}

void UPragmaMatchApi::LeaveMatchmaking(const FString& GameInstanceId,
	UPragmaGameInstancePartnerServiceRaw::FLeaveMatchmakingV1Delegate Delegate)
{
	const auto Request = FPragma_GameInstance_LeaveMatchmakingV1Request{GameInstanceId};
	GameInstanceServiceRaw->LeaveMatchmakingV1(Request,
		UPragmaGameInstancePartnerServiceRaw::FLeaveMatchmakingV1Delegate::CreateWeakLambda(this,
			[this, Delegate](const TPragmaResult<FPragma_GameInstance_LeaveMatchmakingV1Response>& Result,
			const FPragmaMessageMetadata& Metadata)
			{
				auto _ = Delegate.ExecuteIfBound(Result, Metadata);
			}
		));
}

TFuture<TPragmaResult<FPragma_GameInstance_LeaveMatchmakingV1Response>>
UPragmaMatchApi::LeaveMatchmaking(const FString& GameInstanceId)
{
	auto Promise = MakeShared<TPromise<TPragmaResult<FPragma_GameInstance_LeaveMatchmakingV1Response>>>();
	LeaveMatchmaking(GameInstanceId,
		UPragmaGameInstancePartnerServiceRaw::FLeaveMatchmakingV1Delegate::CreateWeakLambda(
			this, [this, Promise](TPragmaResult<FPragma_GameInstance_LeaveMatchmakingV1Response> Result,
			const FPragmaMessageMetadata&)
			{
				Promise->SetValue(MoveTemp(Result));
			}));
	return Promise->GetFuture();
}

FPragmaTimerHandle& UPragmaMatchApi::GetReportCapacityTimerHandle()
{
	return ReportCapacityTimerHandle;
}

////////////////////////////////////////////////////////////////
// Notification Handlers.
////////////////////////////////////////////////////////////////

void UPragmaMatchApi::HandleGameInstanceEnteredMatchmaking(
	FPragma_GameInstance_GameInstanceEnteredMatchmakingV1Notification Notification,
	const FPragmaMessageMetadata&) const
{
	OnGameInstanceEnteredMatchmaking.Broadcast(Notification.GameInstanceId);
}

void UPragmaMatchApi::HandleGameInstanceLeftMatchmaking(
	FPragma_GameInstance_GameInstanceLeftMatchmakingV1Notification Notification,
	const FPragmaMessageMetadata&) const
{
	OnGameInstanceLeftMatchmaking.Broadcast(Notification.GameInstanceId);
}

void UPragmaMatchApi::HandleAddPlayers(
	const FPragma_GameInstance_AddPlayersV1Notification Notification,
	const FPragmaMessageMetadata&) const
{
	OnAddPlayers.Broadcast(Notification.GameInstanceId, Notification.GameServerPlayers);
}

void UPragmaMatchApi::HandlePlayerDeclinedReconnect(
	const FPragma_GameInstance_PlayerDeclinedReconnectV1Notification Notification,
	const FPragmaMessageMetadata&) const
{
	OnPlayerDeclinedReconnect.Broadcast(Notification.GameInstanceId, Notification.PlayerId);
}

////////////////////////////////////////////////////////////////
// Private Functions.
////////////////////////////////////////////////////////////////

void UPragmaMatchApi::KeepAliveLoop(FString GameInstanceId)
{
	auto TimerInfo = FLocalGameInstanceInfo{};
	TimerInfo.TimerInfo.Delegate.BindWeakLambda(this, [this, GameInstanceId]
	{
		const auto KeepAliveRequest = FPragma_GameInstance_GameKeepAliveV1Request{GameInstanceId};
		GameInstanceServiceRaw->GameKeepAliveV1(KeepAliveRequest,
			UPragmaGameInstancePartnerServiceRaw::FGameKeepAliveV1Delegate::CreateWeakLambda(this,
				[this, GameInstanceId](TPragmaResult<FPragma_GameInstance_GameKeepAliveV1Response> KeepAliveResult,
				const FPragmaMessageMetadata&)
				{
					const auto LocalGameInstanceInfo = GameInstanceIdToLocalInfo.Find(GameInstanceId);
					if (LocalGameInstanceInfo == nullptr)
					{
						return;
					}

					if (KeepAliveResult.IsSuccessful())
					{
						KeepAliveDelay = KeepAliveResult.Payload().KeepAliveIntervalMillis;
						if (!KeepAliveResult.Payload().EnableKeepAlive)
						{
							CleanUpKeepAliveLoop(GameInstanceId, LocalGameInstanceInfo->TimerInfo);
							return;
						}
					}
					else if (KeepAliveResult.Error().Platform() ==
						EPragma_PlatformError::GameInstanceService_UnknownGameInstanceId)
					{
						CleanUpKeepAliveLoop(GameInstanceId, LocalGameInstanceInfo->TimerInfo);
						OnKeepAliveFailed.Broadcast(GameInstanceId, KeepAliveResult.Error().Platform());
						return;
					}
					TimerManager->SetTimer(LocalGameInstanceInfo->TimerInfo.Handle,
						LocalGameInstanceInfo->TimerInfo.Delegate,
						KeepAliveDelay / 1000.f,
						false);
				}
			));
	});
	TimerManager->SetTimer(TimerInfo.TimerInfo.Handle, TimerInfo.TimerInfo.Delegate, KeepAliveDelay / 1000.f,
		false);
	GameInstanceIdToLocalInfo.Add(GameInstanceId, TimerInfo);
}

void UPragmaMatchApi::CleanUpKeepAliveLoop(FString GameInstanceId, FTimerInfo& Info)
{
	TimerManager->ClearTimer(Info.Handle);
	GameInstanceIdToLocalInfo.Remove(GameInstanceId);
}

void UPragmaMatchApi::RequestStartGame(FString& GameInstanceId)
{
	const auto Request = FPragma_GameInstance_GetGameStartDataV1Request{GameInstanceId};
	GameInstanceServiceRaw->GetGameStartDataV1(Request,
		UPragmaGameInstancePartnerServiceRaw::FGetGameStartDataV1Delegate::CreateWeakLambda(
			this, [this, GameInstanceId](
			TPragmaResult<FPragma_GameInstance_GetGameStartDataV1Response> GameStartResult,
			const FPragmaMessageMetadata&)
			{
				if (GameStartResult.IsFailure())
				{
					PRAGMA_LOG(Warning, "GetGameStartData from RequestStartGame returned an error: {0}", *GameStartResult.ErrorCode());
					OnGameStartFailed.Broadcast(GameStartResult.Error());
				} else
				{
					PRAGMA_LOG(Display, "Game start data received in RequestStartGame call for game instance id: %s.", *GameInstanceId);
					OnGameStart.Broadcast(MoveTemp(GameStartResult.Payload().GameStart));					
				}
			}
		));
}

void UPragmaMatchApi::ReportCapacity() const
{
	PRAGMA_LOG(VeryVerbose, "Polling ReportCapacity: ServerId %s, ServerPoolId %s", *ReportCapacityRequest.ServerId, *ReportCapacityRequest.ServerPoolId);
	FleetRaw->ReportCapacityV1(ReportCapacityRequest, ReportCapacityResponseDelegate);
}

void UPragmaMatchApi::ReportMatchCapacity() const
{
	PRAGMA_LOG(VeryVerbose, "Polling ReportCapacity: ServerId %s, GameVersion %s", *ReportCapacityMatchRequest.ServerId,
		*ReportCapacityMatchRequest.GameVersion);
	MatchCapacityRaw->ReportCapacityV2(ReportCapacityMatchRequest, ReportCapacityMatchResponseDelegate);
}

void UPragmaMatchApi::HandleReportCapacityResponse(
	TPragmaResult<FPragma_Fleet_ReportCapacityV1Response> ReportCapacityResult,
	const FPragmaMessageMetadata&)
{
	if (ReportCapacityResult.IsFailure())
	{
		PRAGMA_LOG(Warning, "ReportCapacity returned an error: {0}", *ReportCapacityResult.ErrorCode());
		StopReportCapacityPollingWithError(ReportCapacityResult.Error());
		return;
	}

	const int32 NumGameInstanceIds = ReportCapacityResult.Payload().GameInstanceIds.Num();
	const float NextHeartbeatSeconds = ReportCapacityResult.Payload().NextHeartbeatPeriodMillis / 1000.f;

	if (NumGameInstanceIds == 0)
	{
		const bool HasTimeout = ReportCapacityTimeout > 0.0f;
		const bool HasReachedTimeout = SecondsPolling > ReportCapacityTimeout;
		if (!HasEverBeenAllocatedAGame && HasTimeout && HasReachedTimeout)
		{
			StopReportCapacityPollingWithError(FPragmaError{EPragma_SdkError::Fleet_ReportCapacityTimeout});
			return;
		}

		ContinueReportCapacityPolling(NextHeartbeatSeconds);
		return;
	}

	for (const FString &GameInstanceId : ReportCapacityResult.Payload().GameInstanceIds)
	{
		if (ReportCapacityRequest.UsedCapacity == ReportCapacityRequest.TotalCapacity)
		{
			PRAGMA_LOG(Warning,
				"Tried to allocate a new game instance (%s) when total capacity is already met.",
				*GameInstanceId);
			ContinueReportCapacityPolling(NextHeartbeatSeconds);
			return;
		}
			
		PRAGMA_LOG(Display, "Game instance allocated: %s.", *GameInstanceId);
		const auto Request = FPragma_GameInstance_GetGameStartDataV1Request{GameInstanceId};
		GameInstanceServiceRaw->GetGameStartDataV1(Request,
			UPragmaGameInstancePartnerServiceRaw::FGetGameStartDataV1Delegate::CreateWeakLambda(
				this, [this, GameInstanceId, NextHeartbeatSeconds](
				TPragmaResult<FPragma_GameInstance_GetGameStartDataV1Response> GameStartResult,
				const FPragmaMessageMetadata&)
				{
					if (GameStartResult.IsFailure())
					{
						PRAGMA_LOG(Warning, "GetGameStartData from ReportCapacity returned an error: {0}", *GameStartResult.ErrorCode());
						StopReportCapacityPollingWithError(GameStartResult.Error());
						return;
					}

					PRAGMA_LOG(Display, "Game start data received in ReportCapacity call for game instance id: %s.", *GameInstanceId);
					HasEverBeenAllocatedAGame = true;
					ReportCapacityRequest.UsedCapacity += 1;
					PRAGMA_LOG(VeryVerbose, "Adding one unit of used capacity. New capacity: %d/%d", ReportCapacityRequest.UsedCapacity, ReportCapacityRequest.TotalCapacity);
					OnGameStart.Broadcast(MoveTemp(GameStartResult.Payload().GameStart));
					ContinueReportCapacityPolling(NextHeartbeatSeconds);
				}
			));
	}
}

void UPragmaMatchApi::HandleReportCapacityMatchResponse(
	TPragmaResult<FPragma_MatchCapacity_ReportCapacityV2Response> ReportCapacityMatchResult,
	const FPragmaMessageMetadata&)
{
	if (ReportCapacityMatchResult.IsFailure())
	{
		PRAGMA_LOG(Warning, "ReportCapacity returned an error: {0}", *ReportCapacityMatchResult.ErrorCode());
		StopReportCapacityPollingWithError(ReportCapacityMatchResult.Error());
		return;
	}

	const int32 NumGameInstanceIds = ReportCapacityMatchResult.Payload().GameInstanceIds.Num();
	const float NextHeartbeatSeconds = ReportCapacityMatchResult.Payload().NextHeartbeatPeriodMillis / 1000.f;

	if (NumGameInstanceIds == 0)
	{
		const bool HasTimeout = ReportCapacityTimeout > 0.0f;
		const bool HasReachedTimeout = SecondsPolling > ReportCapacityTimeout;
		if (!HasEverBeenAllocatedAGame && HasTimeout && HasReachedTimeout)
		{
			StopReportCapacityPollingWithError(FPragmaError{EPragma_SdkError::MatchCapacity_ReportCapacityTimeout});
			return;
		}

		ContinueReportCapacityPolling(NextHeartbeatSeconds);
		return;
	}

	for (const FString &GameInstanceId : ReportCapacityMatchResult.Payload().GameInstanceIds)
	{
		if (ReportCapacityMatchRequest.UsedCapacity == ReportCapacityMatchRequest.TotalCapacity)
		{
			PRAGMA_LOG(Warning,
				"Tried to allocate a new game instance (%s) when total capacity is already met.",
				*GameInstanceId);
			ContinueReportCapacityPolling(NextHeartbeatSeconds);
			return;
		}

		PRAGMA_LOG(Display, "Game instance allocated: %s.", *GameInstanceId);
		const auto Request = FPragma_GameInstance_GetGameStartDataV1Request{GameInstanceId};
		GameInstanceServiceRaw->GetGameStartDataV1(Request,
			UPragmaGameInstancePartnerServiceRaw::FGetGameStartDataV1Delegate::CreateWeakLambda(
				this, [this, GameInstanceId, NextHeartbeatSeconds](
				TPragmaResult<FPragma_GameInstance_GetGameStartDataV1Response> GameStartResult,
				const FPragmaMessageMetadata&)
				{
					if (GameStartResult.IsFailure())
					{
						PRAGMA_LOG(Warning, "GetGameStartData returned an error: {0}", *GameStartResult.ErrorCode());
						StopReportCapacityPollingWithError(GameStartResult.Error());
						return;
					}

					PRAGMA_LOG(Display, "Game start data received for: %s.", *GameInstanceId);
					HasEverBeenAllocatedAGame = true;
					ReportCapacityMatchRequest.UsedCapacity += 1;
					PRAGMA_LOG(VeryVerbose, "Adding one unit of used capacity. New capacity: %d/%d", ReportCapacityMatchRequest.UsedCapacity, ReportCapacityMatchRequest.TotalCapacity);
					OnGameStart.Broadcast(MoveTemp(GameStartResult.Payload().GameStart));
					ContinueReportCapacityPolling(NextHeartbeatSeconds);
				}
			));
	}
}

void UPragmaMatchApi::ContinueReportCapacityPolling(const float NextHeartbeatSeconds)
{
	TimerManager->SetTimer(ReportCapacityTimerHandle, FPragmaTimerDelegate::CreateWeakLambda(this,
		[this, NextHeartbeatSeconds]
		{
			SecondsPolling += NextHeartbeatSeconds;

			if (UseMatchCapacityService)
			{
				ReportMatchCapacity();
			} else
			{
				ReportCapacity();
			}
		}), NextHeartbeatSeconds, false);
}

void UPragmaMatchApi::StopReportCapacityPollingWithError(const FPragmaError& Error) const
{
	OnGameStartFailed.Broadcast(Error);
}
