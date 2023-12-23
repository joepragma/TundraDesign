// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/Dto.h.mustache
// </auto-generated>
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Dto/PragmaTypesDto.h"
#include "Dto/PragmaOptionsDto.h"

#include "PragmaPlayerDataTypesDto.generated.h"

// Forward declarations.
struct FPragma_PlayerData_BigDataResponseProto;
struct FPragma_PlayerData_DemoContentOperationResponseProto;
struct FPragma_PlayerData_EmptyOperationResponseProto;
struct FPragma_PlayerData_DeleteNumberDataResponseProto;
struct FPragma_PlayerData_EchoResponseProto;
struct FPragma_PlayerData_SomeServiceResponseProto;
struct FPragma_PlayerData_MychecklistResponseOneProto;
struct FPragma_PlayerData_GrantStarterItemsResponseProto;
struct FPragma_PlayerData_ContentDrivenGrantsResponseProto;
struct FPragma_PlayerData_AddNumberTimesTwoResponseProto;
struct FPragma_PlayerData_AddNumberResponseProto;
struct FPragma_PlayerData_EchoListResponseProto;
struct FPragma_PlayerData_SomeOperatorResponseProto;
struct FPragma_PlayerData_LoadTestEntityTransformResponseProto;
struct FPragma_PlayerData_SendEmptyPayloadResponseProto;
struct FPragma_PlayerData_ManyComponentResponseProto;
struct FPragma_PlayerData_PlayerHiddenComponentResponseProto;
struct FPragma_PlayerData_SomeServiceRequestProto;
struct FPragma_PlayerData_PlayerHiddenComponentRequestProto;
struct FPragma_PlayerData_GrantStarterItemsRequestProto;
struct FPragma_PlayerData_MyChecklistRequestOneProto;
struct FPragma_PlayerData_EmptyOperationRequestProto;
struct FPragma_PlayerData_AddNumberTimesTwoRequestProto;
struct FPragma_PlayerData_SomeOperatorRequestProto;
struct FPragma_PlayerData_SendEmptyPayloadRequestProto;
struct FPragma_PlayerData_AddNumberRequestProto;
struct FPragma_PlayerData_ManyComponentRequestProto;
struct FPragma_PlayerData_DeleteNumberDataRequestProto;
struct FPragma_PlayerData_ContentDrivenGrantsRequestProto;
struct FPragma_PlayerData_EchoListRequestProto;
struct FPragma_PlayerData_LoadTestEntityTransformRequestProto;
struct FPragma_PlayerData_EchoRequestProto;
struct FPragma_PlayerData_DemoContentOperationRequestProto;
struct FPragma_PlayerData_BigDataRequestProto;
struct FPragma_PlayerData_PlayerVisibleComponentProto;
struct FPragma_PlayerData_MyComponentProto;
struct FPragma_PlayerData_WalletComponentProto;
struct FPragma_PlayerData_StringComponentProto;
struct FPragma_PlayerData_NumberDataProto;
struct FPragma_PlayerData_DemoContainedTypeProto;
struct FPragma_PlayerData_DemoNestedTypeProto;
struct FPragma_PlayerData_DemoComplexTypeProto;
struct FPragma_PlayerData_EchoComplexResponseProto;
struct FPragma_PlayerData_PlayerDataResponseProto;
struct FPragma_PlayerData_EchoComplexRequestProto;
struct FPragma_PlayerData_PlayerDataRequestProto;
struct FPragma_PlayerData_MyLiveDataComponentProto;
struct FPragma_PlayerData_PlayerHiddenComponentProto;

//
// Source protos:
// generated/pragma/playerdata/playerDataTypes.proto
//


USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_BigDataResponseProto
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_BigDataResponseProto& Lhs, const FPragma_PlayerData_BigDataResponseProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_BigDataResponseProto& Lhs, const FPragma_PlayerData_BigDataResponseProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_DemoContentOperationResponseProto
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_DemoContentOperationResponseProto& Lhs, const FPragma_PlayerData_DemoContentOperationResponseProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_DemoContentOperationResponseProto& Lhs, const FPragma_PlayerData_DemoContentOperationResponseProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_EmptyOperationResponseProto
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_EmptyOperationResponseProto& Lhs, const FPragma_PlayerData_EmptyOperationResponseProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_EmptyOperationResponseProto& Lhs, const FPragma_PlayerData_EmptyOperationResponseProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_DeleteNumberDataResponseProto
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_DeleteNumberDataResponseProto& Lhs, const FPragma_PlayerData_DeleteNumberDataResponseProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_DeleteNumberDataResponseProto& Lhs, const FPragma_PlayerData_DeleteNumberDataResponseProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_EchoResponseProto
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString Message;

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_EchoResponseProto& Lhs, const FPragma_PlayerData_EchoResponseProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_EchoResponseProto& Lhs, const FPragma_PlayerData_EchoResponseProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_SomeServiceResponseProto
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_SomeServiceResponseProto& Lhs, const FPragma_PlayerData_SomeServiceResponseProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_SomeServiceResponseProto& Lhs, const FPragma_PlayerData_SomeServiceResponseProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_MychecklistResponseOneProto
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString String;

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_MychecklistResponseOneProto& Lhs, const FPragma_PlayerData_MychecklistResponseOneProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_MychecklistResponseOneProto& Lhs, const FPragma_PlayerData_MychecklistResponseOneProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_GrantStarterItemsResponseProto
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_GrantStarterItemsResponseProto& Lhs, const FPragma_PlayerData_GrantStarterItemsResponseProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_GrantStarterItemsResponseProto& Lhs, const FPragma_PlayerData_GrantStarterItemsResponseProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_ContentDrivenGrantsResponseProto
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_ContentDrivenGrantsResponseProto& Lhs, const FPragma_PlayerData_ContentDrivenGrantsResponseProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_ContentDrivenGrantsResponseProto& Lhs, const FPragma_PlayerData_ContentDrivenGrantsResponseProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_AddNumberTimesTwoResponseProto
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	int32 Total{0};

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_AddNumberTimesTwoResponseProto& Lhs, const FPragma_PlayerData_AddNumberTimesTwoResponseProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_AddNumberTimesTwoResponseProto& Lhs, const FPragma_PlayerData_AddNumberTimesTwoResponseProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_AddNumberResponseProto
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	int32 Total{0};

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_AddNumberResponseProto& Lhs, const FPragma_PlayerData_AddNumberResponseProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_AddNumberResponseProto& Lhs, const FPragma_PlayerData_AddNumberResponseProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_EchoListResponseProto
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<FString> ListOfStrings;

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_EchoListResponseProto& Lhs, const FPragma_PlayerData_EchoListResponseProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_EchoListResponseProto& Lhs, const FPragma_PlayerData_EchoListResponseProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_SomeOperatorResponseProto
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_SomeOperatorResponseProto& Lhs, const FPragma_PlayerData_SomeOperatorResponseProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_SomeOperatorResponseProto& Lhs, const FPragma_PlayerData_SomeOperatorResponseProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_LoadTestEntityTransformResponseProto
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_LoadTestEntityTransformResponseProto& Lhs, const FPragma_PlayerData_LoadTestEntityTransformResponseProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_LoadTestEntityTransformResponseProto& Lhs, const FPragma_PlayerData_LoadTestEntityTransformResponseProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_SendEmptyPayloadResponseProto
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_SendEmptyPayloadResponseProto& Lhs, const FPragma_PlayerData_SendEmptyPayloadResponseProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_SendEmptyPayloadResponseProto& Lhs, const FPragma_PlayerData_SendEmptyPayloadResponseProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_ManyComponentResponseProto
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_ManyComponentResponseProto& Lhs, const FPragma_PlayerData_ManyComponentResponseProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_ManyComponentResponseProto& Lhs, const FPragma_PlayerData_ManyComponentResponseProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_PlayerHiddenComponentResponseProto
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_PlayerHiddenComponentResponseProto& Lhs, const FPragma_PlayerData_PlayerHiddenComponentResponseProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_PlayerHiddenComponentResponseProto& Lhs, const FPragma_PlayerData_PlayerHiddenComponentResponseProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_SomeServiceRequestProto
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_SomeServiceRequestProto& Lhs, const FPragma_PlayerData_SomeServiceRequestProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_SomeServiceRequestProto& Lhs, const FPragma_PlayerData_SomeServiceRequestProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_PlayerHiddenComponentRequestProto
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_PlayerHiddenComponentRequestProto& Lhs, const FPragma_PlayerData_PlayerHiddenComponentRequestProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_PlayerHiddenComponentRequestProto& Lhs, const FPragma_PlayerData_PlayerHiddenComponentRequestProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_GrantStarterItemsRequestProto
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_GrantStarterItemsRequestProto& Lhs, const FPragma_PlayerData_GrantStarterItemsRequestProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_GrantStarterItemsRequestProto& Lhs, const FPragma_PlayerData_GrantStarterItemsRequestProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_MyChecklistRequestOneProto
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	int32 Int{0};

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_MyChecklistRequestOneProto& Lhs, const FPragma_PlayerData_MyChecklistRequestOneProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_MyChecklistRequestOneProto& Lhs, const FPragma_PlayerData_MyChecklistRequestOneProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_EmptyOperationRequestProto
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_EmptyOperationRequestProto& Lhs, const FPragma_PlayerData_EmptyOperationRequestProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_EmptyOperationRequestProto& Lhs, const FPragma_PlayerData_EmptyOperationRequestProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_AddNumberTimesTwoRequestProto
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	int32 Number{0};

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_AddNumberTimesTwoRequestProto& Lhs, const FPragma_PlayerData_AddNumberTimesTwoRequestProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_AddNumberTimesTwoRequestProto& Lhs, const FPragma_PlayerData_AddNumberTimesTwoRequestProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_SomeOperatorRequestProto
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_SomeOperatorRequestProto& Lhs, const FPragma_PlayerData_SomeOperatorRequestProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_SomeOperatorRequestProto& Lhs, const FPragma_PlayerData_SomeOperatorRequestProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_SendEmptyPayloadRequestProto
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_SendEmptyPayloadRequestProto& Lhs, const FPragma_PlayerData_SendEmptyPayloadRequestProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_SendEmptyPayloadRequestProto& Lhs, const FPragma_PlayerData_SendEmptyPayloadRequestProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_AddNumberRequestProto
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	int32 Number{0};

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_AddNumberRequestProto& Lhs, const FPragma_PlayerData_AddNumberRequestProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_AddNumberRequestProto& Lhs, const FPragma_PlayerData_AddNumberRequestProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_ManyComponentRequestProto
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_ManyComponentRequestProto& Lhs, const FPragma_PlayerData_ManyComponentRequestProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_ManyComponentRequestProto& Lhs, const FPragma_PlayerData_ManyComponentRequestProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_DeleteNumberDataRequestProto
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_DeleteNumberDataRequestProto& Lhs, const FPragma_PlayerData_DeleteNumberDataRequestProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_DeleteNumberDataRequestProto& Lhs, const FPragma_PlayerData_DeleteNumberDataRequestProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_ContentDrivenGrantsRequestProto
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_ContentDrivenGrantsRequestProto& Lhs, const FPragma_PlayerData_ContentDrivenGrantsRequestProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_ContentDrivenGrantsRequestProto& Lhs, const FPragma_PlayerData_ContentDrivenGrantsRequestProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_EchoListRequestProto
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<FString> ListOfStrings;

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_EchoListRequestProto& Lhs, const FPragma_PlayerData_EchoListRequestProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_EchoListRequestProto& Lhs, const FPragma_PlayerData_EchoListRequestProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_LoadTestEntityTransformRequestProto
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_LoadTestEntityTransformRequestProto& Lhs, const FPragma_PlayerData_LoadTestEntityTransformRequestProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_LoadTestEntityTransformRequestProto& Lhs, const FPragma_PlayerData_LoadTestEntityTransformRequestProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_EchoRequestProto
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString Message;

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_EchoRequestProto& Lhs, const FPragma_PlayerData_EchoRequestProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_EchoRequestProto& Lhs, const FPragma_PlayerData_EchoRequestProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_DemoContentOperationRequestProto
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_DemoContentOperationRequestProto& Lhs, const FPragma_PlayerData_DemoContentOperationRequestProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_DemoContentOperationRequestProto& Lhs, const FPragma_PlayerData_DemoContentOperationRequestProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_BigDataRequestProto
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_BigDataRequestProto& Lhs, const FPragma_PlayerData_BigDataRequestProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_BigDataRequestProto& Lhs, const FPragma_PlayerData_BigDataRequestProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_PlayerVisibleComponentProto
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_PlayerVisibleComponentProto& Lhs, const FPragma_PlayerData_PlayerVisibleComponentProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_PlayerVisibleComponentProto& Lhs, const FPragma_PlayerData_PlayerVisibleComponentProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_MyComponentProto
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	int32 Num{0};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString Str;

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_MyComponentProto& Lhs, const FPragma_PlayerData_MyComponentProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_MyComponentProto& Lhs, const FPragma_PlayerData_MyComponentProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_WalletComponentProto
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	int32 Gold{0};

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_WalletComponentProto& Lhs, const FPragma_PlayerData_WalletComponentProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_WalletComponentProto& Lhs, const FPragma_PlayerData_WalletComponentProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_StringComponentProto
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString Value;

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_StringComponentProto& Lhs, const FPragma_PlayerData_StringComponentProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_StringComponentProto& Lhs, const FPragma_PlayerData_StringComponentProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_NumberDataProto
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	int32 Number{0};

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_NumberDataProto& Lhs, const FPragma_PlayerData_NumberDataProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_NumberDataProto& Lhs, const FPragma_PlayerData_NumberDataProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_DemoContainedTypeProto
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	int32 Number{0};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString Word;

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_DemoContainedTypeProto& Lhs, const FPragma_PlayerData_DemoContainedTypeProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_DemoContainedTypeProto& Lhs, const FPragma_PlayerData_DemoContainedTypeProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_DemoNestedTypeProto
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<FPragma_PlayerData_DemoContainedTypeProto> DemoList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TMap<int32, FPragma_PlayerData_DemoContainedTypeProto> DemoMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<FPragma_PlayerData_DemoContainedTypeProto> DemoMutableList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TMap<int32, FPragma_PlayerData_DemoContainedTypeProto> DemoMutableMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString Uuid;

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_DemoNestedTypeProto& Lhs, const FPragma_PlayerData_DemoNestedTypeProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_DemoNestedTypeProto& Lhs, const FPragma_PlayerData_DemoNestedTypeProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_DemoComplexTypeProto
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<FPragma_PlayerData_DemoContainedTypeProto> DemoList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<int32> DemoListInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<FString> DemoListString;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TMap<int32, FPragma_PlayerData_DemoContainedTypeProto> DemoMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<FPragma_PlayerData_DemoContainedTypeProto> DemoMutableList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TMap<int32, FPragma_PlayerData_DemoContainedTypeProto> DemoMutableMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FPragma_PlayerData_DemoNestedTypeProto NestedType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString Uuid;

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_DemoComplexTypeProto& Lhs, const FPragma_PlayerData_DemoComplexTypeProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_DemoComplexTypeProto& Lhs, const FPragma_PlayerData_DemoComplexTypeProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_EchoComplexResponseProto
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FPragma_PlayerData_DemoComplexTypeProto DemoComplexType;

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_EchoComplexResponseProto& Lhs, const FPragma_PlayerData_EchoComplexResponseProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_EchoComplexResponseProto& Lhs, const FPragma_PlayerData_EchoComplexResponseProto& Rhs);

UENUM(BlueprintType, Category=Pragma)
enum class EPragma_PlayerData_PlayerDataResponseProto_PayloadType : uint8
{
	Invalid,
	AddNumber,
	AddNumberTimesTwo,
	BigDataEntity,
	ContentDrivenGrants,
	DeleteNumberData,
	DemoContentOperation,
	Echo,
	EchoComplex,
	EchoList,
	EmptyOperation,
	GrantStarterItems,
	HideStuffFromPlayer,
	LoadTestEntityTransform,
	ManyComponentEntity,
	MyOperation,
	SendEmptyPayload,
	SomeOperatorOne,
	SomeServiceOne,
};

USTRUCT(BlueprintType, Category=Pragma)
struct PRAGMASDK_API FPragma_PlayerData_PlayerDataResponseProto_Payload
{
	GENERATED_BODY()

	FPragma_PlayerData_PlayerDataResponseProto_Payload();
	~FPragma_PlayerData_PlayerDataResponseProto_Payload();

	FPragma_PlayerData_PlayerDataResponseProto_Payload(const FPragma_PlayerData_PlayerDataResponseProto_Payload& Other);
	FPragma_PlayerData_PlayerDataResponseProto_Payload& operator=(const FPragma_PlayerData_PlayerDataResponseProto_Payload& Other);

    bool Equals(const FPragma_PlayerData_PlayerDataResponseProto_Payload& Other) const;

	void Reset();

	EPragma_PlayerData_PlayerDataResponseProto_PayloadType OneOfType() const { return Type; }

	const FPragma_PlayerData_AddNumberResponseProto& AddNumber() const;
	const FPragma_PlayerData_AddNumberTimesTwoResponseProto& AddNumberTimesTwo() const;
	const FPragma_PlayerData_BigDataResponseProto& BigDataEntity() const;
	const FPragma_PlayerData_ContentDrivenGrantsResponseProto& ContentDrivenGrants() const;
	const FPragma_PlayerData_DeleteNumberDataResponseProto& DeleteNumberData() const;
	const FPragma_PlayerData_DemoContentOperationResponseProto& DemoContentOperation() const;
	const FPragma_PlayerData_EchoResponseProto& Echo() const;
	const FPragma_PlayerData_EchoComplexResponseProto& EchoComplex() const;
	const FPragma_PlayerData_EchoListResponseProto& EchoList() const;
	const FPragma_PlayerData_EmptyOperationResponseProto& EmptyOperation() const;
	const FPragma_PlayerData_GrantStarterItemsResponseProto& GrantStarterItems() const;
	const FPragma_PlayerData_PlayerHiddenComponentResponseProto& HideStuffFromPlayer() const;
	const FPragma_PlayerData_LoadTestEntityTransformResponseProto& LoadTestEntityTransform() const;
	const FPragma_PlayerData_ManyComponentResponseProto& ManyComponentEntity() const;
	const FPragma_PlayerData_MychecklistResponseOneProto& MyOperation() const;
	const FPragma_PlayerData_SendEmptyPayloadResponseProto& SendEmptyPayload() const;
	const FPragma_PlayerData_SomeOperatorResponseProto& SomeOperatorOne() const;
	const FPragma_PlayerData_SomeServiceResponseProto& SomeServiceOne() const;

    void SetAddNumber(const FPragma_PlayerData_AddNumberResponseProto& Value);
    void SetAddNumberTimesTwo(const FPragma_PlayerData_AddNumberTimesTwoResponseProto& Value);
    void SetBigDataEntity(const FPragma_PlayerData_BigDataResponseProto& Value);
    void SetContentDrivenGrants(const FPragma_PlayerData_ContentDrivenGrantsResponseProto& Value);
    void SetDeleteNumberData(const FPragma_PlayerData_DeleteNumberDataResponseProto& Value);
    void SetDemoContentOperation(const FPragma_PlayerData_DemoContentOperationResponseProto& Value);
    void SetEcho(const FPragma_PlayerData_EchoResponseProto& Value);
    void SetEchoComplex(const FPragma_PlayerData_EchoComplexResponseProto& Value);
    void SetEchoList(const FPragma_PlayerData_EchoListResponseProto& Value);
    void SetEmptyOperation(const FPragma_PlayerData_EmptyOperationResponseProto& Value);
    void SetGrantStarterItems(const FPragma_PlayerData_GrantStarterItemsResponseProto& Value);
    void SetHideStuffFromPlayer(const FPragma_PlayerData_PlayerHiddenComponentResponseProto& Value);
    void SetLoadTestEntityTransform(const FPragma_PlayerData_LoadTestEntityTransformResponseProto& Value);
    void SetManyComponentEntity(const FPragma_PlayerData_ManyComponentResponseProto& Value);
    void SetMyOperation(const FPragma_PlayerData_MychecklistResponseOneProto& Value);
    void SetSendEmptyPayload(const FPragma_PlayerData_SendEmptyPayloadResponseProto& Value);
    void SetSomeOperatorOne(const FPragma_PlayerData_SomeOperatorResponseProto& Value);
    void SetSomeServiceOne(const FPragma_PlayerData_SomeServiceResponseProto& Value);

	static bool SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
	static bool DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);

private:
	UPROPERTY()
	EPragma_PlayerData_PlayerDataResponseProto_PayloadType Type { EPragma_PlayerData_PlayerDataResponseProto_PayloadType::Invalid };

	union
	{
		TTypeCompatibleBytes<FPragma_PlayerData_AddNumberResponseProto> AddNumber;
		TTypeCompatibleBytes<FPragma_PlayerData_AddNumberTimesTwoResponseProto> AddNumberTimesTwo;
		TTypeCompatibleBytes<FPragma_PlayerData_BigDataResponseProto> BigDataEntity;
		TTypeCompatibleBytes<FPragma_PlayerData_ContentDrivenGrantsResponseProto> ContentDrivenGrants;
		TTypeCompatibleBytes<FPragma_PlayerData_DeleteNumberDataResponseProto> DeleteNumberData;
		TTypeCompatibleBytes<FPragma_PlayerData_DemoContentOperationResponseProto> DemoContentOperation;
		TTypeCompatibleBytes<FPragma_PlayerData_EchoResponseProto> Echo;
		TTypeCompatibleBytes<FPragma_PlayerData_EchoComplexResponseProto> EchoComplex;
		TTypeCompatibleBytes<FPragma_PlayerData_EchoListResponseProto> EchoList;
		TTypeCompatibleBytes<FPragma_PlayerData_EmptyOperationResponseProto> EmptyOperation;
		TTypeCompatibleBytes<FPragma_PlayerData_GrantStarterItemsResponseProto> GrantStarterItems;
		TTypeCompatibleBytes<FPragma_PlayerData_PlayerHiddenComponentResponseProto> HideStuffFromPlayer;
		TTypeCompatibleBytes<FPragma_PlayerData_LoadTestEntityTransformResponseProto> LoadTestEntityTransform;
		TTypeCompatibleBytes<FPragma_PlayerData_ManyComponentResponseProto> ManyComponentEntity;
		TTypeCompatibleBytes<FPragma_PlayerData_MychecklistResponseOneProto> MyOperation;
		TTypeCompatibleBytes<FPragma_PlayerData_SendEmptyPayloadResponseProto> SendEmptyPayload;
		TTypeCompatibleBytes<FPragma_PlayerData_SomeOperatorResponseProto> SomeOperatorOne;
		TTypeCompatibleBytes<FPragma_PlayerData_SomeServiceResponseProto> SomeServiceOne;
	} Union{};
};
bool operator==(const FPragma_PlayerData_PlayerDataResponseProto_Payload& Lhs, const FPragma_PlayerData_PlayerDataResponseProto_Payload& Rhs);
bool operator!=(const FPragma_PlayerData_PlayerDataResponseProto_Payload& Lhs, const FPragma_PlayerData_PlayerDataResponseProto_Payload& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_PlayerDataResponseProto
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	FPragma_PlayerData_PlayerDataResponseProto_Payload Payload;

    PRAGMASDK_API static bool AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
    PRAGMASDK_API static bool AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);
};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_PlayerDataResponseProto& Lhs, const FPragma_PlayerData_PlayerDataResponseProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_PlayerDataResponseProto& Lhs, const FPragma_PlayerData_PlayerDataResponseProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_EchoComplexRequestProto
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FPragma_PlayerData_DemoComplexTypeProto DemoComplexType;

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_EchoComplexRequestProto& Lhs, const FPragma_PlayerData_EchoComplexRequestProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_EchoComplexRequestProto& Lhs, const FPragma_PlayerData_EchoComplexRequestProto& Rhs);

UENUM(BlueprintType, Category=Pragma)
enum class EPragma_PlayerData_PlayerDataRequestProto_PayloadType : uint8
{
	Invalid,
	AddNumber,
	AddNumberTimesTwo,
	BigDataEntity,
	ContentDrivenGrants,
	DeleteNumberData,
	DemoContentOperation,
	Echo,
	EchoComplex,
	EchoList,
	EmptyOperation,
	GrantStarterItems,
	HideStuffFromPlayer,
	LoadTestEntityTransform,
	ManyComponentEntity,
	MyOperation,
	SendEmptyPayload,
	SomeOperatorOne,
	SomeServiceOne,
};

USTRUCT(BlueprintType, Category=Pragma)
struct PRAGMASDK_API FPragma_PlayerData_PlayerDataRequestProto_Payload
{
	GENERATED_BODY()

	FPragma_PlayerData_PlayerDataRequestProto_Payload();
	~FPragma_PlayerData_PlayerDataRequestProto_Payload();

	FPragma_PlayerData_PlayerDataRequestProto_Payload(const FPragma_PlayerData_PlayerDataRequestProto_Payload& Other);
	FPragma_PlayerData_PlayerDataRequestProto_Payload& operator=(const FPragma_PlayerData_PlayerDataRequestProto_Payload& Other);

    bool Equals(const FPragma_PlayerData_PlayerDataRequestProto_Payload& Other) const;

	void Reset();

	EPragma_PlayerData_PlayerDataRequestProto_PayloadType OneOfType() const { return Type; }

	const FPragma_PlayerData_AddNumberRequestProto& AddNumber() const;
	const FPragma_PlayerData_AddNumberTimesTwoRequestProto& AddNumberTimesTwo() const;
	const FPragma_PlayerData_BigDataRequestProto& BigDataEntity() const;
	const FPragma_PlayerData_ContentDrivenGrantsRequestProto& ContentDrivenGrants() const;
	const FPragma_PlayerData_DeleteNumberDataRequestProto& DeleteNumberData() const;
	const FPragma_PlayerData_DemoContentOperationRequestProto& DemoContentOperation() const;
	const FPragma_PlayerData_EchoRequestProto& Echo() const;
	const FPragma_PlayerData_EchoComplexRequestProto& EchoComplex() const;
	const FPragma_PlayerData_EchoListRequestProto& EchoList() const;
	const FPragma_PlayerData_EmptyOperationRequestProto& EmptyOperation() const;
	const FPragma_PlayerData_GrantStarterItemsRequestProto& GrantStarterItems() const;
	const FPragma_PlayerData_PlayerHiddenComponentRequestProto& HideStuffFromPlayer() const;
	const FPragma_PlayerData_LoadTestEntityTransformRequestProto& LoadTestEntityTransform() const;
	const FPragma_PlayerData_ManyComponentRequestProto& ManyComponentEntity() const;
	const FPragma_PlayerData_MyChecklistRequestOneProto& MyOperation() const;
	const FPragma_PlayerData_SendEmptyPayloadRequestProto& SendEmptyPayload() const;
	const FPragma_PlayerData_SomeOperatorRequestProto& SomeOperatorOne() const;
	const FPragma_PlayerData_SomeServiceRequestProto& SomeServiceOne() const;

    void SetAddNumber(const FPragma_PlayerData_AddNumberRequestProto& Value);
    void SetAddNumberTimesTwo(const FPragma_PlayerData_AddNumberTimesTwoRequestProto& Value);
    void SetBigDataEntity(const FPragma_PlayerData_BigDataRequestProto& Value);
    void SetContentDrivenGrants(const FPragma_PlayerData_ContentDrivenGrantsRequestProto& Value);
    void SetDeleteNumberData(const FPragma_PlayerData_DeleteNumberDataRequestProto& Value);
    void SetDemoContentOperation(const FPragma_PlayerData_DemoContentOperationRequestProto& Value);
    void SetEcho(const FPragma_PlayerData_EchoRequestProto& Value);
    void SetEchoComplex(const FPragma_PlayerData_EchoComplexRequestProto& Value);
    void SetEchoList(const FPragma_PlayerData_EchoListRequestProto& Value);
    void SetEmptyOperation(const FPragma_PlayerData_EmptyOperationRequestProto& Value);
    void SetGrantStarterItems(const FPragma_PlayerData_GrantStarterItemsRequestProto& Value);
    void SetHideStuffFromPlayer(const FPragma_PlayerData_PlayerHiddenComponentRequestProto& Value);
    void SetLoadTestEntityTransform(const FPragma_PlayerData_LoadTestEntityTransformRequestProto& Value);
    void SetManyComponentEntity(const FPragma_PlayerData_ManyComponentRequestProto& Value);
    void SetMyOperation(const FPragma_PlayerData_MyChecklistRequestOneProto& Value);
    void SetSendEmptyPayload(const FPragma_PlayerData_SendEmptyPayloadRequestProto& Value);
    void SetSomeOperatorOne(const FPragma_PlayerData_SomeOperatorRequestProto& Value);
    void SetSomeServiceOne(const FPragma_PlayerData_SomeServiceRequestProto& Value);

	static bool SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
	static bool DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);

private:
	UPROPERTY()
	EPragma_PlayerData_PlayerDataRequestProto_PayloadType Type { EPragma_PlayerData_PlayerDataRequestProto_PayloadType::Invalid };

	union
	{
		TTypeCompatibleBytes<FPragma_PlayerData_AddNumberRequestProto> AddNumber;
		TTypeCompatibleBytes<FPragma_PlayerData_AddNumberTimesTwoRequestProto> AddNumberTimesTwo;
		TTypeCompatibleBytes<FPragma_PlayerData_BigDataRequestProto> BigDataEntity;
		TTypeCompatibleBytes<FPragma_PlayerData_ContentDrivenGrantsRequestProto> ContentDrivenGrants;
		TTypeCompatibleBytes<FPragma_PlayerData_DeleteNumberDataRequestProto> DeleteNumberData;
		TTypeCompatibleBytes<FPragma_PlayerData_DemoContentOperationRequestProto> DemoContentOperation;
		TTypeCompatibleBytes<FPragma_PlayerData_EchoRequestProto> Echo;
		TTypeCompatibleBytes<FPragma_PlayerData_EchoComplexRequestProto> EchoComplex;
		TTypeCompatibleBytes<FPragma_PlayerData_EchoListRequestProto> EchoList;
		TTypeCompatibleBytes<FPragma_PlayerData_EmptyOperationRequestProto> EmptyOperation;
		TTypeCompatibleBytes<FPragma_PlayerData_GrantStarterItemsRequestProto> GrantStarterItems;
		TTypeCompatibleBytes<FPragma_PlayerData_PlayerHiddenComponentRequestProto> HideStuffFromPlayer;
		TTypeCompatibleBytes<FPragma_PlayerData_LoadTestEntityTransformRequestProto> LoadTestEntityTransform;
		TTypeCompatibleBytes<FPragma_PlayerData_ManyComponentRequestProto> ManyComponentEntity;
		TTypeCompatibleBytes<FPragma_PlayerData_MyChecklistRequestOneProto> MyOperation;
		TTypeCompatibleBytes<FPragma_PlayerData_SendEmptyPayloadRequestProto> SendEmptyPayload;
		TTypeCompatibleBytes<FPragma_PlayerData_SomeOperatorRequestProto> SomeOperatorOne;
		TTypeCompatibleBytes<FPragma_PlayerData_SomeServiceRequestProto> SomeServiceOne;
	} Union{};
};
bool operator==(const FPragma_PlayerData_PlayerDataRequestProto_Payload& Lhs, const FPragma_PlayerData_PlayerDataRequestProto_Payload& Rhs);
bool operator!=(const FPragma_PlayerData_PlayerDataRequestProto_Payload& Lhs, const FPragma_PlayerData_PlayerDataRequestProto_Payload& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_PlayerDataRequestProto
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	FPragma_PlayerData_PlayerDataRequestProto_Payload Payload;

    PRAGMASDK_API static bool AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
    PRAGMASDK_API static bool AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);
};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_PlayerDataRequestProto& Lhs, const FPragma_PlayerData_PlayerDataRequestProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_PlayerDataRequestProto& Lhs, const FPragma_PlayerData_PlayerDataRequestProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_MyLiveDataComponentProto
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	int32 SomeNumber{0};

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_MyLiveDataComponentProto& Lhs, const FPragma_PlayerData_MyLiveDataComponentProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_MyLiveDataComponentProto& Lhs, const FPragma_PlayerData_MyLiveDataComponentProto& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_PlayerData_PlayerHiddenComponentProto
{
	GENERATED_BODY()

};
PRAGMASDK_API bool operator==(const FPragma_PlayerData_PlayerHiddenComponentProto& Lhs, const FPragma_PlayerData_PlayerHiddenComponentProto& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_PlayerData_PlayerHiddenComponentProto& Lhs, const FPragma_PlayerData_PlayerHiddenComponentProto& Rhs);

