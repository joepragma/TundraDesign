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

#include "Dto/DemoDelayedServiceRaw.h"

#include "PragmaServiceImplMacros.h"
#include "PragmaProtocol.h"
#include "PragmaMessageMetadata.h"

void UDemoDelayedServiceRaw::OnInitialized()
{
}

#define IMPL_PRAGMA_METHOD(FunctionName, RequestType, ResponseType, DelegateType) \
IMPL_PRAGMA_GAME_METHOD(UDemoDelayedServiceRaw, FunctionName, RequestType, ResponseType, DelegateType)

IMPL_PRAGMA_METHOD(DelayedV1, FPragma_Delayed_DelayedV1Request, FPragma_Delayed_DelayedV1Response, FDelayedV1Delegate);

// Must undefine for UnityBuild or could get redefinition errors.
#undef IMPL_PRAGMA_METHOD
