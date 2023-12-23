#include "PragmaJwt.h"

#include "CoreMinimal.h"
#include "Misc/Base64.h"
#include "PragmaJson.h"
#include "PragmaResult.h"

// Linux build will fail without the cpp redeclaration.
constexpr TCHAR FPragmaJwt::Error_IncorrectSectionCount[];
constexpr TCHAR FPragmaJwt::Error_FailedToParseHeader[];
constexpr TCHAR FPragmaJwt::Error_FailedToParsePayload[];

namespace
{
void Base64UrlToBase64Inline(FString& Base64)
{
	// Per Base64 spec (https://datatracker.ietf.org/doc/html/rfc4648#section-5) values 62 and 63 are replaced
	// in a base64url-encoded string with url-safe values.
	Base64.ReplaceCharInline('-', '+');
	Base64.ReplaceCharInline('_', '/');
}

bool Base64DecodeUtf8(const FString& Source, FString& OutDest)
{
	uint32 ExpectedLength = FBase64::GetDecodedDataSize(Source);

	TArray<ANSICHAR> TempDest;
	TempDest.AddZeroed(ExpectedLength + 1);
	if(!FBase64::Decode(*Source, Source.Len(), (uint8*)TempDest.GetData()))
	{
		return false;
	}
	OutDest = UTF8_TO_TCHAR(TempDest.GetData());
	return true;
}

template <typename T>
bool ParseJwtComponent(FString Component, T& Out)
{
	Base64UrlToBase64Inline(Component);
	FString JsonBuffer;
	if (!Base64DecodeUtf8(Component, JsonBuffer))
	{
		return false;
	}
	if (!PragmaJson::JsonStrToStruct(JsonBuffer, Out))
	{
		return false;
	}
	return true;
}
}

TPragmaResult<FPragmaJwt> FPragmaJwt::FromEncoded(const FString& EncodedJwt)
{
	TArray<FString> EncodedSplit;
	const int32 Count = EncodedJwt.ParseIntoArray(EncodedSplit, TEXT("."));
	if (Count != 3)
	{
		return TPragmaResult<FPragmaJwt>::Failure(Error_IncorrectSectionCount);
	}
	
	FPragmaJwt Jwt;
	FString JsonBuffer;
	if (!ParseJwtComponent(MoveTemp(EncodedSplit[0]), Jwt.Header))
	{
		return TPragmaResult<FPragmaJwt>::Failure(Error_FailedToParseHeader);
	}
	if (!ParseJwtComponent(MoveTemp(EncodedSplit[1]), Jwt.Payload))
	{
		return TPragmaResult<FPragmaJwt>::Failure(Error_FailedToParsePayload);
	}
	
	return TPragmaResult<FPragmaJwt>{ Jwt };
}
