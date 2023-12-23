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
#include "Dto/PragmaGameInstanceRpcDto.h"

#include "PragmaGameInstanceServiceRaw.generated.h"

struct FPragmaMessageMetadata;

UCLASS(Transient)
class PRAGMASDK_API UPragmaGameInstanceServiceRaw : public UPragmaPlayerApi
{
GENERATED_BODY()

public:
    DECLARE_EVENT_TwoParams(UPragmaGameInstanceServiceRaw, FAddedToGameV1Event, FPragma_GameInstance_AddedToGameV1Notification /* Notification */, const FPragmaMessageMetadata& /* Metadata */);
    FAddedToGameV1Event OnAddedToGameV1;
    DECLARE_EVENT_TwoParams(UPragmaGameInstanceServiceRaw, FHostConnectionDetailsV1Event, FPragma_GameInstance_HostConnectionDetailsV1Notification /* Notification */, const FPragmaMessageMetadata& /* Metadata */);
    FHostConnectionDetailsV1Event OnHostConnectionDetailsV1;
    DECLARE_EVENT_TwoParams(UPragmaGameInstanceServiceRaw, FGameInstanceStartFailureV1Event, FPragma_GameInstance_GameInstanceStartFailureV1Notification /* Notification */, const FPragmaMessageMetadata& /* Metadata */);
    FGameInstanceStartFailureV1Event OnGameInstanceStartFailureV1;
    DECLARE_EVENT_TwoParams(UPragmaGameInstanceServiceRaw, FGameInstanceTerminationV1Event, FPragma_GameInstance_GameInstanceTerminationV1Notification /* Notification */, const FPragmaMessageMetadata& /* Metadata */);
    FGameInstanceTerminationV1Event OnGameInstanceTerminationV1;
    DECLARE_EVENT_TwoParams(UPragmaGameInstanceServiceRaw, FGameInstanceReconnectV1Event, FPragma_GameInstance_GameInstanceReconnectV1Notification /* Notification */, const FPragmaMessageMetadata& /* Metadata */);
    FGameInstanceReconnectV1Event OnGameInstanceReconnectV1;
    DECLARE_EVENT_TwoParams(UPragmaGameInstanceServiceRaw, FRemovedFromGameV1Event, FPragma_GameInstance_RemovedFromGameV1Notification /* Notification */, const FPragmaMessageMetadata& /* Metadata */);
    FRemovedFromGameV1Event OnRemovedFromGameV1;
    DECLARE_EVENT_TwoParams(UPragmaGameInstanceServiceRaw, FGameEndedV1Event, FPragma_GameInstance_GameEndedV1Notification /* Notification */, const FPragmaMessageMetadata& /* Metadata */);
    FGameEndedV1Event OnGameEndedV1;

    void virtual OnInitialized() override;

    DECLARE_DELEGATE_TwoParams(FDeclineReconnectV1Delegate, TPragmaResult<FPragma_GameInstance_DeclineReconnectV1Response> /* Result */, const FPragmaMessageMetadata& /* Metadata */);
    virtual void DeclineReconnectV1(const FPragma_GameInstance_DeclineReconnectV1Request& Request, FDeclineReconnectV1Delegate Delegate) const;
    virtual void DeclineReconnectV1(const FPragma_GameInstance_DeclineReconnectV1Request& Request, TUniqueFunction<void(TPragmaResult<FPragma_GameInstance_DeclineReconnectV1Response>, const FPragmaMessageMetadata&)> Callback) const;
    DECLARE_DELEGATE_TwoParams(FGetHostConnectionDetailsV1Delegate, TPragmaResult<FPragma_GameInstance_GetHostConnectionDetailsV1Response> /* Result */, const FPragmaMessageMetadata& /* Metadata */);
    virtual void GetHostConnectionDetailsV1(const FPragma_GameInstance_GetHostConnectionDetailsV1Request& Request, FGetHostConnectionDetailsV1Delegate Delegate) const;
    virtual void GetHostConnectionDetailsV1(const FPragma_GameInstance_GetHostConnectionDetailsV1Request& Request, TUniqueFunction<void(TPragmaResult<FPragma_GameInstance_GetHostConnectionDetailsV1Response>, const FPragmaMessageMetadata&)> Callback) const;
};
