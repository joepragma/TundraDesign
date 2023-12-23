#include "PragmaRtti.h"

#include "Tests/TestDto.h"

namespace PragmaRtti
{
bool GetSerializer(const UScriptStruct* Struct, FJsonSerializer& OutFn)
{
	return false;
}
bool GetDeserializer(const UScriptStruct* Struct, FJsonDeserializer& OutFn)
{
	return false;
}
bool GetAddonSerializer(const UScriptStruct* Struct, FJsonAddonSerializer& OutFn)
{
	if (PragmaGeneratedRtti::GetAddonSerializer(Struct, OutFn))
	{
		return true;
	}

	// Test structs.
	if (Struct == FPragma_TestOneOf::StaticStruct()) { OutFn = &FPragma_TestOneOf::AddonSerializer; return true; }
	if (Struct == FPragma_TestOneOfStruct::StaticStruct()) { OutFn = &FPragma_TestOneOfStruct::AddonSerializer; return true; }
	if (Struct == FPragma_TestOptional::StaticStruct()) { OutFn = &FPragma_TestOptional::AddonSerializer; return true; }
	return false;
}
bool GetAddonDeserializer(const UScriptStruct* Struct, FJsonAddonDeserializer& OutFn)
{
	if (PragmaGeneratedRtti::GetAddonDeserializer(Struct, OutFn))
	{
		return true;
	}

	// Test structs.
	if (Struct == FPragma_TestOneOf::StaticStruct()) { OutFn = &FPragma_TestOneOf::AddonDeserializer; return true; }
	if (Struct == FPragma_TestOneOfStruct::StaticStruct()) { OutFn = &FPragma_TestOneOfStruct::AddonDeserializer; return true; }
	if (Struct == FPragma_TestOptional::StaticStruct()) { OutFn = &FPragma_TestOptional::AddonDeserializer; return true; }
	return false;
}
}
