#include "PragmaServer.h"
#include "PragmaConnection.h"
#include "Dto/PragmaGeneratedServiceRegistration.h"
#include "Dto/PragmaAccountPartnerServiceRaw.h"
#include "Services/PragmaAccountPartnerService.h"
#include "Services/PragmaPlayerDataService.h"
#include "Services/PragmaSessionPartnerService.h"
#include "Services/PragmaSessionPartnerServiceRaw.h"

namespace Pragma
{
	FServer::FServer(FSdkConfigStrongPtr Config,
	                 TSharedPtr<FPragmaTimerManager> TimerManager)
		: FSession(MoveTemp(Config), MoveTemp(TimerManager))
	{
	}

	void FServer::InitializeServices()
	{
		RegisterGeneratedServerApis(*this);
		RegisterDefaultApis();
		InitDefaultApis();
	}

	void FServer::RegisterDefaultApis()
	{
		// Special case, not generated because it contains both social and game RPCs.
		RegisterApi<UPragmaSessionPartnerServiceRaw>();

		RegisterApi<UPragmaAccountPartnerService>();
		RegisterApi<UPragmaSessionPartnerService>();
		RegisterApi<UPragmaInventoryPartnerService>();
		RegisterApi<UPragmaMatchApi>();

		RegisterApi<UPragmaPlayerDataPartnerService>();

		MatchApi().SetDependencies(&Api<UPragmaGameInstancePartnerServiceRaw>(),
		   &Api<UPragmaFleetPartnerServiceRaw>(),
           &Api<UPragmaMatchCapacityPartnerServiceRaw>(),
           TimerManager()
		);
	}

	void FServer::InitDefaultApis()
	{
		UPragmaAccountPartnerService& AccountService = Api<UPragmaAccountPartnerService>();
		AccountService.SetDependencies(&Api<UPragmaAccountPartnerServiceRaw>());

		UPragmaSessionPartnerServiceRaw& SessionServiceRaw = Api<UPragmaSessionPartnerServiceRaw>();
		SessionServiceRaw.SetDependencies(AsShared());

		UPragmaSessionPartnerService& SessionService = Api<UPragmaSessionPartnerService>();
		SessionService.SetDependencies(AsShared(), &SessionServiceRaw, TimerManager(),
		                               Config().GetHeartbeatPeriodSeconds());

		UPragmaInventoryPartnerService& InventoryService = Api<UPragmaInventoryPartnerService>();
		InventoryService.SetDependencies(&Api<UPragmaInventoryPartnerServiceRaw>());

		Api<UPragmaPlayerDataPartnerService>().SetDependencies(&Api<UPragmaPlayerDataPartnerServiceRaw>());
	}

	void FServer::Connect(const FConnectedDelegate& OnComplete, const FString& ServerPort)
	{
		if (IsConnected())
		{
			auto _ = OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
			return;
		}
		Config().PartnerBackendAddress = Config().GetBackendAddress(true).Replace(TEXT("10000"), *ServerPort);
		InitLegacySession();
		Connection().UpdateBackendConfig([this, OnComplete](auto Result)
		{
			if (Result.IsFailure())
			{
				OnComplete.ExecuteIfBound(Result);
				return;
			}

			if (Config().PartnerProtocolType == EPragmaProtocolType::Http)
			{
				OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
				return;
			}

			Connection().Connect(Config().PartnerSessionSocialAuthToken,
									   Config().PartnerSessionGameAuthToken, [OnComplete](auto PragmaResult)
									   {
										   OnComplete.ExecuteIfBound(PragmaResult);
									   });
		});
	}

	void FServer::Disconnect(const FDisconnectedDelegate& OnComplete)
	{
		if (!IsConnected())
		{
			auto _ = OnComplete.ExecuteIfBound();
			return;
		}
		Connection().Disconnect();
		auto _ = OnComplete.ExecuteIfBound();
	}

	void FServer::InitApi(UClass* StaticClass)
	{
		ServerApis.FindRef(StaticClass)->Init(AsShared());
	}
}
