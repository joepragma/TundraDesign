#include "PragmaPlayer.h"

#include "PragmaHttp.h"
#include "PragmaWebSocket.h"
#include "PragmaConnection.h"
#include "HttpDto/PragmaLoginQueueDto.h"
#include "Dto/PragmaGeneratedServiceRegistration.h"
#include "Dto/PragmaPartyServiceRaw.h"
#include "Dto/PragmaPlayerDataServiceRaw.h"
#include "Pragma/Api/Player/PragmaPresenceApi.h"
#include "Services/PragmaPlayerDataService.h"
#include "Services/PragmaSessionServiceRaw.h"
#include "Services/PragmaSessionService.h"

class UPragmaPlayerDataService;

namespace Pragma
{
const FString& FPlayer::Id() const
{
	return IdValue;
}

const FString& FPlayer::SocialId() const
{
	return SocialIdValue;
}

const FString& FPlayer::DisplayName() const
{
	return GameJwt.Payload.DisplayName;
}

const FString& FPlayer::Discriminator() const
{
	return GameJwt.Payload.Discriminator;
}

const FString& FPlayer::FullDisplayName() const
{
	return FullDisplayNameValue;
}

const FString& FPlayer::PragmaSocialToken() const
{
	return SocialToken;
}

const FString& FPlayer::PragmaGameToken() const
{
	return GameToken;
}

void FPlayer::LogIn(const EPragma_Account_IdProvider ProviderId, const FString& ProviderToken,
	const FLoggedInDelegate& OnComplete)
{
	LogInProvider(EnumToString<EPragma_Account_IdProvider>(ProviderId), ProviderToken, OnComplete);
}

void FPlayer::LogIn(const EPragma_Account_ExtIdProvider ProviderId, const FString& ProviderToken,
	const FLoggedInDelegate& OnComplete)
{
	LogInProvider(EnumToString<EPragma_Account_ExtIdProvider>(ProviderId), ProviderToken, OnComplete);
}

void FPlayer::LogInProvider(const FString& ProviderId, const FString& ProviderToken, const FLoggedInDelegate& OnComplete)
{
	if (IsLoggingIn() || IsLoggedIn())
	{
		auto _ = OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
		return;
	}
	PRAGMA_LOG(Display, "FPlayer -- Logging in...");
	InitLegacySession();
	LegacyLogInProvider(ProviderId, ProviderToken, OnComplete);
}

void FPlayer::LogIn(FString InPragmaSocialToken, FString InPragmaGameToken, const FLoggedInDelegate& OnComplete)
{
	if (IsLoggingIn() || IsLoggedIn())
	{
		auto _ = OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
		return;
	}

	PRAGMA_LOG(Display, "FPlayer -- Logging in...");
	InitLegacySession();
	LegacyLogIn(InPragmaSocialToken, InPragmaGameToken, OnComplete);
}

void FPlayer::LogOut(const FLoggedOutDelegate& OnComplete)
{
	if (!IsLoggingIn() && !IsLoggedIn())
	{
		auto _ = OnComplete.ExecuteIfBound();
		return;
	}
	PRAGMA_LOG(Display, "FPlayer -- Logging out...");
	auto Handle = MakeShared<FDelegateHandle>();
	*Handle = Connection().OnDisconnectedV2.AddLambda(WeakLambda<TOptional<FPragmaConnectionError>>(this, {[this, Handle, OnComplete](auto)
	{
		auto _ = OnComplete.ExecuteIfBound();
		Connection().OnDisconnectedV2.Remove(*Handle);
	}}));
	Connection().Disconnect();
}

void FPlayer::InitApi(UClass* StaticClass)
{
	PlayerApis.FindRef(StaticClass)->Init(AsShared());
}

FPlayer::FPlayer(FSdkConfigStrongPtr Config, TSharedPtr<IPragmaTimerManager> TimerManager)
	: FSession(MoveTemp(Config), MoveTemp(TimerManager))
{
}

void FPlayer::InitializeServices()
{
	RegisterGeneratedPlayerApis(*this);
	RegisterDefaultApis();
	InitDefaultApis();
}

void FPlayer::RegisterDefaultApis()
{
	// Special case, not generated because it contains both social and game RPCs.
	RegisterApi<UPragmaSessionServiceRaw>();

	RegisterApi<UPragmaSessionService>();
	RegisterApi<UPragmaAccountService>();
	RegisterApi<UPragmaInventoryService>();
	RegisterApi<UPragmaGameDataService>();
	RegisterApi<UPragmaGameLoopApi>();
	RegisterApi<UPragmaPlayerDataService>();
	RegisterApi<UPragmaFriendApi>();
	RegisterApi<UPragmaPresenceApi>();
}

void FPlayer::InitDefaultApis()
{
	UPragmaAccountService& AccountService = Api<UPragmaAccountService>();
	AccountService.SetDependencies(&Api<UPragmaAccountServiceRaw>());

	UPragmaSessionServiceRaw& SessionServiceRaw = Api<UPragmaSessionServiceRaw>();
	SessionServiceRaw.SetDependencies(AsShared());

	UPragmaSessionService& SessionService = Api<UPragmaSessionService>();
	SessionService.SetDependencies(AsShared(), &SessionServiceRaw, TimerManager(), Config().GetHeartbeatPeriodSeconds());

	UPragmaGameDataService& GameDataService = Api<UPragmaGameDataService>();
	GameDataService.SetDependencies(&Api<UPragmaGameDataServiceRaw>());

	UPragmaInventoryService& InventoryService = Api<UPragmaInventoryService>();
	InventoryService.SetDependencies(&Api<UPragmaInventoryServiceRaw>(),
		&Api<UPragmaGameDataService>(),
		nullptr);

	UPragmaGameLoopApi& GameLoopApi = Api<UPragmaGameLoopApi>();
	GameLoopApi.SetDependencies(&Api<UPragmaPartyServiceRaw>(),
		&Api<UPragmaMatchmakingServiceRaw>(),
		&Api<UPragmaGameInstanceServiceRaw>(),
		&Api<UPragmaSessionService>(),
		&Config());

	UPragmaPlayerDataService& PlayerDataService = Api<UPragmaPlayerDataService>();
	PlayerDataService.SetDependencies(&Api<UPragmaPlayerDataServiceRaw>());

	UPragmaFriendApi& FriendApi = Api<UPragmaFriendApi>();
	FriendApi.SetDependencies(&Api<UPragmaFriendServiceRaw>());

	UPragmaPresenceApi& PresenceApi = Api<UPragmaPresenceApi>();
	PresenceApi.SetDependencies(&Api<UPragmaFriendServiceRaw>());
}

void FPlayer::LegacyLogIn(const EPragma_Account_IdProvider ProviderId, const FString& ProviderToken,
	const FLoggedInDelegate& OnComplete)
{
	LegacyLogInProvider(EnumToString<EPragma_Account_IdProvider>(ProviderId), ProviderToken, OnComplete);
}

void FPlayer::LegacyLogIn(const EPragma_Account_ExtIdProvider ProviderId, const FString& ProviderToken,
	const FLoggedInDelegate& OnComplete)
{
	LegacyLogInProvider(EnumToString<EPragma_Account_ExtIdProvider>(ProviderId), ProviderToken, OnComplete);
}

void FPlayer::LegacyLogInProvider(const FString& ProviderId, const FString& ProviderToken, const FLoggedInDelegate& OnComplete)
{
	//
	// Step 1: Update backend config, if necessary.
	//
	PRAGMA_LOG(Log, "FPlayer::LogIn Updating backend config...");
	Connection().UpdateBackendConfig([this, ProviderId, ProviderToken, OnComplete](const TPragmaResult<> Result)
	{
		if (Result.IsFailure())
		{
			PRAGMA_LOG(Error, "FPlayer::LogIn Failed to retrieve backend config, the server is not"
				" available or client configuration is invalid.");
			auto _ = OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(Result.Error()));
			return;
		}

		PRAGMA_LOG( Log, "FPlayer::LogIn Updated backend config successfully, getting in login queue...");
		Connection().Http(EPragmaBackendType::Game).HttpGet<FPragma_GetInQueueV1_Response>(
			"/v1/loginqueue/getinqueuev1",
			[this,ProviderId,ProviderToken,OnComplete](TPragmaResult<FPragma_GetInQueueV1_Response> Response)
			{
				if(Response.IsFailure())
				{
					PRAGMA_LOG(Error, "FPlayer::LogIn Failed getinqueue");
					auto _ = OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(Response.Error()));
					return;
				}

				const bool IsAllowedIn = Response.Payload().IsAllowedIn;
				const FString TicketToken = Response.Payload().Token;
				const int32 NextPollDuration = Response.Payload().PollNextDuration;
				const FString Eta = *Response.Payload().Eta;
				const int32 PositionInQueue = Response.Payload().PositionInQueue;
				CheckTicket(IsAllowedIn, TicketToken, NextPollDuration, Eta, PositionInQueue, ProviderId, ProviderToken, OnComplete);
			});
	});
}

void FPlayer::LegacyLogIn(FString InPragmaSocialToken, FString InPragmaGameToken, const FLoggedInDelegate& OnComplete)
{
	SetPragmaSocialToken(MoveTemp(InPragmaSocialToken));
	SetPragmaGameToken(MoveTemp(InPragmaGameToken));

	PRAGMA_LOG(Log, "FPlayer::LegacyLogIntoExistingSession -- Updating backend config...");
	Connection().UpdateBackendConfig([this, OnComplete](const TPragmaResult<> Result)
	{
		if (Result.IsFailure())
		{
			PRAGMA_LOG(Error,
				"FPlayer::LegacyLogIntoExistingSession -- Failed to retrieve backend config, the server is not available or client configuration is invalid.");
			auto _ = OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(Result.Error()));
			return;
		}

		PRAGMA_LOG(Log,
			"FPlayer::LegacyLogIntoExistingSession -- Updated backend config successfully, establishing websocket connections...");
		Connect(true, OnComplete);
	});
}

void FPlayer::CheckTicketRequest(const FString& TicketToken,
	const FString& ProviderId, const FString& ProviderToken, const FLoggedInDelegate& OnComplete)
{
	const FPragma_CheckTicketV1_Request CheckTicketRequest{
		TicketToken
	};
	Connection().Http(EPragmaBackendType::Game).HttpPost<
		FPragma_CheckTicketV1_Request, FPragma_CheckTicketV1_Response>
	(TEXT("/v1/loginqueue/checkticketv1"), CheckTicketRequest,
		[this, ProviderId, ProviderToken, OnComplete](
		TPragmaResult<FPragma_CheckTicketV1_Response> CheckTicketResult)
		{
			if (CheckTicketResult.IsFailure())
			{
				PRAGMA_LOG(Error, "FPlayer::LogIn Failed checkticket");
				auto _ = OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(CheckTicketResult.Error()));
				return;
			}

			PRAGMA_LOG(Log, "FPlayer::LogIn Got check ticket response, checking if allowed in...");
			const bool IsAllowedIn = CheckTicketResult.Payload().IsAllowedIn;
			const FString TicketToken = CheckTicketResult.Payload().Token;
			const int32 NextPollDuration = CheckTicketResult.Payload().PollNextDuration;
			const FString Eta = *CheckTicketResult.Payload().Eta;
			const int32 PositionInQueue = CheckTicketResult.Payload().PositionInQueue;
			CheckTicket(IsAllowedIn, TicketToken,  NextPollDuration, Eta, PositionInQueue, ProviderId, ProviderToken, OnComplete);
		});
}

void FPlayer::CheckTicket(
	const bool IsAllowedIn,
	const FString& TicketToken,
	const int NextPollDuration,
	const FString& Eta,
	const int32 PositionInQueue,
	const FString& ProviderId,
	const FString& ProviderToken,
	const FLoggedInDelegate& OnComplete
)
{
	OnLoginQueueUpdate.Broadcast(FQueueUpdateData{Eta, PositionInQueue});
	if (IsAllowedIn)
	{
		const FPragma_Account_AuthenticateOrCreateV2Request Request{
			ProviderId,
			// For unsafe, the id and display name are the same. This makes it easier on impl code to just pass e.g. "test01".
			ProviderId == EnumToString<EPragma_Account_IdProvider>(EPragma_Account_IdProvider::UNSAFE)
				? FString::Printf(TEXT("{\"accountId\":\"%s\",\"displayName\":\"%s\"}"), *ProviderToken, *ProviderToken)
				: ProviderToken,
			Connection().GetGameShardId(),
			TicketToken
		};
		PRAGMA_LOG( Log, "FPlayer::LogIn Login queue passed, attempting authentication...");
		LegacyAuthenticate(Request, OnComplete);
		return;
	}

	const float InRate = static_cast<float>(NextPollDuration/1000.0);
	TimerManager()->SetTimer(
		CheckTicketHandle,
		FRefreshTokenFunc::CreateLambda(WeakLambda(this,
		{
			[this, ProviderId, ProviderToken, OnComplete, TicketToken]
			{
				CheckTicketRequest(TicketToken, ProviderId, ProviderToken, OnComplete);
			}
		})),
		InRate,
		false,
		0
	);

}

void FPlayer::LegacyAuthenticate(FPragma_Account_AuthenticateOrCreateV2Request Request,
	const FLoggedInDelegate& OnComplete)
{
	Connection().Http(EPragmaBackendType::Social).HttpPost<FPragma_Account_AuthenticateOrCreateV2Request, FPragma_Account_AuthenticateOrCreateV2Response>
	("/v1/account/authenticateorcreatev2", Request,[this, OnComplete](TPragmaResult<FPragma_Account_AuthenticateOrCreateV2Response> Result)
	{
			if (Result.IsFailure())
			{
				PRAGMA_LOG(Error, "FPlayer::LogIn Failed to authenticate, error code: %s",
					*Result.ErrorCode());
				auto _ = OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(Result.Error()));
				return;
			}

			//
			// Authenticated successfully. Now connect the websockets using our authenticated session token.
			//
			PRAGMA_LOG(Log,
				"FPlayer::LogIn Authenticated successfully, establishing websocket connections...");
			SetPragmaGameToken(MoveTemp(Result.Payload().PragmaTokens.PragmaGameToken));
			SetPragmaSocialToken(Result.Payload().PragmaTokens.PragmaSocialToken);
			Connect(false, OnComplete);
		});
}



float FPlayer::GetInitialRefreshDelayInSeconds(const bool bRefreshImmediately) const
{
	if (bRefreshImmediately)
	{
		return 1.0f;
	}
	return static_cast<float>(FCString::Atoi64(*GameJwt.Payload.RefreshInMillis) / 1000.0);
}

void FPlayer::CreateRefreshLoop(const bool bRefreshImmediately)
{
	RefreshFailCount = 0;
	TimerManager()->SetTimer(
		RefreshTokenHandle,
		FRefreshTokenFunc::CreateLambda(WeakLambda(this, {[this]
		{
			if (RefreshFailCount >= 5)
			{
				PRAGMA_LOG(Error,
					"FPlayer::CreateRefreshLoop -- Refresh has failed 5 times, stopping refresh loop");
				TimerManager()->ClearTimer(RefreshTokenHandle);
				return;
			}
			Api<UPragmaAccountServiceRaw>().RefreshTokensV1(
				FPragma_Account_RefreshTokensV1Request{
					FPragma_Account_PragmaTokens{PragmaGameToken(), PragmaSocialToken()}
				},
				UPragmaAccountServiceRaw::FRefreshTokensV1Delegate::CreateLambda(WeakLambda(this,
					TFunction<void(const TPragmaResult<FPragma_Account_RefreshTokensV1Response>, const FPragmaMessageMetadata&)>{[this](auto Result, auto)
					{
						if (Result.IsSuccessful())
						{
							SetPragmaGameToken(Result.Payload().PragmaTokens.PragmaGameToken);
							SetPragmaSocialToken(Result.Payload().PragmaTokens.PragmaSocialToken);

							TOptional<FPragmaConnectionError> HasError = ValidateTokens();
							if (HasError.IsSet())
							{
								Connection().Disconnect();
								return;
							}

							Connection().SetAuthTokens(PragmaSocialToken(), PragmaGameToken());
							RefreshSession();
							CreateRefreshLoop(false);
							OnTokenRefreshCompleted.Broadcast(PragmaSocialToken(), PragmaGameToken());
						}
						else
						{
							RefreshFailCount += 1;
						}
					}})
				)
			);
		}})),
		60,
		true,
		GetInitialRefreshDelayInSeconds(bRefreshImmediately)
	);
}

TOptional<FPragmaConnectionError> FPlayer::ValidateTokens()
{
	if (SocialToken.IsEmpty() || GameToken.IsEmpty())
	{
		if (!SocialToken.IsEmpty() && GameToken.IsEmpty())
		{
			PRAGMA_LOG(Error, "FPlayer::ValidateTokens -- Permission denied to login");
			return FPragmaConnectionError(EPragmaProtocolError::NoPermission, EPragmaProtocolError::NoPermission);
		}
		PRAGMA_LOG(Error, "FPlayer::ValidateTokens -- Did not pass in valid tokens"
			"\n    SOCIAL: %s"
			"\n    PLAYER: %s", *SocialToken, *GameToken);
		return FPragmaConnectionError(EPragmaProtocolError::Unknown, EPragmaProtocolError::Unknown);
	}
	return {};
}

void FPlayer::RefreshSession()
{
	if (Config().GetProtocolType(false) == EPragmaProtocolType::WebSocket)
	{
		Api<UPragmaSessionServiceRaw>().SocialSessionRefreshV1(
			FPragma_Session_SessionRefreshV1Request{SocialToken},
			UPragmaSessionServiceRaw::FSessionRefreshV1Delegate::CreateLambda(WeakLambda(this,
				TFunction<void(TPragmaResult<FPragma_Session_SessionRefreshV1Response>, const FPragmaMessageMetadata&)>{[](auto, auto) {}}
			))
		);
		Api<UPragmaSessionServiceRaw>().GameSessionRefreshV1(
			FPragma_Session_SessionRefreshV1Request{GameToken},
			UPragmaSessionServiceRaw::FSessionRefreshV1Delegate::CreateLambda(WeakLambda(this,
				TFunction<void(const TPragmaResult<FPragma_Session_SessionRefreshV1Response>, const FPragmaMessageMetadata&)>{[](auto, auto){}}
			))
		);
	}
}

void FPlayer::Connect(const bool bRefreshImmediately, const FLoggedInDelegate& OnComplete)
{
	TOptional<FPragmaConnectionError> HasError = ValidateTokens();
	if (HasError.IsSet())
	{
		auto _ = OnComplete.ExecuteIfBound(
			TPragmaResult<>::Failure(EProtocolErrorToFString(HasError.GetValue().GameBackendError)));
		return;
	}

	if (Config().ProtocolType == EPragmaProtocolType::Http)
	{
		OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
		return;
	}

	Connection().Connect(SocialToken, GameToken, WeakLambda(this, TFunction<void(const TPragmaResult<>)>{
		[this, OnComplete, bRefreshImmediately](const TPragmaResult<> Result)
		{
			if (Result.IsSuccessful())
			{
				Connection().OnPreDisconnect.AddLambda(WeakLambda(this,
					{
						[this]
						{
							TimerManager()->ClearTimer(RefreshTokenHandle);
						}
					}
				));
				CreateRefreshLoop(bRefreshImmediately);
				PRAGMA_LOG(Log, "FPlayer::Connect -- Connected successfully.");
				auto _ = OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
			}
			else
			{
				PRAGMA_LOG(Error, "FPlayer::Connect -- Failed to connect websockets.");
				auto _ = OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(Result.Error()));
			}
		}}));
}

void FPlayer::SetPragmaGameToken(FString InPragmaGameToken)
{
	TPragmaResult<FPragmaJwt> JwtResult = FPragmaJwt::FromEncoded(InPragmaGameToken);
	if (JwtResult.IsSuccessful())
	{
		GameJwt = JwtResult.Payload();
		FullDisplayNameValue = FString::Printf(TEXT("%s#%s"), *DisplayName(), *Discriminator());
		if (!GameJwt.Payload.PragmaPlayerId.IsEmpty())
		{
			IdValue = MoveTemp(GameJwt.Payload.PragmaPlayerId);
			SocialIdValue = MoveTemp(GameJwt.Payload.PragmaSocialId);
		}
	}
	else
	{
		PRAGMA_LOG(Error,
			"FPlayer::LogIn Unable to parse PlayerGameToken as a JWT. Some game info will be unavailable."
			" May also fail to connect web sockets. Error code: %s, JWT: %s", *JwtResult.ErrorCode(),
			*InPragmaGameToken);
	}
	GameToken = MoveTemp(InPragmaGameToken);
}

void FPlayer::SetPragmaSocialToken(FString InPragmaSocialToken)
{
	SocialToken = MoveTemp(InPragmaSocialToken);
}

}
