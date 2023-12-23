#include "PragmaError.h"

constexpr TCHAR FPragmaError::UnknownError[];

FPragmaError::FPragmaError(const EPragma_SdkError Error)
    : SdkError(Error)
{
	Which = EWhich_Is_Set::SDK;
}

FPragmaError::FPragmaError(const EPragma_PlatformError Error)
    : PlatformError(Error)
{
	Which = EWhich_Is_Set::PLATFORM;
}

FPragmaError::FPragmaError(const EPragma_ExtError Error)
	: ExtPlatformError(Error)
{
	Which = EWhich_Is_Set::EXT;
}

FPragmaError::FPragmaError(const FPragmaError& Other)
{
	*this = Other;
}

FPragmaError& FPragmaError::operator=(const FPragmaError& Other)
{
	PlatformError = Other.PlatformError;
	ExtPlatformError= Other.ExtPlatformError;
	SdkError = Other.SdkError;
	Which = Other.Which;
	return *this;
}

FPragmaError::FPragmaError(const FPragmaServiceError& Error)
{
	*this = Error;
}

FPragmaError& FPragmaError::operator=(const FPragmaServiceError& Error)
{
	if (Error.Error != EPragma_PlatformError::UNKNOWN_ERROR)
	{
		PlatformError = Error.Error;
		Which = EWhich_Is_Set::PLATFORM;
	}
	else
	{
		ExtPlatformError = Error.ExtError;
		Which = EWhich_Is_Set::EXT;
	}
	return *this;
}


FString FPragmaError::ErrorCode() const
{
	if (IsSdk())
	{
		return StaticEnum<EPragma_SdkError>()->GetValueAsString(SdkError);
	}
	if (IsPlatform())
	{
		return StaticEnum<EPragma_PlatformError>()->GetValueAsString(PlatformError);
	}
	if (IsExt())
	{
		return StaticEnum<EPragma_ExtError>()->GetValueAsString(ExtPlatformError);
	}
	return UnknownError;
}

FString FPragmaError::ToString() const
{
	if (IsSdk())
	{
		return StaticEnum<EPragma_SdkError>()->GetValueAsString(SdkError);
	}
	if (IsPlatform())
	{
		return StaticEnum<EPragma_PlatformError>()->GetValueAsString(PlatformError);
	}
	if (IsExt())
	{
		return StaticEnum<EPragma_ExtError>()->GetValueAsString(ExtPlatformError);
	}
	return UnknownError;
}
