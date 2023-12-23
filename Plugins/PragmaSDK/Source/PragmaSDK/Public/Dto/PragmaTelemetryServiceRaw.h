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
#include "PragmaPlayerApi.h"
#include "Dto/PragmaTelemetryRpcDto.h"

#include "PragmaTelemetryServiceRaw.generated.h"

struct FPragmaMessageMetadata;

UCLASS(Transient)
class PRAGMASDK_API UPragmaTelemetryServiceRaw : public UPragmaPlayerApi
{
GENERATED_BODY()

public:

    void virtual OnInitialized() override;

    DECLARE_DELEGATE_TwoParams(FPlayerEventV1Delegate, TPragmaResult<FPragma_Telemetry_PlayerEventV1Response> /* Result */, const FPragmaMessageMetadata& /* Metadata */);
    virtual void PlayerEventV1(const FPragma_Telemetry_PlayerEventV1Request& Request, FPlayerEventV1Delegate Delegate) const;
    virtual void PlayerEventV1(const FPragma_Telemetry_PlayerEventV1Request& Request, TUniqueFunction<void(TPragmaResult<FPragma_Telemetry_PlayerEventV1Response>, const FPragmaMessageMetadata&)> Callback) const;
    DECLARE_DELEGATE_TwoParams(FPlayerEventsV1Delegate, TPragmaResult<FPragma_Telemetry_PlayerEventsV1Response> /* Result */, const FPragmaMessageMetadata& /* Metadata */);
    virtual void PlayerEventsV1(const FPragma_Telemetry_PlayerEventsV1Request& Request, FPlayerEventsV1Delegate Delegate) const;
    virtual void PlayerEventsV1(const FPragma_Telemetry_PlayerEventsV1Request& Request, TUniqueFunction<void(TPragmaResult<FPragma_Telemetry_PlayerEventsV1Response>, const FPragmaMessageMetadata&)> Callback) const;
};
