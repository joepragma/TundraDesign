// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/RawService.h.mustache
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
#pragma once
#include "PragmaResult.h"
#include "PragmaServerApi.h"
#include "Dto/PragmaGameManagementRpcDto.h"

#include "PragmaGameManagementPartnerServiceRaw.generated.h"

struct FPragmaMessageMetadata;

UCLASS(Transient)
class PRAGMASDK_API UPragmaGameManagementPartnerServiceRaw : public UPragmaServerApi
{
GENERATED_BODY()

public:

    void virtual OnInitialized() override;

    DECLARE_DELEGATE_TwoParams(FGetAllLimitedAccessEventsPartnerV1Delegate, TPragmaResult<FPragma_GameManagement_GetAllLimitedAccessEventsPartnerV1Response> /* Result */, const FPragmaMessageMetadata& /* Metadata */);
    virtual void GetAllLimitedAccessEventsPartnerV1(const FPragma_GameManagement_GetAllLimitedAccessEventsPartnerV1Request& Request, FGetAllLimitedAccessEventsPartnerV1Delegate Delegate) const;
    virtual void GetAllLimitedAccessEventsPartnerV1(const FPragma_GameManagement_GetAllLimitedAccessEventsPartnerV1Request& Request, TUniqueFunction<void(TPragmaResult<FPragma_GameManagement_GetAllLimitedAccessEventsPartnerV1Response>, const FPragmaMessageMetadata&)> Callback) const;
};

