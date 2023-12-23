// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/GeneratedPlayerDataRtti.cpp.mustache
// </auto-generated>

#include "PragmaRtti.h"
#include "Dto/PragmaPlayerDataTypesDto.h"
#include "Dto/PragmaPlayerDataExtDto.h"

namespace PragmaPlayerDataGeneratedRtti
{
bool GetAddonSerializer(const UScriptStruct* Struct, PragmaRtti::FJsonAddonSerializer& OutFn)
{
	if (Struct == FPragma_PlayerData_PlayerDataResponseProto::StaticStruct()) { OutFn = &FPragma_PlayerData_PlayerDataResponseProto::AddonSerializer; return true; }
	if (Struct == FPragma_PlayerData_PlayerDataRequestProto::StaticStruct()) { OutFn = &FPragma_PlayerData_PlayerDataRequestProto::AddonSerializer; return true; }
	if (Struct == FPragma_PlayerData_ExtComponent::StaticStruct()) { OutFn = &FPragma_PlayerData_ExtComponent::AddonSerializer; return true; }
	return false;
}
bool GetAddonDeserializer(const UScriptStruct* Struct, PragmaRtti::FJsonAddonDeserializer& OutFn)
{
	if (Struct == FPragma_PlayerData_PlayerDataResponseProto::StaticStruct()) { OutFn = &FPragma_PlayerData_PlayerDataResponseProto::AddonDeserializer; return true; }
	if (Struct == FPragma_PlayerData_PlayerDataRequestProto::StaticStruct()) { OutFn = &FPragma_PlayerData_PlayerDataRequestProto::AddonDeserializer; return true; }
	if (Struct == FPragma_PlayerData_ExtComponent::StaticStruct()) { OutFn = &FPragma_PlayerData_ExtComponent::AddonDeserializer; return true; }
	return false;
}

// This exists to get around Unreal FName issues. Specifically, the Json serialization is based on the FProperty's name,
// which could be a different case (e.g. HostName vs Hostname) at runtime since FNames are case-insensitive. This strange
// function ensures that the specific field is serialized with the correct casing regardless of FName shenanigans.
const FString* GetFieldNameForSerialization(const UStruct* Struct, const FProperty* Property)
{
	static TMap<TPair<const UStruct*,FName>, FString> Mapping {
		{TPair<const UStruct*,FName>(FPragma_PlayerData_EchoResponseProto::StaticStruct(), FName("Message")), "Message"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_MychecklistResponseOneProto::StaticStruct(), FName("String")), "String"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_AddNumberTimesTwoResponseProto::StaticStruct(), FName("Total")), "Total"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_AddNumberResponseProto::StaticStruct(), FName("Total")), "Total"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_EchoListResponseProto::StaticStruct(), FName("ListOfStrings")), "ListOfStrings"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_MyChecklistRequestOneProto::StaticStruct(), FName("Int")), "Int"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_AddNumberTimesTwoRequestProto::StaticStruct(), FName("Number")), "Number"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_AddNumberRequestProto::StaticStruct(), FName("Number")), "Number"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_EchoListRequestProto::StaticStruct(), FName("ListOfStrings")), "ListOfStrings"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_EchoRequestProto::StaticStruct(), FName("Message")), "Message"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_MyComponentProto::StaticStruct(), FName("Num")), "Num"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_MyComponentProto::StaticStruct(), FName("Str")), "Str"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_WalletComponentProto::StaticStruct(), FName("Gold")), "Gold"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_StringComponentProto::StaticStruct(), FName("Value")), "Value"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_NumberDataProto::StaticStruct(), FName("Number")), "Number"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_DemoContainedTypeProto::StaticStruct(), FName("Number")), "Number"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_DemoContainedTypeProto::StaticStruct(), FName("Word")), "Word"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_DemoNestedTypeProto::StaticStruct(), FName("DemoList")), "DemoList"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_DemoNestedTypeProto::StaticStruct(), FName("DemoMap")), "DemoMap"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_DemoNestedTypeProto::StaticStruct(), FName("DemoMutableList")), "DemoMutableList"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_DemoNestedTypeProto::StaticStruct(), FName("DemoMutableMap")), "DemoMutableMap"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_DemoNestedTypeProto::StaticStruct(), FName("Uuid")), "Uuid"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_DemoComplexTypeProto::StaticStruct(), FName("DemoList")), "DemoList"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_DemoComplexTypeProto::StaticStruct(), FName("DemoListInt")), "DemoListInt"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_DemoComplexTypeProto::StaticStruct(), FName("DemoListString")), "DemoListString"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_DemoComplexTypeProto::StaticStruct(), FName("DemoMap")), "DemoMap"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_DemoComplexTypeProto::StaticStruct(), FName("DemoMutableList")), "DemoMutableList"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_DemoComplexTypeProto::StaticStruct(), FName("DemoMutableMap")), "DemoMutableMap"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_DemoComplexTypeProto::StaticStruct(), FName("NestedType")), "NestedType"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_DemoComplexTypeProto::StaticStruct(), FName("Uuid")), "Uuid"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_EchoComplexResponseProto::StaticStruct(), FName("DemoComplexType")), "DemoComplexType"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_PlayerDataResponseProto::StaticStruct(), FName("Payload")), "Payload"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_EchoComplexRequestProto::StaticStruct(), FName("DemoComplexType")), "DemoComplexType"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_PlayerDataRequestProto::StaticStruct(), FName("Payload")), "Payload"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_MyLiveDataComponentProto::StaticStruct(), FName("SomeNumber")), "SomeNumber"},
		{TPair<const UStruct*,FName>(FPragma_PlayerData_ExtComponent::StaticStruct(), FName("Component")), "Component"},
	};
	return Mapping.Find(TPair<const UStruct*,FName>{Struct, Property->GetFName()});
}
}