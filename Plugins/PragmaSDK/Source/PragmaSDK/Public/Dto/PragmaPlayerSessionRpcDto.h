// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/Dto.h.mustache
// </auto-generated>
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Dto/PragmaSessionCommonDto.h"
#include "Dto/PragmaTypesDto.h"
#include "Dto/PragmaOptionsDto.h"

#include "PragmaPlayerSessionRpcDto.generated.h"

// Forward declarations.
struct FPragma_Session_SessionRefreshV1Response;
struct FPragma_Session_SessionRefreshV1Request;
struct FPragma_Session_HeartbeatV1Response;
struct FPragma_Session_HeartbeatV1Request;
struct FPragma_Session_RequestLogoutV1Response;
struct FPragma_Session_RequestLogoutV1Request;
struct FPragma_Session_SessionTerminatedV1Notification;
struct FPragma_Session_SessionChangedV1Notification;

//
// Source protos:
// pragma/session/playerSessionRpc.proto
//


USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Session_SessionRefreshV1Response
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_Session_SessionRefreshV1Response& Lhs, const FPragma_Session_SessionRefreshV1Response& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Session_SessionRefreshV1Response& Lhs, const FPragma_Session_SessionRefreshV1Response& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Session_SessionRefreshV1Request
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString Token;

};
PRAGMASDK_API bool operator==(const FPragma_Session_SessionRefreshV1Request& Lhs, const FPragma_Session_SessionRefreshV1Request& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Session_SessionRefreshV1Request& Lhs, const FPragma_Session_SessionRefreshV1Request& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Session_HeartbeatV1Response
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_Session_HeartbeatV1Response& Lhs, const FPragma_Session_HeartbeatV1Response& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Session_HeartbeatV1Response& Lhs, const FPragma_Session_HeartbeatV1Response& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Session_HeartbeatV1Request
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_Session_HeartbeatV1Request& Lhs, const FPragma_Session_HeartbeatV1Request& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Session_HeartbeatV1Request& Lhs, const FPragma_Session_HeartbeatV1Request& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Session_RequestLogoutV1Response
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_Session_RequestLogoutV1Response& Lhs, const FPragma_Session_RequestLogoutV1Response& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Session_RequestLogoutV1Response& Lhs, const FPragma_Session_RequestLogoutV1Response& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Session_RequestLogoutV1Request
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_Session_RequestLogoutV1Request& Lhs, const FPragma_Session_RequestLogoutV1Request& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Session_RequestLogoutV1Request& Lhs, const FPragma_Session_RequestLogoutV1Request& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Session_SessionTerminatedV1Notification
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FPragma_PlayerSession PlayerSession;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	EPragma_SessionTerminationReason Reason{static_cast<EPragma_SessionTerminationReason>(0)};

};
PRAGMASDK_API bool operator==(const FPragma_Session_SessionTerminatedV1Notification& Lhs, const FPragma_Session_SessionTerminatedV1Notification& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Session_SessionTerminatedV1Notification& Lhs, const FPragma_Session_SessionTerminatedV1Notification& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_Session_SessionChangedV1Notification
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	int32 NewVersion{0};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TMap<int32, FString> AttributesSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<int32> AttributesRemoved;

};
PRAGMASDK_API bool operator==(const FPragma_Session_SessionChangedV1Notification& Lhs, const FPragma_Session_SessionChangedV1Notification& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_Session_SessionChangedV1Notification& Lhs, const FPragma_Session_SessionChangedV1Notification& Rhs);
