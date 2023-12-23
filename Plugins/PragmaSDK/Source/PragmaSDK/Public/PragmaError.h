#pragma once

#include "CoreMinimal.h"

#include "CoreDto/PragmaServiceError.h"
#include "Dto/PragmaErrorsDto.h"
#include "Dto/PragmaErrorsExtDto.h"

#include "PragmaError.generated.h"

UENUM()
enum class EPragma_SdkError
{
	Unknown = 0,

	InvalidConnectionState,
	InvalidConnectionRequest,
	ProtocolShuttingDown,
	RequestFailedToSend,
	RequestHandlerExpired,
	Unauthorized,			// http equivalents 401, 403
	NotFound,				// http equivalent http 404
	PlatformUnavailable,	// http equivalents 501, 502, 503
	GenericPlatformError,	// http equivalent 500
	AlreadyLoggedIn,

	// InventoryService
	InventoryService_CraftFailed,
	InventoryService_NotInitialized,

	// PartyService
	PartyService_PartyDataOutOfDate,
	PartyService_FailedToLeaveParty,

	// MatchCapacity
	MatchCapacity_ReportCapacityTimeout,

	// Fleet
	Fleet_ReportCapacityTimeout,

	// Friend
	Friend_UnknownError,
	Friend_SessionNotInitialized,
	Friend_ApiNotInitialized,
	Friend_InviteeNotFound,
	Friend_InviteNotFound,
	Friend_InviterNotFound,
	Friend_FriendNotFound,
	Friend_InviteeAlreadyFriend,
	Friend_FriendListFull,
	Friend_InviteeReceivedInvitesFull,

	// Presence
	Presence_GameIdentityNotFound,
	Presence_PresenceNotFound
};

// PragmaError will have either an Sdk, a Platform error, or an ExtError.
USTRUCT(BlueprintType)
struct PRAGMASDK_API FPragmaError
{
	GENERATED_BODY()

public:
	static constexpr TCHAR UnknownError[] = TEXT("UNKNOWN_ERROR");

	FPragmaError() {}
	// ReSharper disable once CppNonExplicitConvertingConstructor
	FPragmaError(const EPragma_SdkError Error);
	// ReSharper disable once CppNonExplicitConvertingConstructor
	FPragmaError(const EPragma_PlatformError Error);
	// ReSharper disable once CppNonExplicitConvertingConstructor
	FPragmaError(const EPragma_ExtError Error);

	FPragmaError(const FPragmaError& Other);
	FPragmaError& operator=(const FPragmaError& Other);

	// ReSharper disable once CppNonExplicitConvertingConstructor
	FPragmaError(const FPragmaServiceError& Error);
	FPragmaError& operator=(const FPragmaServiceError& Error);

	bool operator==(const FPragmaError& Other) const
	{
		return SdkError == Other.SdkError && PlatformError == Other.PlatformError;
	}

	bool operator!=(const FPragmaError& Other) const { return !(*this == Other); }
	bool operator==(const EPragma_SdkError& Error) const { return IsSdk() && SdkError == Error; }
	bool operator==(const EPragma_PlatformError& Error) const { return IsPlatform() && PlatformError == Error; }
	bool operator!=(const EPragma_SdkError& Error) const { return !(SdkError == Error); }
	bool operator!=(const EPragma_PlatformError& Error) const { return !(PlatformError == Error); }

	bool IsSdk() const { return Which == EWhich_Is_Set::SDK; }
	bool IsPlatform() const { return Which == EWhich_Is_Set::PLATFORM; }
	bool IsExt() const { return Which == EWhich_Is_Set::EXT; }
	bool IsUnknown() const { return Which == EWhich_Is_Set::NONE; }

	EPragma_SdkError Sdk() const { return SdkError; }
	EPragma_PlatformError Platform() const { return PlatformError; }
	EPragma_ExtError Ext() const { return ExtPlatformError; }

	// Returns the string error code whether it is an SDK or Platform error.
	FString ErrorCode() const;

	FString ToString() const;

private:
	enum class EWhich_Is_Set
	{
		NONE,
		SDK,
		PLATFORM,
		EXT
	};

	EWhich_Is_Set Which{EWhich_Is_Set::NONE};

	EPragma_SdkError SdkError{EPragma_SdkError::Unknown};
	EPragma_PlatformError PlatformError{EPragma_PlatformError::UNKNOWN_ERROR};
	EPragma_ExtError ExtPlatformError{EPragma_ExtError::UNKNOWN_EXT_ERROR};
};
