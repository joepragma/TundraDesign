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
#include "Dto/PragmaMatchCapacityRpcDto.h"

#include "PragmaMatchCapacityPartnerServiceRaw.generated.h"

struct FPragmaMessageMetadata;

UCLASS(Transient)
class PRAGMASDK_API UPragmaMatchCapacityPartnerServiceRaw : public UPragmaServerApi
{
GENERATED_BODY()

public:

    void virtual OnInitialized() override;

    DECLARE_DELEGATE_TwoParams(FReportCapacityV2Delegate, TPragmaResult<FPragma_MatchCapacity_ReportCapacityV2Response> /* Result */, const FPragmaMessageMetadata& /* Metadata */);
    virtual void ReportCapacityV2(const FPragma_MatchCapacity_ReportCapacityV2Request& Request, FReportCapacityV2Delegate Delegate) const;
    virtual void ReportCapacityV2(const FPragma_MatchCapacity_ReportCapacityV2Request& Request, TUniqueFunction<void(TPragmaResult<FPragma_MatchCapacity_ReportCapacityV2Response>, const FPragmaMessageMetadata&)> Callback) const;
};

