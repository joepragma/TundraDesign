#pragma once
#include "PragmaResult.h"

#include "PragmaJwt.generated.h"

USTRUCT()
struct FPragmaJwtHeader
{
	GENERATED_BODY()
	// Empty since we don't use these fields.
};

USTRUCT()
struct FPragmaJwtPayload
{
	GENERATED_BODY()

	UPROPERTY()
	FString DisplayName;

	UPROPERTY()
	FString Discriminator;

	UPROPERTY()
	FString PragmaPlayerId;
	
	UPROPERTY()
	FString PragmaSocialId;
	
	UPROPERTY()
	FString RefreshInMillis;
	
	UPROPERTY()
	float Exp{0}; // Expiration time in seconds

	UPROPERTY()
	float Iat{0}; // Issued at time in seconds
};

USTRUCT()
struct FPragmaJwt
{
	GENERATED_BODY()

	UPROPERTY()
	FPragmaJwtHeader Header;
	UPROPERTY()
	FPragmaJwtPayload Payload;
	// Note: We do not parse the signature at all.

	static constexpr TCHAR Error_IncorrectSectionCount[] = TEXT("JWT_INCORRECT_SECTION_COUNT");
	static constexpr TCHAR Error_FailedToParseHeader[] = TEXT("JWT_FAILED_TO_PARSE_HEADER");
	static constexpr TCHAR Error_FailedToParsePayload[] = TEXT("JWT_FAILED_TO_PARSE_PAYLOAD");
	static PRAGMASDK_API TPragmaResult<FPragmaJwt> FromEncoded(const FString& EncodedJwt);
};
