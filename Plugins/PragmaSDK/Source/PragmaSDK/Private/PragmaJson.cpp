#include "PragmaJson.h"

#include "PragmaRtti.h"
#include "Policies/CondensedJsonPrintPolicy.h"
#include "Runtime/Launch/Resources/Version.h"

#if ENGINE_MAJOR_VERSION < 5 && ENGINE_MINOR_VERSION < 25
namespace
{
typedef UProperty FProperty;
typedef UNumericProperty FNumericProperty;
typedef UObjectProperty FObjectProperty;
typedef UEnumProperty FEnumProperty;
typedef UStructProperty FStructProperty;
}
#define PRAGMA_CAST_PROPERTY Cast
#else
#define PRAGMA_CAST_PROPERTY CastField
#endif

namespace PragmaJson
{
const FString& JsonFailedToDeserializeErrorCode()
{
	static const FString Str = TEXT("JSON_DESERIALIZATION_FAILED");
	return Str;
}

//
// This callback handles any Pragma overrides when serializing to json.
//
const FPragmaJsonObjectConverter::CustomExportCallback* GetOrCreatePragmaTypeExporter()
{
	static FPragmaJsonObjectConverter::CustomExportCallback PragmaTypeExporter;
	if (!PragmaTypeExporter.IsBound())
	{
		PragmaTypeExporter.BindLambda([](FProperty* Property, const void* Value) -> TSharedPtr<FJsonValue>
		{
			// We explicitly override int64 serialization to serialize as strings. The platform expects this.
			// This let's us keep full precision of int64s. Note that Unreal deserializes string->int64 properly by default.
			if (FNumericProperty* NumericProperty = PRAGMA_CAST_PROPERTY<FNumericProperty>(Property))
			{
				if (NumericProperty != nullptr
					&& NumericProperty->IsInteger())
				{
					static FString Int64CppType = TEXT("int64");
					if (NumericProperty->GetCPPType() == Int64CppType)
					{
						return Int64ToJsonValue(NumericProperty->GetSignedIntPropertyValue(Value));
					}
					static FString UInt64CppType = TEXT("uint64");
					if (NumericProperty->GetCPPType() == UInt64CppType)
					{
						return UInt64ToJsonValue(NumericProperty->GetUnsignedIntPropertyValue(Value));
					}
				}
			}
			else if (FStructProperty* StructProperty = PRAGMA_CAST_PROPERTY<FStructProperty>(Property))
			{
				PragmaRtti::FJsonSerializer Serialize;
				if (PragmaRtti::GetSerializer(StructProperty->Struct, Serialize))
				{
					TSharedRef<FJsonObject> OutJsonObj = MakeShared<FJsonObject>();
					if (!Serialize(Value, OutJsonObj))
					{
	                    PRAGMA_LOG(Error, "Failed to serialize type %s to json using custom serializer. There is probably a mismatch between SDK and"
	                    	" platform types. Most likely solution is you need to regenerate SDK types from the platform proto definitions.", *Property->GetNameCPP());
						return MakeShared<FJsonValueObject>(MakeShared<FJsonObject>());
					}
					return MakeShared<FJsonValueObject>(OutJsonObj);
				}
				// Note: Addon serialization is handled in PragmaJsonObjectConverter.cpp.

				// Fallthrough.
			}
			else if (FObjectProperty* ObjectProperty = PRAGMA_CAST_PROPERTY<FObjectProperty>(Property))
			{
				// By default, UE4's JSON exporter (ConvertScalarFPropertyToJsonValue in JsonObjectConverter.cpp) only outputs instanced properties by value,
				// while normal UObject* properties output as asset references.
				// When serializing UObject* for platform (e.g. in item instance data), we explicitly override to force all UObject* properties to output by value
				// because we actually want the properties.
				UObject* Object = ObjectProperty->GetObjectPropertyValue(Value);
				if (Object != nullptr)
				{
					TSharedRef<FJsonObject> Out = MakeShared<FJsonObject>();

					// Pass CPF_UObjectWrapper SkipFlags to avoid having the UObject nativeClass FClassProperty written into the JSON (default behavior for UE JSON exporter).
					// (Extra field that looks like "nativeClass":{})
					if (FPragmaJsonObjectConverter::UStructToJsonObject(Object->GetClass(), Object, Out, 0, CPF_Transient | CPF_UObjectWrapper, GetOrCreatePragmaTypeExporter()))
					{
						TSharedRef<FJsonValueObject> JsonObject = MakeShared<FJsonValueObject>(Out);
						JsonObject->Type = EJson::Object;
						return JsonObject;
					}
				}
			}
			return TSharedPtr<FJsonValue>();
		});
	}
	return &PragmaTypeExporter;
}

//
// This callback handles any Pragma overrides when deserializing from json.
//
const FPragmaJsonObjectConverter::CustomImportCallback* GetOrCreatePragmaTypeImporter()
{
	static FPragmaJsonObjectConverter::CustomImportCallback PragmaTypeImporter;
	if (!PragmaTypeImporter.IsBound())
	{
		PragmaTypeImporter.BindLambda([](const TSharedPtr<FJsonValue>& JsonValue, FProperty* Property, void* OutValue) -> bool
		{
			if (FEnumProperty* EnumProperty = PRAGMA_CAST_PROPERTY<FEnumProperty>(Property))
			{
				const UEnum* Enum =  EnumProperty->GetEnum();
				const int64 IntValue = JsonValueToEnumInt(JsonValue.ToSharedRef(), Enum);
				EnumProperty->GetUnderlyingProperty()->SetIntPropertyValue(OutValue, IntValue);
				return true;
			}
			FNumericProperty* NumericProperty = PRAGMA_CAST_PROPERTY<FNumericProperty>(Property);
			if (NumericProperty != nullptr
				&& NumericProperty->IsInteger())
			{
                static FString Int64CppType = TEXT("int64");
				static FString UInt64CppType = TEXT("uint64");
                if (JsonValue->Type == EJson::String)
                {
	                // Parse string -> int64/uint64 ourselves so we don't lose any precision going through AsNumber (aka double).
                	// Note the default Unreal deserialization only handles int64
					if (NumericProperty->GetCPPType() == Int64CppType)
					{
						NumericProperty->SetIntPropertyValue(OutValue, JsonValueToInt64(JsonValue.ToSharedRef()));
						return true;
					}
					if (NumericProperty->GetCPPType() == UInt64CppType)
					{
						NumericProperty->SetIntPropertyValue(OutValue, JsonValueToUInt64(JsonValue.ToSharedRef()));
						return true;
					}
                }
                else
                {
                	if (NumericProperty->GetCPPType() == Int64CppType)
                	{
	                    PRAGMA_LOG(Warning, "The field %s is a %s integer and we are trying to import a json number. This can result in loss of precision in"
	                        " the deserialized value. The json should instead be a string (wrapped in double quotes), e.g. \"12345\" will deserialize to an int64 or uint64"
	                        " without loss of precision", *NumericProperty->GetFullName(), *NumericProperty->GetCPPType());
						NumericProperty->SetIntPropertyValue(OutValue, static_cast<int64>(JsonValue->AsNumber()));
						return true;
                    }
                    if (NumericProperty->GetCPPType() == UInt64CppType)
                    {
	                    PRAGMA_LOG(Warning, "The field %s is a %s integer and we are trying to import a json number. This can result in loss of precision in"
	                        " the deserialized value. The json should instead be a string (wrapped in double quotes), e.g. \"12345\" will deserialize to an int64 or uint64"
	                        " without loss of precision", *NumericProperty->GetFullName(), *NumericProperty->GetCPPType());
						NumericProperty->SetIntPropertyValue(OutValue, static_cast<uint64>(JsonValue->AsNumber()));
						return true;
                    }
                }
            }
			if (FStructProperty* StructProperty = PRAGMA_CAST_PROPERTY<FStructProperty>(Property))
			{
				PragmaRtti::FJsonDeserializer Deserialize;
				if (!PragmaRtti::GetDeserializer(StructProperty->Struct, Deserialize))
				{
					return false;
				}
				if (JsonValue->Type != EJson::Object)
				{
					PRAGMA_LOG(Error, "Failed to deserialize type %s using custom json deserializer, the target is not a json object. There is probably a"
						" mismatch between SDK and platform types. Most likely solution is you need to regenerate SDK types from the platform proto definitions.", *NumericProperty->GetPathName());
					return true;
				}
				const TSharedPtr<FJsonObject> Obj = JsonValue->AsObject();
				check(Obj.IsValid()); // should not fail if Type == EJson::Object
				if (!Deserialize(Obj.ToSharedRef(), OutValue))
				{
                    PRAGMA_LOG(Error, "Failed to deserialize type %s using custom json deserializer. There is probably a mismatch between SDK and platform"
                    	" types. Most likely solution is you need to regenerate SDK types from the platform proto definitions.", *NumericProperty->GetPathName());
				}
				return true;
			}
			return false;
		});
	}
	return &PragmaTypeImporter;
}

TPragmaResult<TSharedPtr<FJsonObject>> JsonStrResultToJsonObjResult(const FString& LogPrefix, const TPragmaResult<FString>& Result)
{
	if (Result.IsFailure())
	{
		return TPragmaResult<TSharedPtr<FJsonObject>>::Failure(Result.ErrorCode());
	}
	TSharedPtr<FJsonObject> JsonObject = JsonStrToObj(Result.Payload());
	if (JsonObject == nullptr)
	{
		PRAGMA_LOG(Error, "%s -- Failed to deserialize json string to a map. Payload: %s.",
			*LogPrefix, *Result.Payload());
		return TPragmaResult<TSharedPtr<FJsonObject>>::Failure(JsonFailedToDeserializeErrorCode());
	}
	return TPragmaResult<TSharedPtr<FJsonObject>>{ MoveTemp(JsonObject) };
}

FString JsonObjToStr(const TSharedRef<FJsonObject>& JsonObj)
{
	FString JsonStr;
	const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer
		= TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&JsonStr);
	FJsonSerializer::Serialize(JsonObj, Writer);
	return JsonStr;
}

TSharedPtr<FJsonObject> JsonStrToObj(const FString& Json)
{
	const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Json);
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	if (!FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		return nullptr;
	}
	return JsonObject;
}

int64 JsonValueToEnumInt(const TSharedRef<FJsonValue>& JsonValue, const UEnum* Enum)
{
	check(Enum);
	// Override enum serialization. This does _almost_ the same thing as the default, except that instead of erroring out on failure,
	// we warn and default to 0. This is desired when deserializing structs from platform, especially in the case where the platform is
	// ahead in versions from the SDK, and it has Errors the SDK does not know about. These should not stop the struct from deserializing
	// altogether or we won't be able to pass the error back to the requester!
	if (JsonValue->Type == EJson::String)
	{
		const FString StrValue = JsonValue->AsString();
		const int64 IntValue = Enum->GetValueByName(FName(*StrValue));
		if (IntValue == INDEX_NONE)
		{
			PRAGMA_LOG(Warning, "JsonValueToFProperty - Unable import enum %s from string value %s. Defaulting to 0 value.", *Enum->CppType, *StrValue);
			return 0;
		}
		return IntValue;
	}
	return JsonValueToInt64(JsonValue);
}

int64 JsonValueToInt64(const TSharedRef<FJsonValue>& JsonValue)
{
	// Parse strings to int64 ourselves. Default json number parses to double and loses precision.
	return FCString::Atoi64(*JsonValue->AsString());
}

uint64 JsonValueToUInt64(const TSharedRef<FJsonValue>& JsonValue)
{
	// Parse strings to uint64 ourselves. Default json number parses to double and loses precision.
	return FCString::Strtoui64(*JsonValue->AsString(), nullptr, 10);
}
}
