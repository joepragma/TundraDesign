// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/Dto.h.mustache
// </auto-generated>
#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Dto/PragmaDataRightsCommonDto.h"
#include "Dto/PragmaTypesDto.h"
#include "Dto/PragmaOptionsDto.h"

#include "PragmaDataRightsRpcDto.generated.h"

// Forward declarations.
struct FPragma_DataRights_GenerateDownloadUrlAndTokenPartnerV1Response;
struct FPragma_DataRights_GenerateDownloadUrlAndTokenPartnerV1Request;
struct FPragma_DataRights_GetPersonalDataPackagePartnerV1Response;
struct FPragma_DataRights_GetPersonalDataPackagePartnerV1Request;
struct FPragma_DataRights_SubmitRequestPartnerV1Request;
struct FPragma_DataRights_SubmitRequestV1Request;
struct FPragma_DataRights_DataRightsRequestSearchFilter;
struct FPragma_DataRights_GetRequestsPartnerV1Request;
struct FPragma_DataRights_DataRightsRequest;
struct FPragma_DataRights_SubmitRequestPartnerV1Response;
struct FPragma_DataRights_SubmitRequestV1Response;
struct FPragma_DataRights_GetRequestsPartnerV1Response;

//
// Source protos:
// pragma/datarights/dataRightsRpc.proto
//


//
// Enums.
//

UENUM(BlueprintType, Category=Pragma)
enum class EPragma_DataRights_DataRightsRequestType : uint8
{
	NO_REQUEST_TYPE = 0,
	ACCOUNT_DELETION = 1,
	PERSONAL_DATA = 2,
};

//
// Messages.
//

//
// Enums.
//

UENUM(BlueprintType, Category=Pragma)
enum class EPragma_DataRights_DataRightsRequestStatus : uint8
{
	NO_STATUS = 0,
	IN_PROGRESS = 1,
	READY = 2,
	FULFILLED = 3,
	FAILED = 4,
	CANCELED = 5,
};

//
// Messages.
//

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_DataRights_GenerateDownloadUrlAndTokenPartnerV1Response
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString DownloadUrl;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString DownloadToken;

};
PRAGMASDK_API bool operator==(const FPragma_DataRights_GenerateDownloadUrlAndTokenPartnerV1Response& Lhs, const FPragma_DataRights_GenerateDownloadUrlAndTokenPartnerV1Response& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_DataRights_GenerateDownloadUrlAndTokenPartnerV1Response& Lhs, const FPragma_DataRights_GenerateDownloadUrlAndTokenPartnerV1Response& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_DataRights_GenerateDownloadUrlAndTokenPartnerV1Request
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString RequestId;

};
PRAGMASDK_API bool operator==(const FPragma_DataRights_GenerateDownloadUrlAndTokenPartnerV1Request& Lhs, const FPragma_DataRights_GenerateDownloadUrlAndTokenPartnerV1Request& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_DataRights_GenerateDownloadUrlAndTokenPartnerV1Request& Lhs, const FPragma_DataRights_GenerateDownloadUrlAndTokenPartnerV1Request& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_DataRights_GetPersonalDataPackagePartnerV1Response
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FPragma_DataRights_PersonalDataPackage PersonalDataPackage;

};
PRAGMASDK_API bool operator==(const FPragma_DataRights_GetPersonalDataPackagePartnerV1Response& Lhs, const FPragma_DataRights_GetPersonalDataPackagePartnerV1Response& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_DataRights_GetPersonalDataPackagePartnerV1Response& Lhs, const FPragma_DataRights_GetPersonalDataPackagePartnerV1Response& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_DataRights_GetPersonalDataPackagePartnerV1Request
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString DownloadToken;

};
PRAGMASDK_API bool operator==(const FPragma_DataRights_GetPersonalDataPackagePartnerV1Request& Lhs, const FPragma_DataRights_GetPersonalDataPackagePartnerV1Request& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_DataRights_GetPersonalDataPackagePartnerV1Request& Lhs, const FPragma_DataRights_GetPersonalDataPackagePartnerV1Request& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_DataRights_SubmitRequestPartnerV1Request
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString PragmaSocialId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	EPragma_DataRights_DataRightsRequestType RequestType{static_cast<EPragma_DataRights_DataRightsRequestType>(0)};

};
PRAGMASDK_API bool operator==(const FPragma_DataRights_SubmitRequestPartnerV1Request& Lhs, const FPragma_DataRights_SubmitRequestPartnerV1Request& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_DataRights_SubmitRequestPartnerV1Request& Lhs, const FPragma_DataRights_SubmitRequestPartnerV1Request& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_DataRights_SubmitRequestV1Request
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	EPragma_DataRights_DataRightsRequestType RequestType{static_cast<EPragma_DataRights_DataRightsRequestType>(0)};

};
PRAGMASDK_API bool operator==(const FPragma_DataRights_SubmitRequestV1Request& Lhs, const FPragma_DataRights_SubmitRequestV1Request& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_DataRights_SubmitRequestV1Request& Lhs, const FPragma_DataRights_SubmitRequestV1Request& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_DataRights_DataRightsRequestSearchFilter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	EPragma_DataRights_DataRightsRequestType RequestType{static_cast<EPragma_DataRights_DataRightsRequestType>(0)};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	EPragma_DataRights_DataRightsRequestStatus RequestStatus{static_cast<EPragma_DataRights_DataRightsRequestStatus>(0)};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString PragmaSocialId;

};
PRAGMASDK_API bool operator==(const FPragma_DataRights_DataRightsRequestSearchFilter& Lhs, const FPragma_DataRights_DataRightsRequestSearchFilter& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_DataRights_DataRightsRequestSearchFilter& Lhs, const FPragma_DataRights_DataRightsRequestSearchFilter& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_DataRights_GetRequestsPartnerV1Request
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FPragma_DataRights_DataRightsRequestSearchFilter Filter;

};
PRAGMASDK_API bool operator==(const FPragma_DataRights_GetRequestsPartnerV1Request& Lhs, const FPragma_DataRights_GetRequestsPartnerV1Request& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_DataRights_GetRequestsPartnerV1Request& Lhs, const FPragma_DataRights_GetRequestsPartnerV1Request& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_DataRights_DataRightsRequest
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString RequestId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString PragmaSocialId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	EPragma_DataRights_DataRightsRequestType RequestType{static_cast<EPragma_DataRights_DataRightsRequestType>(0)};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	EPragma_DataRights_DataRightsRequestStatus RequestStatus{static_cast<EPragma_DataRights_DataRightsRequestStatus>(0)};

	UPROPERTY()
	int64 SubmittedTimestampMs{0};

	UPROPERTY()
	int64 TargetTimestampMs{0};

	UPROPERTY()
	int64 FulfilledOrCanceledTimestampMs{0};

};
PRAGMASDK_API bool operator==(const FPragma_DataRights_DataRightsRequest& Lhs, const FPragma_DataRights_DataRightsRequest& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_DataRights_DataRightsRequest& Lhs, const FPragma_DataRights_DataRightsRequest& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_DataRights_SubmitRequestPartnerV1Response
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FPragma_DataRights_DataRightsRequest Request;

};
PRAGMASDK_API bool operator==(const FPragma_DataRights_SubmitRequestPartnerV1Response& Lhs, const FPragma_DataRights_SubmitRequestPartnerV1Response& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_DataRights_SubmitRequestPartnerV1Response& Lhs, const FPragma_DataRights_SubmitRequestPartnerV1Response& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_DataRights_SubmitRequestV1Response
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FPragma_DataRights_DataRightsRequest Request;

};
PRAGMASDK_API bool operator==(const FPragma_DataRights_SubmitRequestV1Response& Lhs, const FPragma_DataRights_SubmitRequestV1Response& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_DataRights_SubmitRequestV1Response& Lhs, const FPragma_DataRights_SubmitRequestV1Response& Rhs);

USTRUCT(BlueprintType, Category=Pragma)
struct FPragma_DataRights_GetRequestsPartnerV1Response
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<FPragma_DataRights_DataRightsRequest> Requests;

};
PRAGMASDK_API bool operator==(const FPragma_DataRights_GetRequestsPartnerV1Response& Lhs, const FPragma_DataRights_GetRequestsPartnerV1Response& Rhs);
PRAGMASDK_API bool operator!=(const FPragma_DataRights_GetRequestsPartnerV1Response& Lhs, const FPragma_DataRights_GetRequestsPartnerV1Response& Rhs);

