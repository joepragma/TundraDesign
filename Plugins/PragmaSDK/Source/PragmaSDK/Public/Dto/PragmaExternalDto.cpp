// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/Dto.cpp.mustache
// </auto-generated>

#include "PragmaExternalDto.h"
#include "CoreMinimal.h"
#include "PragmaJson.h"



bool operator==(const FPragma_ExternalRequest& Lhs, const FPragma_ExternalRequest& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.RequestId == Rhs.RequestId
			&& Lhs.Type == Rhs.Type
			&& Lhs.Payload == Rhs.Payload
;
}
bool operator!=(const FPragma_ExternalRequest& Lhs, const FPragma_ExternalRequest& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_ExternalNotification& Lhs, const FPragma_ExternalNotification& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Type == Rhs.Type
			&& Lhs.Payload == Rhs.Payload
;
}
bool operator!=(const FPragma_ExternalNotification& Lhs, const FPragma_ExternalNotification& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_ExternalResponse& Lhs, const FPragma_ExternalResponse& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.RequestId == Rhs.RequestId
			&& Lhs.TraceId == Rhs.TraceId
			&& Lhs.Type == Rhs.Type
			&& Lhs.Payload == Rhs.Payload
;
}
bool operator!=(const FPragma_ExternalResponse& Lhs, const FPragma_ExternalResponse& Rhs)
{
	return !(Lhs == Rhs);
}

FPragma_ExternalMessage_Type::FPragma_ExternalMessage_Type() {}
FPragma_ExternalMessage_Type::~FPragma_ExternalMessage_Type()
{
	Reset();
}

FPragma_ExternalMessage_Type::FPragma_ExternalMessage_Type(const FPragma_ExternalMessage_Type& Other)
{
	*this = Other;
}

bool FPragma_ExternalMessage_Type::Equals(const FPragma_ExternalMessage_Type& Other) const
{
    if (OneOfType() != Other.OneOfType())
    {
        return false;
    }
	switch (OneOfType())
	{
	case EPragma_ExternalMessage_TypeType::Response:
		return Response() == Other.Response();
	case EPragma_ExternalMessage_TypeType::Notification:
		return Notification() == Other.Notification();
	case EPragma_ExternalMessage_TypeType::ServiceError:
		return ServiceError() == Other.ServiceError();
	case EPragma_ExternalMessage_TypeType::Invalid:
        return true;
	default:
		return false;
	}
}

FPragma_ExternalMessage_Type& FPragma_ExternalMessage_Type::operator=(const FPragma_ExternalMessage_Type& Other)
{
	Reset();
	Type = Other.OneOfType();
	switch (Other.OneOfType())
	{
	case EPragma_ExternalMessage_TypeType::Response:
		new(Union.Response.GetTypedPtr()) FPragma_ExternalResponse(Other.Response());
		break;
	case EPragma_ExternalMessage_TypeType::Notification:
		new(Union.Notification.GetTypedPtr()) FPragma_ExternalNotification(Other.Notification());
		break;
	case EPragma_ExternalMessage_TypeType::ServiceError:
		new(Union.ServiceError.GetTypedPtr()) FPragma_ServiceError(Other.ServiceError());
		break;
	case EPragma_ExternalMessage_TypeType::Invalid:
	default:
		break;
	}
	return *this;
}

void FPragma_ExternalMessage_Type::Reset()
{
	switch (OneOfType())
	{
	case EPragma_ExternalMessage_TypeType::Response:
		Union.Response.GetTypedPtr()->~FPragma_ExternalResponse();
		break;
	case EPragma_ExternalMessage_TypeType::Notification:
		Union.Notification.GetTypedPtr()->~FPragma_ExternalNotification();
		break;
	case EPragma_ExternalMessage_TypeType::ServiceError:
		Union.ServiceError.GetTypedPtr()->~FPragma_ServiceError();
		break;
	case EPragma_ExternalMessage_TypeType::Invalid:
	default:
		break;
	}
	Type = EPragma_ExternalMessage_TypeType::Invalid;
}

const FPragma_ExternalResponse& FPragma_ExternalMessage_Type::Response() const {
	checkf(OneOfType() == EPragma_ExternalMessage_TypeType::Response, TEXT("Failed to retrieve Response from FPragma_ExternalMessage_Type. Check the OneOfType() before accessing the value."));
	return *Union.Response.GetTypedPtr();
}
const FPragma_ExternalNotification& FPragma_ExternalMessage_Type::Notification() const {
	checkf(OneOfType() == EPragma_ExternalMessage_TypeType::Notification, TEXT("Failed to retrieve Notification from FPragma_ExternalMessage_Type. Check the OneOfType() before accessing the value."));
	return *Union.Notification.GetTypedPtr();
}
const FPragma_ServiceError& FPragma_ExternalMessage_Type::ServiceError() const {
	checkf(OneOfType() == EPragma_ExternalMessage_TypeType::ServiceError, TEXT("Failed to retrieve ServiceError from FPragma_ExternalMessage_Type. Check the OneOfType() before accessing the value."));
	return *Union.ServiceError.GetTypedPtr();
}

void FPragma_ExternalMessage_Type::SetResponse(const FPragma_ExternalResponse& Value)
{
	Reset();
	Type = EPragma_ExternalMessage_TypeType::Response;
	new(Union.Response.GetTypedPtr()) FPragma_ExternalResponse(Value);
}
void FPragma_ExternalMessage_Type::SetNotification(const FPragma_ExternalNotification& Value)
{
	Reset();
	Type = EPragma_ExternalMessage_TypeType::Notification;
	new(Union.Notification.GetTypedPtr()) FPragma_ExternalNotification(Value);
}
void FPragma_ExternalMessage_Type::SetServiceError(const FPragma_ServiceError& Value)
{
	Reset();
	Type = EPragma_ExternalMessage_TypeType::ServiceError;
	new(Union.ServiceError.GetTypedPtr()) FPragma_ServiceError(Value);
}

bool FPragma_ExternalMessage_Type::SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj)
{
	const FPragma_ExternalMessage_Type& OneOf = static_cast<const FPragma_ExternalMessage*>(Value)->Type;
	switch (OneOf.OneOfType())
	{
		case EPragma_ExternalMessage_TypeType::Response:
			OutJsonObj->SetField("response", PragmaJson::StructToJsonValue(*OneOf.Union.Response.GetTypedPtr()));
			break;
		case EPragma_ExternalMessage_TypeType::Notification:
			OutJsonObj->SetField("notification", PragmaJson::StructToJsonValue(*OneOf.Union.Notification.GetTypedPtr()));
			break;
		case EPragma_ExternalMessage_TypeType::ServiceError:
			OutJsonObj->SetField("serviceError", PragmaJson::StructToJsonValue(*OneOf.Union.ServiceError.GetTypedPtr()));
			break;
		default:
			break;
	}
	// Per protobuf spec, it's ok to have empty oneof.
	return true;
}
bool FPragma_ExternalMessage_Type::DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue)
{
	FPragma_ExternalMessage_Type& OutOneOf = static_cast<FPragma_ExternalMessage*>(OutValue)->Type;
	if (JsonObj->HasField("response"))
	{
		OutOneOf.Type = EPragma_ExternalMessage_TypeType::Response;
		new(OutOneOf.Union.Response.GetTypedPtr()) FPragma_ExternalResponse(PragmaJson::JsonValueToStruct<FPragma_ExternalResponse>(JsonObj->TryGetField(TEXT("response")).ToSharedRef()));
		return true; // return to short circuit any other checks.
	}
	if (JsonObj->HasField("notification"))
	{
		OutOneOf.Type = EPragma_ExternalMessage_TypeType::Notification;
		new(OutOneOf.Union.Notification.GetTypedPtr()) FPragma_ExternalNotification(PragmaJson::JsonValueToStruct<FPragma_ExternalNotification>(JsonObj->TryGetField(TEXT("notification")).ToSharedRef()));
		return true; // return to short circuit any other checks.
	}
	if (JsonObj->HasField("serviceError"))
	{
		OutOneOf.Type = EPragma_ExternalMessage_TypeType::ServiceError;
		new(OutOneOf.Union.ServiceError.GetTypedPtr()) FPragma_ServiceError(PragmaJson::JsonValueToStruct<FPragma_ServiceError>(JsonObj->TryGetField(TEXT("serviceError")).ToSharedRef()));
		return true; // return to short circuit any other checks.
	}
	// Per protobuf spec, oneofs have _at most_ one of the fields. None set is valid, so we always return true.
	return true;
}

bool operator==(const FPragma_ExternalMessage_Type& Lhs, const FPragma_ExternalMessage_Type& Rhs)
{
	return Lhs.Equals(Rhs);
}
bool operator!=(const FPragma_ExternalMessage_Type& Lhs, const FPragma_ExternalMessage_Type& Rhs)
{
	return !(Lhs == Rhs);
}


bool FPragma_ExternalMessage::AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj)
{
	bool Success = true;
	Success &= FPragma_ExternalMessage_Type::SerializeIntoParentJson(Value, OutJsonObj);
	return Success;
}

bool FPragma_ExternalMessage::AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue)
{
	bool Success = true;
	Success &= FPragma_ExternalMessage_Type::DeserializeFromParentJson(JsonObj, OutValue);
	return Success;
}

bool operator==(const FPragma_ExternalMessage& Lhs, const FPragma_ExternalMessage& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.SequenceNumber == Rhs.SequenceNumber
			&& Lhs.Type == Rhs.Type
;
}
bool operator!=(const FPragma_ExternalMessage& Lhs, const FPragma_ExternalMessage& Rhs)
{
	return !(Lhs == Rhs);
}
