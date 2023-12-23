#pragma once

#include "PragmaError.h"
#include "PragmaProtocolError.generated.h"

UENUM(BlueprintType, Category=Pragma)
enum class EPragmaProtocolError : uint8
{
	None,
	ServiceUnavailable,
	BadRequest,
	BadConfiguration,
	NoPermission,
	Expired,
	Unknown
};

inline FString EProtocolErrorToFString(const EPragmaProtocolError Error)
{
	switch(Error)
	{
	case EPragmaProtocolError::ServiceUnavailable:
		return TEXT("ServiceUnavailable");
	case EPragmaProtocolError::BadRequest:
		return TEXT("BadRequest");
	case EPragmaProtocolError::BadConfiguration:
		return TEXT("BadConfiguration");
	case EPragmaProtocolError::NoPermission:
		return TEXT("NoPermission");
	case EPragmaProtocolError::Expired:
		return TEXT("Expired");
	case EPragmaProtocolError::Unknown: default:
		return TEXT("UNKNOWN");
	}
}

inline FPragmaError EProtocolErrorToPragmaError(const EPragmaProtocolError Error)
{
	auto SdkError = EPragma_SdkError::Unknown;
	switch(Error)
	{
	case EPragmaProtocolError::ServiceUnavailable:
		SdkError = EPragma_SdkError::PlatformUnavailable;
		break;
	case EPragmaProtocolError::BadRequest:
		SdkError = EPragma_SdkError::InvalidConnectionRequest;
		break;
	case EPragmaProtocolError::BadConfiguration:
		SdkError = EPragma_SdkError::InvalidConnectionRequest;
		break;
	case EPragmaProtocolError::NoPermission:
		SdkError = EPragma_SdkError::Unauthorized;
		break;
	case EPragmaProtocolError::Expired:
		SdkError = EPragma_SdkError::Unauthorized;
		break;
	default:
		break;
	}
	return FPragmaError{SdkError};
}
