#pragma once

#include "CoreMinimal.h"
#include "PragmaSessionV2.h"
#include "PragmaPtr.h"
#include "PragmaApiProvider.h"
#include "PragmaServerApi.h"
#include "Pragma/Api/Server/PragmaMatchApi.h"
#include "Services/PragmaAccountPartnerService.h"
#include "Services/PragmaInventoryPartnerService.h"
#include "Services/PragmaPlayerDataService.h"

PRAGMA_PTR_API(FServer);

class UPragmaSdkConfig;

namespace Pragma
{
/**
 * Represents a Game Server and its connection to the Pragma Backend. Any classes or operations accessed through this object
 * do so within the context of this Server.
 */
class PRAGMASDK_API FServer final : public FSession, public TApiProvider<UPragmaServerApi>, public TPragmaSharedFromThis<FServer>
{
public:
	DECLARE_DELEGATE_OneParam(FConnectedDelegate, const TPragmaResult<>&)

    /// Connect the Server to the Pragma backend. Required before making RPCs.
	void Connect(const FConnectedDelegate& OnComplete, const FString& ServerPort = TEXT("10100"));

	DECLARE_DELEGATE(FDisconnectedDelegate)
	void Disconnect(const FDisconnectedDelegate& OnComplete);

	// Pragma API accessors for convenience.
	// Access custom APIs with Service<MyApi>().
	UPragmaMatchApi& MatchApi() const { return Api<UPragmaMatchApi>(); }
	UPragmaAccountPartnerService& Account() const { return Api<UPragmaAccountPartnerService>(); }
	UPragmaInventoryPartnerService& Inventory() const { return Api<UPragmaInventoryPartnerService>(); }
	UPragmaPlayerDataPartnerService& PlayerDataService() const { return Api<UPragmaPlayerDataPartnerService>(); }

	virtual bool IsPartnerSession() const override { return true; }

PACKAGE_SCOPE:
	// Should only be created through the Pragma Runtime.
	static FServerStrongRef Create(FSdkConfigStrongPtr Config, TSharedPtr<FPragmaTimerManager> TimerManager)
	{
		auto Server = FServerStrongRef{new FServer(MoveTemp(Config), MoveTemp(TimerManager))};
		// This cannot be called in the constructor because we need access to AsShared.
		Server->InitializeServices();
		return Server;
	}

	virtual void InitApi(UClass* StaticClass) override;

protected:
	virtual FSessionMap& Apis() override { return ServerApis; }
	virtual const FSessionMap& Apis() const override { return ServerApis; }

	virtual TSharedPtr<FSession, SpMode> AsSharedSession() override { return AsShared(); }

private:
	void InitializeServices();
	void RegisterDefaultApis();
	void InitDefaultApis();

	FServer(FSdkConfigStrongPtr Config, TSharedPtr<FPragmaTimerManager> TimerManager);

	FSessionMap ServerApis;
};
}
