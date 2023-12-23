#pragma once

#include "CoreMinimal.h"

class FJsonObject;

namespace PragmaRtti
{
typedef TFunction<bool(const void* Value, TSharedRef<FJsonObject>& OutJsonObj)> FJsonSerializer;
typedef TFunction<bool(const TSharedRef<FJsonObject>& JsonObj, void* OutValue)> FJsonDeserializer;
typedef TFunction<bool(const void* Value, TSharedRef<FJsonObject>& OutJsonObj)> FJsonAddonSerializer;
typedef TFunction<bool(const TSharedRef<FJsonObject>& JsonObj, void* OutValue)> FJsonAddonDeserializer;

PRAGMASDK_API bool GetAddonSerializer(const UScriptStruct* Struct, FJsonAddonSerializer& OutFn);
PRAGMASDK_API bool GetAddonDeserializer(const UScriptStruct* Struct, FJsonAddonDeserializer& OutFn);
PRAGMASDK_API bool GetSerializer(const UScriptStruct* Struct, FJsonSerializer& OutFn);
PRAGMASDK_API bool GetDeserializer(const UScriptStruct* Struct, FJsonDeserializer& OutFn);
}

// These implementations are created via code gen.
namespace PragmaGeneratedRtti
{
PRAGMASDK_API bool GetAddonSerializer(const UScriptStruct* Struct, PragmaRtti::FJsonAddonSerializer& OutFn);
PRAGMASDK_API bool GetAddonDeserializer(const UScriptStruct* Struct, PragmaRtti::FJsonAddonDeserializer& OutFn);

PRAGMASDK_API const FString* GetFieldNameForSerialization(const UStruct* Struct, const FProperty* Property);
}

// These implementations are created via code gen.
namespace PragmaPlayerDataGeneratedRtti
{
PRAGMASDK_API bool GetAddonSerializer(const UScriptStruct* Struct, PragmaRtti::FJsonAddonSerializer& OutFn);
PRAGMASDK_API bool GetAddonDeserializer(const UScriptStruct* Struct, PragmaRtti::FJsonAddonDeserializer& OutFn);

PRAGMASDK_API const FString* GetFieldNameForSerialization(const UStruct* Struct, const FProperty* Property);
}