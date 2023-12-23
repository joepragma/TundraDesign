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
#include "Dto/PragmaMatchmakingRpcDto.h"

#include "PragmaMatchmakingServiceRaw.generated.h"

struct FPragmaMessageMetadata;

UCLASS(Transient)
class PRAGMASDK_API UPragmaMatchmakingServiceRaw : public UPragmaPlayerApi
{
GENERATED_BODY()

public:
    DECLARE_EVENT_TwoParams(UPragmaMatchmakingServiceRaw, FMatchmakingFailureV1Event, FPragma_Matchmaking_MatchmakingFailureV1Notification /* Notification */, const FPragmaMessageMetadata& /* Metadata */);
    FMatchmakingFailureV1Event OnMatchmakingFailureV1;

    void virtual OnInitialized() override;

    DECLARE_DELEGATE_TwoParams(FLeaveMatchmakingV2Delegate, TPragmaResult<FPragma_Matchmaking_LeaveMatchmakingV2Response> /* Result */, const FPragmaMessageMetadata& /* Metadata */);
    virtual void LeaveMatchmakingV2(const FPragma_Matchmaking_LeaveMatchmakingV2Request& Request, FLeaveMatchmakingV2Delegate Delegate) const;
    virtual void LeaveMatchmakingV2(const FPragma_Matchmaking_LeaveMatchmakingV2Request& Request, TUniqueFunction<void(TPragmaResult<FPragma_Matchmaking_LeaveMatchmakingV2Response>, const FPragmaMessageMetadata&)> Callback) const;
    DECLARE_DELEGATE_TwoParams(FGetMatchmakingInfoV2Delegate, TPragmaResult<FPragma_Matchmaking_GetMatchmakingInfoV2Response> /* Result */, const FPragmaMessageMetadata& /* Metadata */);
    virtual void GetMatchmakingInfoV2(const FPragma_Matchmaking_GetMatchmakingInfoV2Request& Request, FGetMatchmakingInfoV2Delegate Delegate) const;
    virtual void GetMatchmakingInfoV2(const FPragma_Matchmaking_GetMatchmakingInfoV2Request& Request, TUniqueFunction<void(TPragmaResult<FPragma_Matchmaking_GetMatchmakingInfoV2Response>, const FPragmaMessageMetadata&)> Callback) const;
};

