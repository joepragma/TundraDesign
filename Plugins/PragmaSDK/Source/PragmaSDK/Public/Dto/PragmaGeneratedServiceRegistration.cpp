// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/GeneratedServiceRegistration.h.mustache
// </auto-generated>

#include "PragmaGeneratedServiceRegistration.h"
#include "PragmaPlayer.h"
#include "PragmaServer.h"

#include "DemoEchoServiceRaw.h"
#include "DemoDelayedServiceRaw.h"
#include "PragmaFriendServiceRaw.h"
#include "PragmaGameInstanceServiceRaw.h"
#include "PragmaPartyServiceRaw.h"
#include "PragmaPaymentServiceRaw.h"
#include "PragmaPlayerDataServiceRaw.h"
#include "PragmaMatchmakingServiceRaw.h"
#include "PragmaInventoryServiceRaw.h"
#include "PragmaGameDataServiceRaw.h"
#include "PragmaDataRightsServiceRaw.h"
#include "PragmaTelemetryServiceRaw.h"
#include "PragmaAccountServiceRaw.h"
#include "PragmaGameInstancePartnerServiceRaw.h"
#include "PragmaGameManagementPartnerServiceRaw.h"
#include "PragmaPlayerDataPartnerServiceRaw.h"
#include "PragmaInventoryPartnerServiceRaw.h"
#include "PragmaMatchCapacityPartnerServiceRaw.h"
#include "PragmaDataRightsPartnerServiceRaw.h"
#include "PragmaTelemetryPartnerServiceRaw.h"
#include "PragmaAccountPartnerServiceRaw.h"
#include "PragmaFleetPartnerServiceRaw.h"

namespace Pragma {

void RegisterGeneratedPlayerApis(FPlayer& Player)
{
    Player.RegisterApi<UDemoEchoServiceRaw>();
    Player.RegisterApi<UDemoDelayedServiceRaw>();
    Player.RegisterApi<UPragmaFriendServiceRaw>();
    Player.RegisterApi<UPragmaGameInstanceServiceRaw>();
    Player.RegisterApi<UPragmaPartyServiceRaw>();
    Player.RegisterApi<UPragmaPaymentServiceRaw>();
    Player.RegisterApi<UPragmaPlayerDataServiceRaw>();
    Player.RegisterApi<UPragmaMatchmakingServiceRaw>();
    Player.RegisterApi<UPragmaInventoryServiceRaw>();
    Player.RegisterApi<UPragmaGameDataServiceRaw>();
    Player.RegisterApi<UPragmaDataRightsServiceRaw>();
    Player.RegisterApi<UPragmaTelemetryServiceRaw>();
    Player.RegisterApi<UPragmaAccountServiceRaw>();
}

void RegisterGeneratedServerApis(FServer& Server)
{
    Server.RegisterApi<UPragmaGameInstancePartnerServiceRaw>();
    Server.RegisterApi<UPragmaGameManagementPartnerServiceRaw>();
    Server.RegisterApi<UPragmaPlayerDataPartnerServiceRaw>();
    Server.RegisterApi<UPragmaInventoryPartnerServiceRaw>();
    Server.RegisterApi<UPragmaMatchCapacityPartnerServiceRaw>();
    Server.RegisterApi<UPragmaDataRightsPartnerServiceRaw>();
    Server.RegisterApi<UPragmaTelemetryPartnerServiceRaw>();
    Server.RegisterApi<UPragmaAccountPartnerServiceRaw>();
    Server.RegisterApi<UPragmaFleetPartnerServiceRaw>();
}

}
