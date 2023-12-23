#include "Dto/PragmaCoreDto.h"

#include "Dom/JsonObject.h"

namespace
{
TSharedPtr<FJsonObject> GetSubObject(const FString& FieldName, const TSharedRef<FJsonObject>& FromObj)
{
	if (!FromObj->HasField(FieldName))
	{
		return nullptr;
	}
	return FromObj->GetObjectField(FieldName);
}
}

TSharedPtr<FJsonObject> FPragmaMessageJson::GetPayload(const TSharedRef<FJsonObject>& Message)
{
	// Check Response.
	if (Message->HasField(ResponseFieldName()))
	{
		const auto ResponseObj = Message->GetObjectField(ResponseFieldName());
		if (ResponseObj != nullptr)
		{
			return GetSubObject(FPragmaResponseJson::PayloadFieldName(), ResponseObj.ToSharedRef());
		}
	}
	// Check Notification.
	if (Message->HasField(NotificationFieldName()))
	{
		const auto NotificationObj = Message->GetObjectField(NotificationFieldName());
		if (NotificationObj != nullptr)
		{
			return GetSubObject(FPragmaNotificationJson::PayloadFieldName(), NotificationObj.ToSharedRef());
		}
	}
	return nullptr;
}

void FPragmaResponseJson::InjectPayload(const TSharedRef<FJsonObject>& Payload, const TSharedRef<FJsonObject>& Response)
{
	Response->SetObjectField(PayloadFieldName(), Payload);
}

void FPragmaNotificationJson::InjectPayload(const TSharedRef<FJsonObject>& Payload, const TSharedRef<FJsonObject>& Notification)
{
	Notification->SetObjectField(PayloadFieldName(), Payload);
}

void FPragmaRequestJson::InjectPayload(const TSharedRef<FJsonObject>& Payload, const TSharedRef<FJsonObject>& Request)
{
	Request->SetObjectField(PayloadFieldName(), Payload);
}
TSharedPtr<FJsonObject> FPragmaRequestJson::GetPayload(const TSharedRef<FJsonObject>& Request)
{
	return GetSubObject(PayloadFieldName(), Request);
}
