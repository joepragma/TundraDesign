// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/RawService.cpp.mustache
//
//      NOTE:
//      You can suppress Raw API files from being generated for a particular service by adding the
//      following to the pom invocation of the protoc-maven-plugin:
//      <protocPlugin>
//          <!-- other config -->
//          <args>
//              <arg>--noCodegenServices=MyServiceName</arg>
//          </args>
//      </protocPlugin>
// </auto-generated>

#include "Dto/PragmaGameManagementPartnerServiceRaw.h"

#include "PragmaServiceImplMacros.h"
#include "PragmaProtocol.h"
#include "PragmaMessageMetadata.h"

void UPragmaGameManagementPartnerServiceRaw::OnInitialized()
{
}

#define IMPL_PRAGMA_METHOD(FunctionName, RequestType, ResponseType, DelegateType) \
IMPL_PRAGMA_GAME_METHOD(UPragmaGameManagementPartnerServiceRaw, FunctionName, RequestType, ResponseType, DelegateType)

IMPL_PRAGMA_METHOD(GetAllLimitedAccessEventsPartnerV1, FPragma_GameManagement_GetAllLimitedAccessEventsPartnerV1Request, FPragma_GameManagement_GetAllLimitedAccessEventsPartnerV1Response, FGetAllLimitedAccessEventsPartnerV1Delegate);

// Must undefine for UnityBuild or could get redefinition errors.
#undef IMPL_PRAGMA_METHOD
