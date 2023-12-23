#pragma once

#include "CoreMinimal.h"
#include "CoreDto/PragmaServiceError.h"

#include "PragmaCoreDto.generated.h"

class FJsonObject;

//
// Types here are used internally to PragmaSocket. These 'envelopes' wrap message payloads in both directions, and
// contain general information for communicating with the backend.
//
// Source protos:
// gg.pragma.core
//

//
// Wire format used for PragmaUUIDs.
//
USTRUCT()
struct FPragmaFixed128
{
	GENERATED_BODY()

	// Least significant bit.
	UPROPERTY()
	int64 Lsb{0};

	// Most significant bit.
	UPROPERTY()
	int64 Msb{0};
};

UENUM(BlueprintType, Category=Pragma)
enum class EPragmaResponseStatusCode : uint8
{
	// ReSharper disable CppUE4CodingStandardNamingViolationWarning
	// These names must be the same as the proto file definition. When converted to json, the value is encoded as a
	// string with the enum value name, and protobuf will try to match that to the enum value name in the proto def.

	ERROR_UNKNOWN = 0,     // Old clients getting new enum values for errors will get this.
	INTERNAL_ERROR = 1,    // Unqualified error, likely server side uncaught exception.
	OK = 2,                // Processed by service, no errors, payload is valid.
	SERVICE_ERROR = 3,     // Processed by service, errored, payload is NOT valid, see error fields.
	SHUTTING_DOWN = 4,     // Cluster is shutting down.
	BAD_SESSION = 5,       // Session is not authenticated (kicked/timeout/Gateway migration/etc).
	BAD_PARAM = 6,         // Request payload was not structurally correct.

	// ReSharper restore CppUE4CodingStandardNamingViolationWarning
};

//
// A response from the backend for a specific request the client sent.
//
USTRUCT()
struct FPragmaResponseJson
{
	GENERATED_BODY()

	// Inject a json object as the Payload value for this Notification.
	static void InjectPayload(const TSharedRef<FJsonObject>& Payload, const TSharedRef<FJsonObject>& Response);

	// Client-side tracking of request to response.
	UPROPERTY()
	int32 RequestId{0};

	// Qualified Pragma type of the Payload.
	UPROPERTY()
	FString Type;

	// Payload - A json payload from the method invoked on the backend.
	//
	// This field is not included here because it's a full json object (not escaped json str) and needs to be
	// extracted and deserialized separately in order to separate the message type from the payload type.
	static const FString& PayloadFieldName() { static const FString S{TEXT("payload")}; return S; }
};

//
// A response from the backend for a specific request the client sent.
//
USTRUCT()
struct FPragmaNotificationJson
{
	GENERATED_BODY()

	// Inject a json object as the Payload value for this Notification.
	static void InjectPayload(const TSharedRef<FJsonObject>& Payload, const TSharedRef<FJsonObject>& Notification);

	UPROPERTY()
	FString Type;

	// Payload - A json payload from the method invoked on the backend.
	//
	// This field is not included here because it's a full json object (not escaped json str) and needs to be
	// extracted and deserialized separately in order to separate the message type from the payload type.
	static const FString& PayloadFieldName() { static const FString S{TEXT("payload")}; return S; }
};

//
// All Pragma messages received from the backend are wrapped in this type.
//
USTRUCT()
struct FPragmaMessageJson
{
	GENERATED_BODY()

	// Find the Payload in either the Response or Notification, if there is any. Returns nullptr if missing.
	static TSharedPtr<FJsonObject> GetPayload(const TSharedRef<FJsonObject>& Message);

	// A monotonically increasing id for messages from the backend across the lifetime of the connection.
	UPROPERTY()
	int32 SequenceNumber{0};

	//
	// Only one of the fields below will exist for each message.
	//

	// A message sent by the backend in response to a client's request.
	// E.g. When a client asks for its player data.
	UPROPERTY()
	FPragmaResponseJson Response;
	// Expected field name so we can find it when extracting the payload.
	static const FString& ResponseFieldName() { static const FString S{TEXT("response")}; return S; }

	// A message sent by the backend without any action on the client's part.
	// E.g. When a player is sent a chat message by another player.
	UPROPERTY()
	FPragmaNotificationJson Notification;
	// Expected field name so we can find it when extracting the payload.
	static const FString& NotificationFieldName() { static const FString S{TEXT("notification")}; return S; }

	// A message sent by the backend that indicates an error occured in processing a client's request.
	UPROPERTY()
	FPragmaServiceError ServiceError;
};

//
// All requests to the backend use this type.
//
USTRUCT()
struct FPragmaRequestJson
{
	GENERATED_BODY()

	// Inject a json object as the Payload value for this Request.
	static void InjectPayload(const TSharedRef<FJsonObject>& Payload, const TSharedRef<FJsonObject>& Request);

	// Get the Payload object from this Request.
	static TSharedPtr<FJsonObject> GetPayload(const TSharedRef<FJsonObject>& Request);

	// Client-side tracking of request to response.
	UPROPERTY()
	int32 RequestId{0};

	// Qualified Pragma type of the Payload.
	UPROPERTY()
	FString Type;

	// Payload - A json payload the target backend method expects.
	//
	// This field is not included here because it's a full json object (not escaped json str) and needs to be
	// extracted and deserialized separately in order to separate the message type from the payload type.
	static const FString& PayloadFieldName() { static const FString S{TEXT("payload")}; return S; }
};

USTRUCT()
struct FIntAny
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Type{0};

	// This field is not included here because it's a full json object (not escaped json str) and needs to be
	// extracted and deserialized separately in order to separate the message type from the payload type.
	static const FString& DataFieldName() { static const FString S{TEXT("data")}; return S; }

	// Get the Payload object from this Request.
	static TSharedPtr<FJsonObject> GetData(const TSharedRef<FJsonObject>& Request);
};
