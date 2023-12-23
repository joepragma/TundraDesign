#pragma once

#include "PragmaJsonObjectConverter.h"
#include "PragmaResult.h"
#include "PragmaLog.h"
#include "PragmaRtti.h"

namespace PragmaJson
{
PRAGMASDK_API const FString& JsonFailedToDeserializeErrorCode();

PRAGMASDK_API const FPragmaJsonObjectConverter::CustomExportCallback* GetOrCreatePragmaTypeExporter();
PRAGMASDK_API const FPragmaJsonObjectConverter::CustomImportCallback* GetOrCreatePragmaTypeImporter();

PRAGMASDK_API TPragmaResult<TSharedPtr<FJsonObject>> JsonStrResultToJsonObjResult(const FString& LogPrefix, const TPragmaResult<FString>& Result);
PRAGMASDK_API FString JsonObjToStr(const TSharedRef<FJsonObject>& JsonObj);
PRAGMASDK_API TSharedPtr<FJsonObject> JsonStrToObj(const FString& Json);

template <typename T>
TSharedPtr<FJsonObject> StructToJsonObj(const T& Data)
{
	PragmaRtti::FJsonSerializer Serialize;
	if (PragmaRtti::GetSerializer(T::StaticStruct(), Serialize))
	{
		auto JsonObj = MakeShared<FJsonObject>();
		if (!Serialize(&Data, JsonObj))
		{
			PRAGMA_LOG(Error, "StructToJsonObj -- Failed to serialize type %s as json through custom serializer.", *T::StaticStruct()->GetPathName());
			return MakeShared<FJsonObject>();
		}
		return JsonObj;
	}

	auto JsonObj = FPragmaJsonObjectConverter::UStructToJsonObject(Data, 0, CPF_Transient, GetOrCreatePragmaTypeExporter()).ToSharedRef();
	PragmaRtti::FJsonAddonSerializer AddonSerialize;
	if (PragmaRtti::GetAddonSerializer(T::StaticStruct(), AddonSerialize))
	{
		if (!AddonSerialize(&Data, JsonObj))
		{
			PRAGMA_LOG(Error, "JsonObjToStruct -- Failed to deserialize custom addon for type %s as json.", *T::StaticStruct()->GetPathName());
			// Fallthrough without addon data.
		}
	}
	return JsonObj;
}

template <typename T>
bool JsonObjToStruct(const TSharedRef<FJsonObject> JsonObject, T& OutData)
{
	PragmaRtti::FJsonDeserializer Deserialize;
	if (PragmaRtti::GetDeserializer(T::StaticStruct(), Deserialize))
	{
		if (!Deserialize(JsonObject, &OutData))
		{
			PRAGMA_LOG(Error, "JsonObjToStruct -- Failed to serialize type %s as json through custom deserializer.", *T::StaticStruct()->GetPathName());
			return false;
		}
		return true;
	}

	if (!FPragmaJsonObjectConverter::JsonObjectToUStruct(JsonObject, &OutData, 0, CPF_Transient, GetOrCreatePragmaTypeImporter()))
	{
		return false;
	}
	PragmaRtti::FJsonAddonDeserializer AddonDeserialize;
	if (PragmaRtti::GetAddonDeserializer(T::StaticStruct(), AddonDeserialize))
	{
		if (!AddonDeserialize(JsonObject, &OutData))
		{
			PRAGMA_LOG(Error, "JsonObjToStruct -- Failed to deserialize custom addon for type %s as json.", *T::StaticStruct()->GetPathName());
			// Fallthrough without addon data.
		}
	}
	return true;
}

template <typename T>
bool StructToJsonStr(const T& Data, FString& OutJson)
{
	auto JsonObj = StructToJsonObj(Data);
	if (!JsonObj.IsValid())
	{
		return false;
	}
	OutJson = JsonObjToStr(JsonObj.ToSharedRef());
	return true;
}

template <typename T>
bool JsonStrToStruct(const FString& Json, T& OutData)
{
	const auto JsonObject = JsonStrToObj(Json);
	if (JsonObject == nullptr)
	{
		return false;
	}
	return JsonObjToStruct(JsonObject.ToSharedRef(), OutData);
}

template <typename T>
TPragmaResult<T> JsonResultToTypedResult(const FString& LogPrefix, const TPragmaResult<TSharedPtr<FJsonObject>>& JsonResult)
{
	if (JsonResult.IsFailure())
	{
		return TPragmaResult<T>::Failure(JsonResult.Error());
	}
	T Payload;
	if (!JsonObjToStruct(JsonResult.Payload().ToSharedRef(), Payload))
	{
		PRAGMA_LOG(Error, "%s -- Failed to deserialize result payload for type %s as json.",
			*LogPrefix, *T::StaticStruct()->GetPathName());
		return TPragmaResult<T>::Failure(JsonFailedToDeserializeErrorCode());
	}
	return TPragmaResult<T>{ MoveTemp(Payload) };
}

template <typename T>
TPragmaResult<T> JsonResultToTypedResult(const TPragmaResult<TSharedPtr<FJsonObject>>& JsonResult)
{
	static const FString LogPrefix = TEXT("JsonResultToTypedResult");
	return JsonResultToTypedResult<T>(LogPrefix, JsonResult);
}

template <typename T>
TPragmaResult<T> JsonStrResultToTypedResult(const FString& LogPrefix, const TPragmaResult<FString>& Result)
{
	if (Result.IsFailure())
	{
		return TPragmaResult<T>::Failure(Result.Error());
	}
	T Payload;
	if (!JsonStrToStruct(Result.Payload(), Payload))
	{
		PRAGMA_LOG(Error, "%s -- Failed to deserialize json string to struct type %s. Payload: %s.",
			*LogPrefix, *T::StaticStruct()->GetPathName(), *Result.Payload());
		return TPragmaResult<T>::Failure(JsonFailedToDeserializeErrorCode());
	}
	return TPragmaResult<T>{ MoveTemp(Payload) };
}

//
// All JsonValueTo* and *ToJsonValue are specified here so that code gen is simple, and we can easily expand functionality later without touching code gen.
//
PRAGMASDK_API inline int32 JsonValueToInt32(const TSharedRef<FJsonValue>& JsonValue) { return static_cast<int32>(JsonValue->AsNumber()); }
PRAGMASDK_API inline uint32 JsonValueToUInt32(const TSharedRef<FJsonValue>& JsonValue) { return static_cast<uint32>(JsonValue->AsNumber()); }
PRAGMASDK_API int64 JsonValueToInt64(const TSharedRef<FJsonValue>& JsonValue);
PRAGMASDK_API uint64 JsonValueToUInt64(const TSharedRef<FJsonValue>& JsonValue);
PRAGMASDK_API inline float JsonValueToFloat(const TSharedRef<FJsonValue>& JsonValue) { return static_cast<float>(JsonValue->AsNumber()); }
PRAGMASDK_API inline double JsonValueToDouble(const TSharedRef<FJsonValue>& JsonValue) { return JsonValue->AsNumber(); }
PRAGMASDK_API inline bool JsonValueToBool(const TSharedRef<FJsonValue>& JsonValue) { return JsonValue->AsBool(); }
PRAGMASDK_API inline FString JsonValueToString(const TSharedRef<FJsonValue>& JsonValue)  { return JsonValue->AsString(); }
PRAGMASDK_API int64 JsonValueToEnumInt(const TSharedRef<FJsonValue>& JsonValue, const UEnum* Enum);

template <typename T>
T JsonValueToEnum(const TSharedRef<FJsonValue>& JsonValue)
{
	// JsonValueToEnumInt ensures it is in valid range.
	return static_cast<T>(JsonValueToEnumInt(JsonValue, StaticEnum<T>()));
}

template <typename T>
T JsonValueToStruct(const TSharedRef<FJsonValue>& JsonValue)
{
	if (JsonValue->Type != EJson::Object)
	{
		PRAGMA_LOG(Error, "JsonValueToStruct -- Failed to deserialize type %s as a json object. The JsonValue is not of type Object.", *T::StaticStruct()->GetPathName());
		return T();
	}
	const TSharedPtr<FJsonObject> JsonObj = JsonValue->AsObject();
	check(JsonObj.IsValid()); // should not fail if Type == EJson::Object
	T Struct;
	if (!JsonObjToStruct(JsonObj.ToSharedRef(), Struct))
	{
		PRAGMA_LOG(Error, "JsonValueToStruct -- Failed to deserialize type %s as a json object.", *T::StaticStruct()->GetPathName());
		return T();
	}
	return Struct;
}

PRAGMASDK_API inline TSharedPtr<FJsonValue> Int32ToJsonValue(const int32 Value) { return MakeShared<FJsonValueNumber>(Value); }
PRAGMASDK_API inline TSharedPtr<FJsonValue> UInt32ToJsonValue(const uint32 Value) { return MakeShared<FJsonValueNumber>(Value); }
// Note we always serialize int64/uint64 as strings to preserve precision.
PRAGMASDK_API inline TSharedPtr<FJsonValue> Int64ToJsonValue(const int64 Value) { return MakeShared<FJsonValueString>(FString::Printf(TEXT("%lld"), Value)); }
PRAGMASDK_API inline TSharedPtr<FJsonValue> UInt64ToJsonValue(const uint64 Value) { return MakeShared<FJsonValueString>(FString::Printf(TEXT("%llu"), Value)); }
PRAGMASDK_API inline TSharedPtr<FJsonValue> FloatToJsonValue(const float Value) { return MakeShared<FJsonValueNumber>(Value); }
PRAGMASDK_API inline TSharedPtr<FJsonValue> DoubleToJsonValue(const double Value) { return MakeShared<FJsonValueNumber>(Value); }
PRAGMASDK_API inline TSharedPtr<FJsonValue> BoolToJsonValue(const bool Value) { return MakeShared<FJsonValueNumber>(Value); }
PRAGMASDK_API inline TSharedPtr<FJsonValue> StringToJsonValue(const FString& Value)  { return MakeShared<FJsonValueString>(Value); }
PRAGMASDK_API inline TSharedPtr<FJsonValue> StringToJsonValue(const FName& Value)  { return MakeShared<FJsonValueString>(Value.ToString()); }
// PRAGMASDK_API TSharedPtr<FJsonValue> EnumIntToJsonValue(const TSharedPtr<FJsonValue>& JsonValue, const UEnum* Enum);

template <typename T>
TSharedPtr<FJsonValue> EnumToJsonValue(const T Value)
{
	const UEnum* Enum = StaticEnum<T>();
	const FString StringValue = Enum->GetNameStringByValue(static_cast<int64>(Value));
	return MakeShared<FJsonValueString>(StringValue);
}

template <typename T>
TSharedPtr<FJsonValue> StructToJsonValue(const T& Value)
{
	const auto JsonObj = StructToJsonObj(Value);
	if (JsonObj == nullptr)
	{
		return nullptr;
	}
	return MakeShared<FJsonValueObject>(JsonObj);
}

}
