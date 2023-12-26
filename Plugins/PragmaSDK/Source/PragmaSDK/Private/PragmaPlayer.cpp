#include "PragmaPlayer.h"

#include "PragmaHttp.h"
#include "PragmaWebSocket.h"
#include "HttpDto/PragmaAuthenticateDto.h"
#include "Dto/PragmaGeneratedServiceRegistration.h"
#include "Dto/PragmaPartyServiceRaw.h"
#include "Dto/PragmaPlayerDataServiceRaw.h"
#include "Pragma/Api/Player/PragmaPresenceApi.h"
#include "Services/PragmaPlayerDataService.h"
#include "Services/PragmaSessionServiceRaw.h"
#include "Services/PragmaSessionService.h"

class UPragmaPlayerDataService;

// ************************************************************
// *** THIS FILE WAS STUBBED FOR THIS TUNDRA DESIGN PROJECT ***
// ************************************************************

namespace Pragma
{
FPlayer::FPlayer(FSdkConfigStrongPtr Config, TSharedPtr<IPragmaTimerManager> TimerManager)
	: FSession(MoveTemp(Config), MoveTemp(TimerManager))
{
}

void FPlayer::InitApi(UClass* StaticClass)
{
	PlayerApis.FindRef(StaticClass)->Init(AsShared());
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
	StubbedLogin(OnComplete);
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
	StubbedLogin(OnComplete);
}

void FPlayer::LogOut(const FLoggedOutDelegate& OnComplete)
{
	if (!IsLoggingIn() && !IsLoggedIn())
	{
		auto _ = OnComplete.ExecuteIfBound();
		return;
	}
	PRAGMA_LOG(Display, "FPlayer -- Logging out...");
	StubbedLogout(OnComplete);
}

void FPlayer::StubbedLogin(const FLoggedInDelegate& OnComplete)
{
	GameJwt = {
		.Payload = {
			.DisplayName = "Bobby Bobster",
			.Discriminator = "1234",
			.PragmaPlayerId = "FakePragmaPlayerId",
			.PragmaSocialId = "FakePragmaSocialId"
		}
	};
	IdValue = MoveTemp(GameJwt.Payload.PragmaPlayerId);
	SocialIdValue = MoveTemp(GameJwt.Payload.PragmaSocialId);
	FullDisplayNameValue = FString::Printf(TEXT("%s#%s"), *DisplayName(), *Discriminator());
	GameToken = "FakeGameToken";
	SocialToken = "FakeSocialToken";

	auto _ = OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
}

void FPlayer::StubbedLogout(const FLoggedOutDelegate& OnComplete)
{
	auto _ = OnComplete.ExecuteIfBound();
}
}
