// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

/////////////////////////////////////////////////////
// PRAGMA MODIFICATIONS:
// This is directly from the Unreal Engine JsonObjectConverter with the exceptions below.
// Relevant code is wrapped in `begin/end pragma modification` tags. Parameters are only wrapped at the method definition.
//
// - Added an ImportCallback during deserialization to UStruct, in the same way an ExportCallback
// can be defined when serializing. The ImportCallback follows the ExportCallback pattern almost exactly.
//
// - Added U*Property -> F*Property typedefs to handle previous unreal versions.
//
// - Added PragmaGeneratedRtti addon de/serializers after default struct de/serialization.
//		This is necessary because the Import/Export callbacks only happen for the entire type at the beginning
/////////////////////////////////////////////////////

#include "PragmaJsonObjectConverter.h"
#include "Internationalization/Culture.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Class.h"
#include "UObject/UnrealType.h"
#include "UObject/EnumProperty.h"
#include "UObject/TextProperty.h"
#include "UObject/PropertyPortFlags.h"
#include "UObject/Package.h"
#include "Policies/CondensedJsonPrintPolicy.h"
#include "JsonObjectWrapper.h"
#include "PragmaLog.h"
#include "PragmaRtti.h"

FString FPragmaJsonObjectConverter::StandardizeCase(const FString &StringIn)
{
	// this probably won't work for all cases, consider downcaseing the string fully
	FString FixedString = StringIn;
	FixedString[0] = FChar::ToLower(FixedString[0]); // our json classes/variable start lower case
	FixedString.ReplaceInline(TEXT("ID"), TEXT("Id"), ESearchCase::CaseSensitive); // Id is standard instead of ID, some of our fnames use ID
	return FixedString;
}

namespace
{
/////////////// begin pragma modification ///////////////
#if ENGINE_MAJOR_VERSION < 5 && ENGINE_MINOR_VERSION < 25
typedef UProperty FProperty;
typedef UNumericProperty FNumericProperty;
typedef UEnumProperty FEnumProperty;
typedef UObjectProperty FObjectProperty;
typedef UBoolProperty FBoolProperty;
typedef UTextProperty FTextProperty;
typedef UMapProperty FMapProperty;
typedef UArrayProperty FArrayProperty;
typedef UStructProperty FStructProperty;
typedef UStrProperty FStrProperty;
typedef USetProperty FSetProperty;
#define PRAGMA_CAST_PROPERTY Cast
#else
#define PRAGMA_CAST_PROPERTY CastField
#endif
//////////////// end pragma modification ////////////////

const TCHAR* ImportText(const TCHAR* ImportTextPtr, FProperty* Property, void* OutValue) {
#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1
	return Property->ImportText_Direct(ImportTextPtr, OutValue, nullptr, PPF_None);
#else
	return Property->ImportText(ImportTextPtr, OutValue, PPF_None, nullptr);
#endif
}

/** Convert property to JSON, assuming either the property is not an array or the value is an individual array element */
TSharedPtr<FJsonValue> ConvertScalarUPropertyToJsonValue(FProperty* Property, const void* Value, int64 CheckFlags, int64 SkipFlags, const FPragmaJsonObjectConverter::CustomExportCallback* ExportCb)
{
	// See if there's a custom export callback first, so it can override default behavior
	if (ExportCb && ExportCb->IsBound())
	{
		TSharedPtr<FJsonValue> CustomValue = ExportCb->Execute(Property, Value);
		if (CustomValue.IsValid())
		{
			return CustomValue;
		}
		// fall through to default cases
	}

	if (FEnumProperty* EnumProperty = PRAGMA_CAST_PROPERTY<FEnumProperty>(Property))
	{
		// export enums as strings
		UEnum* EnumDef = EnumProperty->GetEnum();
		FString StringValue = EnumDef->GetNameStringByValue(EnumProperty->GetUnderlyingProperty()->GetSignedIntPropertyValue(Value));
		return MakeShared<FJsonValueString>(StringValue);
	}
	else if (FNumericProperty *NumericProperty = PRAGMA_CAST_PROPERTY<FNumericProperty>(Property))
	{
		// see if it's an enum
		UEnum* EnumDef = NumericProperty->GetIntPropertyEnum();
		if (EnumDef != NULL)
		{
			// export enums as strings
			FString StringValue = EnumDef->GetNameStringByValue(NumericProperty->GetSignedIntPropertyValue(Value));
			return MakeShared<FJsonValueString>(StringValue);
		}

		// We want to export numbers as numbers
		if (NumericProperty->IsFloatingPoint())
		{
			return MakeShared<FJsonValueNumber>(NumericProperty->GetFloatingPointPropertyValue(Value));
		}
		else if (NumericProperty->IsInteger())
		{
			return MakeShared<FJsonValueNumber>(NumericProperty->GetSignedIntPropertyValue(Value));
		}

		// fall through to default
	}
	else if (FBoolProperty *BoolProperty = PRAGMA_CAST_PROPERTY<FBoolProperty>(Property))
	{
		// Export bools as bools
		return MakeShared<FJsonValueBoolean>(BoolProperty->GetPropertyValue(Value));
	}
	else if (FStrProperty *StringProperty = PRAGMA_CAST_PROPERTY<FStrProperty>(Property))
	{
		return MakeShared<FJsonValueString>(StringProperty->GetPropertyValue(Value));
	}
	else if (FTextProperty *TextProperty = PRAGMA_CAST_PROPERTY<FTextProperty>(Property))
	{
		return MakeShared<FJsonValueString>(TextProperty->GetPropertyValue(Value).ToString());
	}
	else if (FArrayProperty *ArrayProperty = PRAGMA_CAST_PROPERTY<FArrayProperty>(Property))
	{
		TArray< TSharedPtr<FJsonValue> > Out;
		FScriptArrayHelper Helper(ArrayProperty, Value);
		for (int32 i=0, n=Helper.Num(); i<n; ++i)
		{
			TSharedPtr<FJsonValue> Elem = FPragmaJsonObjectConverter::UPropertyToJsonValue(ArrayProperty->Inner, Helper.GetRawPtr(i), CheckFlags & ( ~CPF_ParmFlags ), SkipFlags, ExportCb);
			if ( Elem.IsValid() )
			{
				// add to the array
				Out.Push(Elem);
			}
		}
		return MakeShared<FJsonValueArray>(Out);
	}
	else if ( FSetProperty* SetProperty = PRAGMA_CAST_PROPERTY<FSetProperty>(Property) )
	{
		TArray< TSharedPtr<FJsonValue> > Out;
		FScriptSetHelper Helper(SetProperty, Value);
		for ( int32 i=0, n=Helper.Num(); n; ++i )
		{
			if ( Helper.IsValidIndex(i) )
			{
				TSharedPtr<FJsonValue> Elem = FPragmaJsonObjectConverter::UPropertyToJsonValue(SetProperty->ElementProp, Helper.GetElementPtr(i), CheckFlags & ( ~CPF_ParmFlags ), SkipFlags, ExportCb);
				if ( Elem.IsValid() )
				{
					// add to the array
					Out.Push(Elem);
				}

				--n;
			}
		}
		return MakeShared<FJsonValueArray>(Out);
	}
	else if ( FMapProperty* MapProperty = PRAGMA_CAST_PROPERTY<FMapProperty>(Property) )
	{
		TSharedRef<FJsonObject> Out = MakeShared<FJsonObject>();

		FScriptMapHelper Helper(MapProperty, Value);
		for ( int32 i=0, n = Helper.Num(); n; ++i )
		{
			if ( Helper.IsValidIndex(i) )
			{
				TSharedPtr<FJsonValue> KeyElement = FPragmaJsonObjectConverter::UPropertyToJsonValue(MapProperty->KeyProp, Helper.GetKeyPtr(i), CheckFlags & ( ~CPF_ParmFlags ), SkipFlags, ExportCb);
				TSharedPtr<FJsonValue> ValueElement = FPragmaJsonObjectConverter::UPropertyToJsonValue(MapProperty->ValueProp, Helper.GetValuePtr(i), CheckFlags & ( ~CPF_ParmFlags ), SkipFlags, ExportCb);
				if ( KeyElement.IsValid() && ValueElement.IsValid() )
				{
					FString KeyString;
					if (!KeyElement->TryGetString(KeyString))
					{
						MapProperty->KeyProp->ExportText_Direct(KeyString, Helper.GetKeyPtr(i), nullptr, nullptr, 0);
						if (KeyString.IsEmpty())
						{
							UE_LOG(LogJson, Error, TEXT("Unable to convert key to string for property %s."), *MapProperty->GetName())
							KeyString = FString::Printf(TEXT("Unparsed Key %d"), i);
						}
					}

					Out->SetField(KeyString, ValueElement);
				}

				--n;
			}
		}

		return MakeShared<FJsonValueObject>(Out);
	}
	else if (FStructProperty *StructProperty = PRAGMA_CAST_PROPERTY<FStructProperty>(Property))
	{
		UScriptStruct::ICppStructOps* TheCppStructOps = StructProperty->Struct->GetCppStructOps();
		// Intentionally exclude the JSON Object wrapper, which specifically needs to export JSON in an object representation instead of a string
		if (StructProperty->Struct != FJsonObjectWrapper::StaticStruct() && TheCppStructOps && TheCppStructOps->HasExportTextItem())
		{
			FString OutValueStr;
			TheCppStructOps->ExportTextItem(OutValueStr, Value, nullptr, nullptr, PPF_None, nullptr);
			return MakeShared<FJsonValueString>(OutValueStr);
		}

		TSharedRef<FJsonObject> Out = MakeShared<FJsonObject>();
		if (FPragmaJsonObjectConverter::UStructToJsonObject(StructProperty->Struct, Value, Out, CheckFlags & (~CPF_ParmFlags), SkipFlags, ExportCb))
		{
			/////////////// begin pragma modification ///////////////
			PragmaRtti::FJsonAddonSerializer Serialize;
			if (PragmaRtti::GetAddonSerializer(StructProperty->Struct, Serialize))
			{
				if (!Serialize(Value, Out))
				{
					PRAGMA_LOG(Error, "Failed to serialize addon data type %s to json using custom serializer addon. There is probably a mismatch between SDK and"
                        " platform types. Most likely solution is you need to regenerate SDK types from the platform proto definitions.", *Property->GetNameCPP());
					// Fallthrough without addon data.
				}
			}
			//////////////// end pragma modification ////////////////
			return MakeShared<FJsonValueObject>(Out);
		}
		// fall through to default
	}
	else
	{
		// Default to export as string for everything else
		FString StringValue;
		Property->ExportText_Direct(StringValue, Value, NULL, NULL, PPF_None);
		return MakeShared<FJsonValueString>(StringValue);
	}

	// invalid
	return TSharedPtr<FJsonValue>();
}
}

TSharedPtr<FJsonValue> FPragmaJsonObjectConverter::ObjectJsonCallback(FProperty* Property, const void* Value)
{
	if (FObjectProperty* ObjectProperty = PRAGMA_CAST_PROPERTY<FObjectProperty>(Property))
	{
		if (!ObjectProperty->HasAnyFlags(RF_Transient)) // We are taking Transient to mean we don't want to serialize to Json either (could make a new flag if nessasary)
		{
			TSharedRef<FJsonObject> Out = MakeShared<FJsonObject>();

			CustomExportCallback CustomCB;
			CustomCB.BindStatic(FPragmaJsonObjectConverter::ObjectJsonCallback);

			void** PtrToValuePtr = (void**)Value;

			if (FPragmaJsonObjectConverter::UStructToJsonObject(ObjectProperty->PropertyClass, (*PtrToValuePtr), Out, 0, 0, &CustomCB))
			{
				return MakeShared<FJsonValueObject>(Out);
			}
		}
	}

	// invalid
	return TSharedPtr<FJsonValue>();
}

TSharedPtr<FJsonValue> FPragmaJsonObjectConverter::UPropertyToJsonValue(FProperty* Property, const void* Value, int64 CheckFlags, int64 SkipFlags, const CustomExportCallback* ExportCb)
{
	if (Property->ArrayDim == 1)
	{
		return ConvertScalarUPropertyToJsonValue(Property, Value, CheckFlags, SkipFlags, ExportCb);
	}

	TArray< TSharedPtr<FJsonValue> > Array;
	for (int Index = 0; Index != Property->ArrayDim; ++Index)
	{
		Array.Add(ConvertScalarUPropertyToJsonValue(Property, (char*)Value + Index * Property->ElementSize, CheckFlags, SkipFlags, ExportCb));
	}
	return MakeShared<FJsonValueArray>(Array);
}

bool FPragmaJsonObjectConverter::UStructToJsonObject(const UStruct* StructDefinition, const void* Struct, TSharedRef<FJsonObject> OutJsonObject, int64 CheckFlags, int64 SkipFlags, const CustomExportCallback* ExportCb)
{
	return UStructToJsonAttributes(StructDefinition, Struct, OutJsonObject->Values, CheckFlags, SkipFlags, ExportCb);
}

bool FPragmaJsonObjectConverter::UStructToJsonAttributes(const UStruct* StructDefinition, const void* Struct, TMap< FString, TSharedPtr<FJsonValue> >& OutJsonAttributes, int64 CheckFlags, int64 SkipFlags, const CustomExportCallback* ExportCb)
{
	if (SkipFlags == 0)
	{
		// If we have no specified skip flags, skip deprecated, transient and skip serialization by default when writing
		SkipFlags |= CPF_Deprecated | CPF_Transient;
	}

	if (StructDefinition == FJsonObjectWrapper::StaticStruct())
	{
		// Just copy it into the object
		const FJsonObjectWrapper* ProxyObject = (const FJsonObjectWrapper *)Struct;

		if (ProxyObject->JsonObject.IsValid())
		{
			OutJsonAttributes = ProxyObject->JsonObject->Values;
		}
		return true;
	}

	for (TFieldIterator<FProperty> It(StructDefinition); It; ++It)
	{
		FProperty* Property = *It;

		// Check to see if we should ignore this property
		if (CheckFlags != 0 && !Property->HasAnyPropertyFlags(CheckFlags))
		{
			continue;
		}
		if (Property->HasAnyPropertyFlags(SkipFlags))
		{
			continue;
		}

		// Unreal's Json serialization is based on FProperty names, which are FNames. These are _case-insensitive_ and can vary between builds, so are
		// NOT a stable way of handling serialization. Pragma code gens some RTTI so we can explicitly specify what name should be used when serializing a field.
		const auto* OverrideName = PragmaGeneratedRtti::GetFieldNameForSerialization(StructDefinition, Property);
		FString VariableName = StandardizeCase(OverrideName != nullptr ? *OverrideName : Property->GetName());
		const void* Value = Property->ContainerPtrToValuePtr<uint8>(Struct);

		// convert the property to a FJsonValue
		TSharedPtr<FJsonValue> JsonValue = UPropertyToJsonValue(Property, Value, CheckFlags, SkipFlags, ExportCb);
		if (!JsonValue.IsValid())
		{
#if ENGINE_MAJOR_VERSION < 5 && ENGINE_MINOR_VERSION < 25
			UClass* PropClass = Property->GetClass();
#else
			FFieldClass* PropClass = Property->GetClass();
#endif
			UE_LOG(LogJson, Error, TEXT("UStructToJsonObject - Unhandled property type '%s': %s"), *PropClass->GetName(), *Property->GetPathName());
			return false;
		}

		// set the value on the output object
		OutJsonAttributes.Add(VariableName, JsonValue);
	}

	return true;
}

template<class CharType, class PrintPolicy>
bool UStructToJsonObjectStringInternal(const TSharedRef<FJsonObject>& JsonObject, FString& OutJsonString, int32 Indent)
{
	TSharedRef<TJsonWriter<CharType, PrintPolicy> > JsonWriter = TJsonWriterFactory<CharType, PrintPolicy>::Create(&OutJsonString, Indent);
	bool bSuccess = FJsonSerializer::Serialize(JsonObject, JsonWriter);
	JsonWriter->Close();
	return bSuccess;
}

bool FPragmaJsonObjectConverter::UStructToJsonObjectString(const UStruct* StructDefinition, const void* Struct, FString& OutJsonString, int64 CheckFlags, int64 SkipFlags, int32 Indent, const CustomExportCallback* ExportCb, bool bPrettyPrint)
{
	TSharedRef<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	if (UStructToJsonObject(StructDefinition, Struct, JsonObject, CheckFlags, SkipFlags, ExportCb))
	{
		bool bSuccess = false;
		if (bPrettyPrint)
		{
			bSuccess = UStructToJsonObjectStringInternal<TCHAR, TPrettyJsonPrintPolicy<TCHAR> >(JsonObject, OutJsonString, Indent);
		}
		else
		{
			bSuccess = UStructToJsonObjectStringInternal<TCHAR, TCondensedJsonPrintPolicy<TCHAR> >(JsonObject, OutJsonString, Indent);
		}
		if (bSuccess)
		{
			return true;
		}
		else
		{
			UE_LOG(LogJson, Warning, TEXT("UStructToJsonObjectString - Unable to write out json"));
		}
	}

	return false;
}

//static
bool FPragmaJsonObjectConverter::GetTextFromObject(const TSharedRef<FJsonObject>& Obj, FText& TextOut)
{
	// get the prioritized culture name list
	FCultureRef CurrentCulture = FInternationalization::Get().GetCurrentCulture();
	TArray<FString> CultureList = CurrentCulture->GetPrioritizedParentCultureNames();

	// try to follow the fall back chain that the engine uses
	FString TextString;
	for (const FString& CultureCode : CultureList)
	{
		if (Obj->TryGetStringField(CultureCode, TextString))
		{
			TextOut = FText::FromString(TextString);
			return true;
		}
	}

	// try again but only search on the locale region (in the localized data). This is a common omission (i.e. en-US source text should be used if no en is defined)
	for (const FString& LocaleToMatch : CultureList)
	{
		int32 SeparatorPos;
		// only consider base language entries in culture chain (i.e. "en")
		if (!LocaleToMatch.FindChar('-', SeparatorPos))
		{
			for (const auto& Pair : Obj->Values)
			{
				// only consider coupled entries now (base ones would have been matched on first path) (i.e. "en-US")
				if (Pair.Key.FindChar('-', SeparatorPos))
				{
					if (Pair.Key.StartsWith(LocaleToMatch))
					{
						TextOut = FText::FromString(Pair.Value->AsString());
						return true;
					}
				}
			}
		}
	}

	// no luck, is this possibly an unrelated json object?
	return false;
}

namespace
{
	bool JsonValueToFPropertyWithContainer(const TSharedPtr<FJsonValue>& JsonValue, FProperty* Property, void* OutValue, const UStruct* ContainerStruct, void* Container, int64 CheckFlags, int64 SkipFlags, const FPragmaJsonObjectConverter::CustomImportCallback* ImportCb);
	bool JsonAttributesToUStructWithContainer(const TMap< FString, TSharedPtr<FJsonValue> >& JsonAttributes, const UStruct* StructDefinition, void* OutStruct, const UStruct* ContainerStruct, void* Container, int64 CheckFlags, int64 SkipFlags, const FPragmaJsonObjectConverter::CustomImportCallback* ImportCb);

	/** Convert JSON to property, assuming either the property is not an array or the value is an individual array element */
	bool ConvertScalarJsonValueToFPropertyWithContainer(const TSharedPtr<FJsonValue>& JsonValue, FProperty* Property, void* OutValue, const UStruct* ContainerStruct, void* Container, int64 CheckFlags, int64 SkipFlags, const FPragmaJsonObjectConverter::CustomImportCallback* ImportCb)
	{
		/////////////// begin pragma modification ///////////////
		// See if there's a custom import callback first, so it can override default behavior
		if (ImportCb
			&& ImportCb->IsBound()
			&& ImportCb->Execute(JsonValue, Property, OutValue))
		{
			return true;
		}
		/////////////// begin pragma modification ///////////////

		if (FEnumProperty* EnumProperty = PRAGMA_CAST_PROPERTY<FEnumProperty>(Property))
		{
			if (JsonValue->Type == EJson::String)
			{
				// see if we were passed a string for the enum
				const UEnum* Enum = EnumProperty->GetEnum();
				check(Enum);
				FString StrValue = JsonValue->AsString();
				int64 IntValue = Enum->GetValueByName(FName(*StrValue));
				if (IntValue == INDEX_NONE)
				{
					UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Unable import enum %s from string value %s for property %s"), *Enum->CppType, *StrValue, *Property->GetNameCPP());
					return false;
				}
				EnumProperty->GetUnderlyingProperty()->SetIntPropertyValue(OutValue, IntValue);
			}
			else
			{
				// AsNumber will log an error for completely inappropriate types (then give us a default)
				EnumProperty->GetUnderlyingProperty()->SetIntPropertyValue(OutValue, (int64)JsonValue->AsNumber());
			}
		}
		else if (FNumericProperty *NumericProperty = PRAGMA_CAST_PROPERTY<FNumericProperty>(Property))
		{
			if (NumericProperty->IsEnum() && JsonValue->Type == EJson::String)
			{
				// see if we were passed a string for the enum
				const UEnum* Enum = NumericProperty->GetIntPropertyEnum();
				check(Enum); // should be assured by IsEnum()
				FString StrValue = JsonValue->AsString();
				int64 IntValue = Enum->GetValueByName(FName(*StrValue));
				if (IntValue == INDEX_NONE)
				{
					UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Unable import enum %s from string value %s for property %s"), *Enum->CppType, *StrValue, *Property->GetNameCPP());
					return false;
				}
				NumericProperty->SetIntPropertyValue(OutValue, IntValue);
			}
			else if (NumericProperty->IsFloatingPoint())
			{
				// AsNumber will log an error for completely inappropriate types (then give us a default)
				NumericProperty->SetFloatingPointPropertyValue(OutValue, JsonValue->AsNumber());
			}
			else if (NumericProperty->IsInteger())
			{
				if (JsonValue->Type == EJson::String)
				{
					// parse string -> int64 ourselves so we don't lose any precision going through AsNumber (aka double)
					NumericProperty->SetIntPropertyValue(OutValue, FCString::Atoi64(*JsonValue->AsString()));
				}
				else
				{
					// AsNumber will log an error for completely inappropriate types (then give us a default)
					NumericProperty->SetIntPropertyValue(OutValue, (int64)JsonValue->AsNumber());
				}
			}
			else
			{
				UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Unable to set numeric property type %s for property %s"), *Property->GetClass()->GetName(), *Property->GetNameCPP());
				return false;
			}
		}
		else if (FBoolProperty *BoolProperty = PRAGMA_CAST_PROPERTY<FBoolProperty>(Property))
		{
			// AsBool will log an error for completely inappropriate types (then give us a default)
			BoolProperty->SetPropertyValue(OutValue, JsonValue->AsBool());
		}
		else if (FStrProperty *StringProperty = PRAGMA_CAST_PROPERTY<FStrProperty>(Property))
		{
			// AsString will log an error for completely inappropriate types (then give us a default)
			StringProperty->SetPropertyValue(OutValue, JsonValue->AsString());
		}
		else if (FArrayProperty *ArrayProperty = PRAGMA_CAST_PROPERTY<FArrayProperty>(Property))
		{
			if (JsonValue->Type == EJson::Array)
			{
				TArray< TSharedPtr<FJsonValue> > ArrayValue = JsonValue->AsArray();
				int32 ArrLen = ArrayValue.Num();

				// make the output array size match
				FScriptArrayHelper Helper(ArrayProperty, OutValue);
				Helper.Resize(ArrLen);

				// set the property values
				for (int32 i = 0; i < ArrLen; ++i)
				{
					const TSharedPtr<FJsonValue>& ArrayValueItem = ArrayValue[i];
					if (ArrayValueItem.IsValid() && !ArrayValueItem->IsNull())
					{
						if (!JsonValueToFPropertyWithContainer(ArrayValueItem, ArrayProperty->Inner, Helper.GetRawPtr(i), ContainerStruct, Container, CheckFlags & (~CPF_ParmFlags), SkipFlags, ImportCb))
						{
							UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Unable to deserialize array element [%d] for property %s"), i, *Property->GetNameCPP());
							return false;
						}
					}
				}
			}
			else
			{
				UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Attempted to import TArray from non-array JSON key for property %s"), *Property->GetNameCPP());
				return false;
			}
		}
		else if (FMapProperty* MapProperty = PRAGMA_CAST_PROPERTY<FMapProperty>(Property))
		{
			if (JsonValue->Type == EJson::Object)
			{
				TSharedPtr<FJsonObject> ObjectValue = JsonValue->AsObject();

				FScriptMapHelper Helper(MapProperty, OutValue);

				check(ObjectValue);

				int32 MapSize = ObjectValue->Values.Num();
				Helper.EmptyValues(MapSize);

				// set the property values
				for (const auto& Entry : ObjectValue->Values)
				{
					if (Entry.Value.IsValid() && !Entry.Value->IsNull())
					{
						int32 NewIndex = Helper.AddDefaultValue_Invalid_NeedsRehash();

						TSharedPtr<FJsonValueString> TempKeyValue = MakeShared<FJsonValueString>(Entry.Key);

						const bool bKeySuccess = JsonValueToFPropertyWithContainer(TempKeyValue, MapProperty->KeyProp, Helper.GetKeyPtr(NewIndex), ContainerStruct, Container, CheckFlags & (~CPF_ParmFlags), SkipFlags, ImportCb);
						const bool bValueSuccess = JsonValueToFPropertyWithContainer(Entry.Value, MapProperty->ValueProp, Helper.GetValuePtr(NewIndex), ContainerStruct, Container, CheckFlags & (~CPF_ParmFlags), SkipFlags, ImportCb);

						if (!(bKeySuccess && bValueSuccess))
						{
							UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Unable to deserialize map element [key: %s] for property %s"), *Entry.Key, *Property->GetNameCPP());
							return false;
						}
					}
				}

				Helper.Rehash();
			}
			else
			{
				UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Attempted to import TMap from non-object JSON key for property %s"), *Property->GetNameCPP());
				return false;
			}
		}
		else if (FSetProperty* SetProperty = PRAGMA_CAST_PROPERTY<FSetProperty>(Property))
		{
			if (JsonValue->Type == EJson::Array)
			{
				TArray< TSharedPtr<FJsonValue> > ArrayValue = JsonValue->AsArray();
				int32 ArrLen = ArrayValue.Num();

				FScriptSetHelper Helper(SetProperty, OutValue);

				// set the property values
				for (int32 i = 0; i < ArrLen; ++i)
				{
					const TSharedPtr<FJsonValue>& ArrayValueItem = ArrayValue[i];
					if (ArrayValueItem.IsValid() && !ArrayValueItem->IsNull())
					{
						int32 NewIndex = Helper.AddDefaultValue_Invalid_NeedsRehash();
						if (!JsonValueToFPropertyWithContainer(ArrayValueItem, SetProperty->ElementProp, Helper.GetElementPtr(NewIndex), ContainerStruct, Container, CheckFlags & (~CPF_ParmFlags), SkipFlags, ImportCb))
						{
							UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Unable to deserialize set element [%d] for property %s"), i, *Property->GetNameCPP());
							return false;
						}
					}
				}

				Helper.Rehash();
			}
			else
			{
				UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Attempted to import TSet from non-array JSON key for property %s"), *Property->GetNameCPP());
				return false;
			}
		}
		else if (FTextProperty* TextProperty = PRAGMA_CAST_PROPERTY<FTextProperty>(Property))
		{
			if (JsonValue->Type == EJson::String)
			{
				// assume this string is already localized, so import as invariant
				TextProperty->SetPropertyValue(OutValue, FText::FromString(JsonValue->AsString()));
			}
			else if (JsonValue->Type == EJson::Object)
			{
				TSharedPtr<FJsonObject> Obj = JsonValue->AsObject();
				check(Obj.IsValid()); // should not fail if Type == EJson::Object

									  // import the subvalue as a culture invariant string
				FText Text;
				if (!FPragmaJsonObjectConverter::GetTextFromObject(Obj.ToSharedRef(), Text))
				{
					UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Attempted to import FText from JSON object with invalid keys for property %s"), *Property->GetNameCPP());
					return false;
				}
				TextProperty->SetPropertyValue(OutValue, Text);
			}
			else
			{
				UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Attempted to import FText from JSON that was neither string nor object for property %s"), *Property->GetNameCPP());
				return false;
			}
		}
		else if (FStructProperty *StructProperty = PRAGMA_CAST_PROPERTY<FStructProperty>(Property))
		{
			static const FName NAME_DateTime(TEXT("DateTime"));
			/////////////// begin pragma modification ///////////////
#if ENGINE_MAJOR_VERSION < 5
			// These two are declared globally in UE5.
			static const FName NAME_Color(TEXT("Color"));
			static const FName NAME_LinearColor(TEXT("LinearColor"));
#endif
			//////////////// end pragma modification ////////////////
			if (JsonValue->Type == EJson::Object)
			{
				TSharedPtr<FJsonObject> Obj = JsonValue->AsObject();
				check(Obj.IsValid()); // should not fail if Type == EJson::Object
				if (!JsonAttributesToUStructWithContainer(Obj->Values, StructProperty->Struct, OutValue, ContainerStruct, Container, CheckFlags & (~CPF_ParmFlags), SkipFlags, ImportCb))
				{
					UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - FPragmaJsonObjectConverter::JsonObjectToUStruct failed for property %s"), *Property->GetNameCPP());
					return false;
				}
				/////////////// begin pragma modification ///////////////
				PragmaRtti::FJsonAddonDeserializer AddonDeserialize;
				if (PragmaRtti::GetAddonDeserializer(StructProperty->Struct, AddonDeserialize))
				{
					if (!AddonDeserialize(Obj.ToSharedRef(), OutValue))
					{
						PRAGMA_LOG(Error, "Failed to deserialize addon data type %s to json using custom deserializer addon. There is probably a mismatch between SDK and"
                            " platform types. Most likely solution is you need to regenerate SDK types from the platform proto definitions.", *Property->GetNameCPP());
						// Fallthrough without addon data.
					}
				}
				//////////////// end pragma modification ////////////////
			}
			else if (JsonValue->Type == EJson::String && StructProperty->Struct->GetFName() == NAME_LinearColor)
			{
				FLinearColor& ColorOut = *(FLinearColor*)OutValue;
				FString ColorString = JsonValue->AsString();

				FColor IntermediateColor;
				IntermediateColor = FColor::FromHex(ColorString);

				ColorOut = IntermediateColor;
			}
			else if (JsonValue->Type == EJson::String && StructProperty->Struct->GetFName() == NAME_Color)
			{
				FColor& ColorOut = *(FColor*)OutValue;
				FString ColorString = JsonValue->AsString();

				ColorOut = FColor::FromHex(ColorString);
			}
			else if (JsonValue->Type == EJson::String && StructProperty->Struct->GetFName() == NAME_DateTime)
			{
				FString DateString = JsonValue->AsString();
				FDateTime& DateTimeOut = *(FDateTime*)OutValue;
				if (DateString == TEXT("min"))
				{
					// min representable value for our date struct. Actual date may vary by platform (this is used for sorting)
					DateTimeOut = FDateTime::MinValue();
				}
				else if (DateString == TEXT("max"))
				{
					// max representable value for our date struct. Actual date may vary by platform (this is used for sorting)
					DateTimeOut = FDateTime::MaxValue();
				}
				else if (DateString == TEXT("now"))
				{
					// this value's not really meaningful from json serialization (since we don't know timezone) but handle it anyway since we're handling the other keywords
					DateTimeOut = FDateTime::UtcNow();
				}
				else if (FDateTime::ParseIso8601(*DateString, DateTimeOut))
				{
					// ok
				}
				else if (FDateTime::Parse(DateString, DateTimeOut))
				{
					// ok
				}
				else
				{
					UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Unable to import FDateTime for property %s"), *Property->GetNameCPP());
					return false;
				}
			}
			else if (JsonValue->Type == EJson::String && StructProperty->Struct->GetCppStructOps() && StructProperty->Struct->GetCppStructOps()->HasImportTextItem())
			{
				UScriptStruct::ICppStructOps* TheCppStructOps = StructProperty->Struct->GetCppStructOps();

				FString ImportTextString = JsonValue->AsString();
				const TCHAR* ImportTextPtr = *ImportTextString;
				if (!TheCppStructOps->ImportTextItem(ImportTextPtr, OutValue, PPF_None, nullptr, (FOutputDevice*)GWarn))
				{
					// Fall back to trying the tagged property approach if custom ImportTextItem couldn't get it done
					ImportText(ImportTextPtr, Property, OutValue);
				}
			}
			else if (JsonValue->Type == EJson::String)
			{
				FString ImportTextString = JsonValue->AsString();
				const TCHAR* ImportTextPtr = *ImportTextString;
				ImportText(ImportTextPtr, Property, OutValue);
			}
			else
			{
				UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Attempted to import UStruct from non-object JSON key for property %s"), *Property->GetNameCPP());
				return false;
			}
		}
		else if (FObjectProperty *ObjectProperty = PRAGMA_CAST_PROPERTY<FObjectProperty>(Property))
		{
			if (JsonValue->Type == EJson::Object)
			{
				UObject* Outer = GetTransientPackage();
				if (ContainerStruct->IsChildOf(UObject::StaticClass()))
				{
					Outer = (UObject*)Container;
				}

				UClass* PropertyClass = ObjectProperty->PropertyClass;
				/////////////// begin pragma modification ///////////////
#if ENGINE_MAJOR_VERSION >= 5
				UObject* createdObj = StaticAllocateObject(PropertyClass, Outer, NAME_None, EObjectFlags::RF_NoFlags, EInternalObjectFlags::None, false);
				(*PropertyClass->ClassConstructor)(FObjectInitializer(createdObj, PropertyClass->ClassDefaultObject, EObjectInitializerOptions::None));
#else
				UObject* createdObj = StaticAllocateObject(PropertyClass, Outer, NAME_None, EObjectFlags::RF_NoFlags, EInternalObjectFlags::None, false);
				(*PropertyClass->ClassConstructor)(FObjectInitializer(createdObj, PropertyClass->ClassDefaultObject, false, false));
#endif
				//////////////// end pragma modification ////////////////


				ObjectProperty->SetObjectPropertyValue(OutValue, createdObj);

				TSharedPtr<FJsonObject> Obj = JsonValue->AsObject();
				check(Obj.IsValid()); // should not fail if Type == EJson::Object
				if (!JsonAttributesToUStructWithContainer(Obj->Values, ObjectProperty->PropertyClass, createdObj, ObjectProperty->PropertyClass, createdObj, CheckFlags & (~CPF_ParmFlags), SkipFlags, ImportCb))
				{
					UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - FPragmaJsonObjectConverter::JsonObjectToUStruct failed for property %s"), *Property->GetNameCPP());
					return false;
				}
			}
			else if (JsonValue->Type == EJson::String)
			{
				// Default to expect a string for everything else
				if (ImportText(*JsonValue->AsString(), Property, OutValue) == nullptr)
				{
					UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Unable import property type %s from string value for property %s"), *Property->GetClass()->GetName(), *Property->GetNameCPP());
					return false;
				}
			}
		}
		else
		{
			// Default to expect a string for everything else
			if (ImportText(*JsonValue->AsString(), Property, OutValue) == nullptr)
			{
				UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Unable import property type %s from string value for property %s"), *Property->GetClass()->GetName(), *Property->GetNameCPP());
				return false;
			}
		}

		return true;
	}


	bool JsonValueToFPropertyWithContainer(const TSharedPtr<FJsonValue>& JsonValue, FProperty* Property, void* OutValue, const UStruct* ContainerStruct, void* Container, int64 CheckFlags, int64 SkipFlags, const FPragmaJsonObjectConverter::CustomImportCallback* ImportCb)
	{
		if (!JsonValue.IsValid())
		{
			UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Invalid value JSON key"));
			return false;
		}

		bool bArrayOrSetProperty = Property->IsA<FArrayProperty>() || Property->IsA<FSetProperty>();
		bool bJsonArray = JsonValue->Type == EJson::Array;

		if (!bJsonArray)
		{
			if (bArrayOrSetProperty)
			{
				UE_LOG(LogJson, Error, TEXT("JsonValueToFProperty - Attempted to import TArray from non-array JSON key"));
				return false;
			}

			if (Property->ArrayDim != 1)
			{
				UE_LOG(LogJson, Warning, TEXT("Ignoring excess properties when deserializing %s"), *Property->GetName());
			}

			return ConvertScalarJsonValueToFPropertyWithContainer(JsonValue, Property, OutValue, ContainerStruct, Container, CheckFlags, SkipFlags, ImportCb);
		}

		// In practice, the ArrayDim == 1 check ought to be redundant, since nested arrays of FPropertys are not supported
		if (bArrayOrSetProperty && Property->ArrayDim == 1)
		{
			// Read into TArray
			return ConvertScalarJsonValueToFPropertyWithContainer(JsonValue, Property, OutValue, ContainerStruct, Container, CheckFlags, SkipFlags, ImportCb);
		}

		// We're deserializing a JSON array
		const auto& ArrayValue = JsonValue->AsArray();
		if (Property->ArrayDim < ArrayValue.Num())
		{
			UE_LOG(LogJson, Warning, TEXT("Ignoring excess properties when deserializing %s"), *Property->GetName());
		}

		// Read into native array
		int ItemsToRead = FMath::Clamp(ArrayValue.Num(), 0, Property->ArrayDim);
		for (int Index = 0; Index != ItemsToRead; ++Index)
		{
			if (!ConvertScalarJsonValueToFPropertyWithContainer(ArrayValue[Index], Property, (char*)OutValue + Index * Property->ElementSize, ContainerStruct, Container, CheckFlags, SkipFlags, ImportCb))
			{
				return false;
			}
		}
		return true;
	}

	bool JsonAttributesToUStructWithContainer(const TMap< FString, TSharedPtr<FJsonValue> >& JsonAttributes, const UStruct* StructDefinition, void* OutStruct, const UStruct* ContainerStruct, void* Container, int64 CheckFlags, int64 SkipFlags, const FPragmaJsonObjectConverter::CustomImportCallback* ImportCb)
	{
		if (StructDefinition == FJsonObjectWrapper::StaticStruct())
		{
			// Just copy it into the object
			FJsonObjectWrapper* ProxyObject = (FJsonObjectWrapper *)OutStruct;
			ProxyObject->JsonObject = MakeShared<FJsonObject>();
			ProxyObject->JsonObject->Values = JsonAttributes;
			return true;
		}

		int32 NumUnclaimedProperties = JsonAttributes.Num();
		if (NumUnclaimedProperties <= 0)
		{
			return true;
		}

		// iterate over the struct properties
		for (TFieldIterator<FProperty> PropIt(StructDefinition); PropIt; ++PropIt)
		{
			FProperty* Property = *PropIt;

			// Check to see if we should ignore this property
			if (CheckFlags != 0 && !Property->HasAnyPropertyFlags(CheckFlags))
			{
				continue;
			}
			if (Property->HasAnyPropertyFlags(SkipFlags))
			{
				continue;
			}

			// find a json value matching this property name
			const TSharedPtr<FJsonValue>* JsonValue = JsonAttributes.Find(Property->GetName());
			if (!JsonValue)
			{
				// we allow values to not be found since this mirrors the typical UObject mantra that all the fields are optional when deserializing
				continue;
			}

			if (JsonValue->IsValid() && !(*JsonValue)->IsNull())
			{
				void* Value = Property->ContainerPtrToValuePtr<uint8>(OutStruct);
				if (!JsonValueToFPropertyWithContainer(*JsonValue, Property, Value, ContainerStruct, Container, CheckFlags, SkipFlags, ImportCb))
				{
					UE_LOG(LogJson, Error, TEXT("JsonObjectToUStruct - Unable to parse %s.%s from JSON"), *StructDefinition->GetName(), *Property->GetName());
					return false;
				}
			}

			if (--NumUnclaimedProperties <= 0)
			{
				// If we found all properties that were in the JsonAttributes map, there is no reason to keep looking for more.
				break;
			}
		}

		return true;
	}

}

bool FPragmaJsonObjectConverter::JsonValueToUProperty(const TSharedPtr<FJsonValue>& JsonValue, FProperty* Property, void* OutValue, int64 CheckFlags, int64 SkipFlags, const CustomImportCallback* ImportCb)
{
	return JsonValueToFPropertyWithContainer(JsonValue, Property, OutValue, nullptr, nullptr, CheckFlags, SkipFlags, ImportCb);
}

bool FPragmaJsonObjectConverter::JsonObjectToUStruct(const TSharedRef<FJsonObject>& JsonObject, const UStruct* StructDefinition, void* OutStruct, int64 CheckFlags, int64 SkipFlags, const CustomImportCallback* ImportCb)
{
	return JsonAttributesToUStruct(JsonObject->Values, StructDefinition, OutStruct, CheckFlags, SkipFlags, ImportCb);
}

bool FPragmaJsonObjectConverter::JsonAttributesToUStruct(const TMap< FString, TSharedPtr<FJsonValue> >& JsonAttributes, const UStruct* StructDefinition, void* OutStruct, int64 CheckFlags, int64 SkipFlags, const CustomImportCallback* ImportCb)
{
	return JsonAttributesToUStructWithContainer(JsonAttributes, StructDefinition, OutStruct, StructDefinition, OutStruct, CheckFlags, SkipFlags, ImportCb);
}

FFormatNamedArguments FPragmaJsonObjectConverter::ParseTextArgumentsFromJson(const TSharedPtr<const FJsonObject>& JsonObject)
{
	FFormatNamedArguments NamedArgs;
	if (JsonObject.IsValid())
	{
		for (const auto& It : JsonObject->Values)
		{
			if (!It.Value.IsValid())
				continue;

			switch (It.Value->Type)
			{
			case EJson::Number:
				// number
				NamedArgs.Emplace(It.Key, It.Value->AsNumber());
				break;
			case EJson::String:
				// culture invariant string
				NamedArgs.Emplace(It.Key, FText::FromString(It.Value->AsString()));
				break;
			case EJson::Object:
			{
				// localized string
				FText TextOut;
				if (FPragmaJsonObjectConverter::GetTextFromObject(It.Value->AsObject().ToSharedRef(), TextOut))
				{
					NamedArgs.Emplace(It.Key, TextOut);
				}
				else
				{
					UE_LOG(LogJson, Error, TEXT("Unable to apply Json parameter %s (could not parse object)"), *It.Key);
				}
			}
			break;
			default:
				UE_LOG(LogJson, Error, TEXT("Unable to apply Json parameter %s (bad type)"), *It.Key);
				break;
			}
		}
	}
	return NamedArgs;
}
