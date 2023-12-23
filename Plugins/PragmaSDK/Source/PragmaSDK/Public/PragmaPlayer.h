#pragma once

#include "Dto/PragmaAccountRpcDto.h"
#include "PragmaSessionV2.h"
#include "PragmaPtr.h"
#include "Services/PragmaAccountService.h"
#include "Services/PragmaInventoryService.h"
#include "Pragma/Api/Player/PragmaGameLoopApi.h"
#include "PragmaApiProvider.h"
#include "PragmaConnectionError.h"
#include "PragmaJwt.h"
#include "PragmaPlayerApi.h"
#include "HttpDto/PragmaAuthenticateDto.h"
#include "Pragma/Api/Player/PragmaFriendApi.h"
#include "Pragma/Api/Player/PragmaPresenceApi.h"
#include "Services/PragmaPlayerDataService.h"

PRAGMA_PTR_API(FPlayer);

namespace Pragma
{
/**
 * Represents a single player within Pragma and their connection to the Pragma Backend. Any classes or operations accessed through this object
 * do so within the context of this player.
 */
class PRAGMASDK_API FPlayer : public FSession, public TApiProvider<UPragmaPlayerApi>, public TPragmaSharedFromThis<FPlayer>
{
public:
	// Pragma API accessors for convenience.
	// Access custom APIs with Api<MyApi>().
	UPragmaGameLoopApi& GameLoopApi() const { return Api<UPragmaGameLoopApi>(); }
	UPragmaAccountService& Account() const { return Api<UPragmaAccountService>(); }
	UPragmaGameDataService& GameData() const { return Api<UPragmaGameDataService>(); }
	UPragmaInventoryService& Inventory() const { return Api<UPragmaInventoryService>(); }
	UPragmaPlayerDataService& PlayerDataService() const { return Api<UPragmaPlayerDataService>(); }
	UPragmaFriendApi& FriendApi() const { return Api<UPragmaFriendApi>(); }
	UPragmaPresenceApi& PresenceApi() const { return Api<UPragmaPresenceApi>(); }

	// Returns true if the player is in the process of logging in.
	bool IsLoggingIn() const { return IsConnecting(); }

	// Returns true if the player is fully logged-in.
	bool IsLoggedIn() const { return IsConnected(); }

	// The player's globally unique Player ID which identifies them within the Pragma Game Backend. Empty if not yet logged in.
	const FString& Id() const;

	// The player's globally unique Social ID which identifies them within the Pragma Game Backend. Empty if not yet logged in.
	const FString& SocialId() const;

	// Returns the name part of the player's display name.
	// e.g. "MyName" from MyName#1234
	const FString& DisplayName() const;

	// Returns the discriminator part of the player's display name.
	// e.g. "1234" from MyName#1234
	const FString& Discriminator() const;

	// Returns the player's full display name.
	// e.g. MyName#1234
	const FString& FullDisplayName() const;

	// Returns the authentication token most recently used to log into the Pragma Social backend.
	// This will remain valid as long as a player is logged in, and may change as the SDK refreshes the token.
	// If the player logs out, it cannot be assumed that this token is still valid.
	const FString& PragmaSocialToken() const;

	// Returns the authentication token most recently used to log into the Pragma Game backend.
	// This will remain valid as long as a player is logged in, and may change as the SDK refreshes the token.
	// If the player logs out, it cannot be assumed that this token is still valid.
	const FString& PragmaGameToken() const;

	DECLARE_DELEGATE_OneParam(FLoggedInDelegate, const TPragmaResult<>&);

	// Log the player in using the selected IdProvider's authentication token.
	void LogIn(const EPragma_Account_IdProvider ProviderId, const FString& ProviderToken, const FLoggedInDelegate& OnComplete);

	// Log the player in using the selected IdProvider's authentication token.
	void LogIn(const EPragma_Account_ExtIdProvider ProviderId, const FString& ProviderToken, const FLoggedInDelegate& OnComplete);

	// Log the player in using the selected IdProvider's authentication token.
	void LogInProvider(const FString& ProviderId, const FString& ProviderToken, const FLoggedInDelegate& OnComplete);

	// Log the player in using existing Pragma authentication tokens.
	// This is for use when you have already have tokens through other means, e.g. a launcher that authenticates before the game is launched.
	void LogIn(FString InPragmaSocialToken, FString InPragmaGameToken, const FLoggedInDelegate& OnComplete);

	DECLARE_DELEGATE(FLoggedOutDelegate);

	// Log the player out of Pragma.
	void LogOut(const FLoggedOutDelegate& OnComplete);

	struct FQueueUpdateData
	{
		FString Eta;
		int32 PositionInQueue{0};
	};

	// Fired upon login queue updates.
	DECLARE_EVENT_OneParam(UPragmaAccountService, FLoginQueueUpdateEvent, FQueueUpdateData /* Eta, PositionInQueue */);

	FLoginQueueUpdateEvent OnLoginQueueUpdate;

	// Fired when a token successfully refreshes.
	DECLARE_EVENT_TwoParams(UPragmaAccountService, FTokensEvent, FString /*PragmaSocialToken*/, FString /*PragmaGameToken*/);

	FTokensEvent OnTokenRefreshCompleted;

	virtual bool IsPartnerSession() const override { return false; }

PACKAGE_SCOPE:
	// Should only be created through the Pragma Runtime.
	static FPlayerStrongRef Create(FSdkConfigStrongPtr Config, TSharedPtr<IPragmaTimerManager> TimerManager)
	{
		auto Player = FPlayerStrongRef{new FPlayer(MoveTemp(Config), MoveTemp(TimerManager))};
		// This cannot be called in the constructor because we need access to AsShared.
		Player->InitializeServices();
		return Player;
	}

protected:
	FPlayer(FSdkConfigStrongPtr Config, TSharedPtr<IPragmaTimerManager> TimerManager);

	virtual TSharedPtr<FSession, SpMode> AsSharedSession() override { return AsShared(); }

	virtual FSessionMap& Apis() override { return PlayerApis; }
	virtual const FSessionMap& Apis() const override { return PlayerApis; }

	virtual void InitApi(UClass* StaticClass) override;

	DECLARE_DELEGATE(FRefreshTokenFunc);

	void InitializeServices();
	void RegisterDefaultApis();
	void InitDefaultApis();

	virtual void LegacyLogIn(const EPragma_Account_IdProvider ProviderId, const FString& ProviderToken, const FLoggedInDelegate& OnComplete);
	virtual void LegacyLogIn(const EPragma_Account_ExtIdProvider ProviderId, const FString& ProviderToken, const FLoggedInDelegate& OnComplete);
	virtual void LegacyLogInProvider(const FString& ProviderId, const FString& ProviderToken, const FLoggedInDelegate& OnComplete);
	virtual void LegacyLogIn(FString InPragmaSocialToken, FString InPragmaGameToken, const FLoggedInDelegate& OnComplete);

	virtual void LegacyAuthenticate(FPragma_Account_AuthenticateOrCreateV2Request Request, const FLoggedInDelegate& OnComplete);
	void CheckTicket(
		const bool IsAllowedIn,
		const FString& TicketToken,
		const int NextPollDuration,
		const FString& Eta,
		const int32 PositionInQueue,
		const FString& ProviderId,
		const FString& ProviderToken,
		const FLoggedInDelegate& OnComplete
	);
	void CheckTicketRequest(
		const FString& TicketToken,
		const FString& ProviderId,
		const FString& ProviderToken,
		const FLoggedInDelegate& OnComplete
	);

	virtual void Connect(const bool bRefreshImmediately, const FLoggedInDelegate& OnComplete);
	virtual void SetPragmaSocialToken(FString InPragmaSocialToken);
	virtual void SetPragmaGameToken(FString InPragmaGameToken);
	float GetInitialRefreshDelayInSeconds(const bool bRefreshImmediately) const;
	virtual void CreateRefreshLoop(const bool bRefreshImmediately);
	TOptional<FPragmaConnectionError> ValidateTokens();
	void RefreshSession();

	// Retry loop counter for refreshing tokens
	int RefreshFailCount{0};

	FPragmaTimerHandle RefreshTokenHandle;
	FPragmaTimerHandle CheckTicketHandle;

	FSessionMap PlayerApis;

	FString IdValue;
	FString SocialIdValue;
	FString FullDisplayNameValue;
	FString SocialToken;
	FString GameToken;
	FPragmaJwt SocialJwt;
	FPragmaJwt GameJwt;
};
}
