// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/Dto.cpp.mustache
// </auto-generated>

#include "PragmaPlayerDataExtDto.h"
#include "CoreMinimal.h"
#include "PragmaJson.h"


FPragma_PlayerData_ExtComponent_Component::FPragma_PlayerData_ExtComponent_Component() {}
FPragma_PlayerData_ExtComponent_Component::~FPragma_PlayerData_ExtComponent_Component()
{
	Reset();
}

FPragma_PlayerData_ExtComponent_Component::FPragma_PlayerData_ExtComponent_Component(const FPragma_PlayerData_ExtComponent_Component& Other)
{
	*this = Other;
}

bool FPragma_PlayerData_ExtComponent_Component::Equals(const FPragma_PlayerData_ExtComponent_Component& Other) const
{
    if (OneOfType() != Other.OneOfType())
    {
        return false;
    }
	switch (OneOfType())
	{
	case EPragma_PlayerData_ExtComponent_ComponentType::DemoComplexType:
		return DemoComplexType() == Other.DemoComplexType();
	case EPragma_PlayerData_ExtComponent_ComponentType::MyComponent:
		return MyComponent() == Other.MyComponent();
	case EPragma_PlayerData_ExtComponent_ComponentType::MyLiveDataComponent:
		return MyLiveDataComponent() == Other.MyLiveDataComponent();
	case EPragma_PlayerData_ExtComponent_ComponentType::NumberData:
		return NumberData() == Other.NumberData();
	case EPragma_PlayerData_ExtComponent_ComponentType::PlayerHiddenComponent:
		return PlayerHiddenComponent() == Other.PlayerHiddenComponent();
	case EPragma_PlayerData_ExtComponent_ComponentType::PlayerVisibleComponent:
		return PlayerVisibleComponent() == Other.PlayerVisibleComponent();
	case EPragma_PlayerData_ExtComponent_ComponentType::StringComponent:
		return StringComponent() == Other.StringComponent();
	case EPragma_PlayerData_ExtComponent_ComponentType::WalletComponent:
		return WalletComponent() == Other.WalletComponent();
	case EPragma_PlayerData_ExtComponent_ComponentType::Invalid:
        return true;
	default:
		return false;
	}
}

FPragma_PlayerData_ExtComponent_Component& FPragma_PlayerData_ExtComponent_Component::operator=(const FPragma_PlayerData_ExtComponent_Component& Other)
{
	Reset();
	Type = Other.OneOfType();
	switch (Other.OneOfType())
	{
	case EPragma_PlayerData_ExtComponent_ComponentType::DemoComplexType:
		new(Union.DemoComplexType.GetTypedPtr()) FPragma_PlayerData_DemoComplexTypeProto(Other.DemoComplexType());
		break;
	case EPragma_PlayerData_ExtComponent_ComponentType::MyComponent:
		new(Union.MyComponent.GetTypedPtr()) FPragma_PlayerData_MyComponentProto(Other.MyComponent());
		break;
	case EPragma_PlayerData_ExtComponent_ComponentType::MyLiveDataComponent:
		new(Union.MyLiveDataComponent.GetTypedPtr()) FPragma_PlayerData_MyLiveDataComponentProto(Other.MyLiveDataComponent());
		break;
	case EPragma_PlayerData_ExtComponent_ComponentType::NumberData:
		new(Union.NumberData.GetTypedPtr()) FPragma_PlayerData_NumberDataProto(Other.NumberData());
		break;
	case EPragma_PlayerData_ExtComponent_ComponentType::PlayerHiddenComponent:
		new(Union.PlayerHiddenComponent.GetTypedPtr()) FPragma_PlayerData_PlayerHiddenComponentProto(Other.PlayerHiddenComponent());
		break;
	case EPragma_PlayerData_ExtComponent_ComponentType::PlayerVisibleComponent:
		new(Union.PlayerVisibleComponent.GetTypedPtr()) FPragma_PlayerData_PlayerVisibleComponentProto(Other.PlayerVisibleComponent());
		break;
	case EPragma_PlayerData_ExtComponent_ComponentType::StringComponent:
		new(Union.StringComponent.GetTypedPtr()) FPragma_PlayerData_StringComponentProto(Other.StringComponent());
		break;
	case EPragma_PlayerData_ExtComponent_ComponentType::WalletComponent:
		new(Union.WalletComponent.GetTypedPtr()) FPragma_PlayerData_WalletComponentProto(Other.WalletComponent());
		break;
	case EPragma_PlayerData_ExtComponent_ComponentType::Invalid:
	default:
		break;
	}
	return *this;
}

void FPragma_PlayerData_ExtComponent_Component::Reset()
{
	switch (OneOfType())
	{
	case EPragma_PlayerData_ExtComponent_ComponentType::DemoComplexType:
		Union.DemoComplexType.GetTypedPtr()->~FPragma_PlayerData_DemoComplexTypeProto();
		break;
	case EPragma_PlayerData_ExtComponent_ComponentType::MyComponent:
		Union.MyComponent.GetTypedPtr()->~FPragma_PlayerData_MyComponentProto();
		break;
	case EPragma_PlayerData_ExtComponent_ComponentType::MyLiveDataComponent:
		Union.MyLiveDataComponent.GetTypedPtr()->~FPragma_PlayerData_MyLiveDataComponentProto();
		break;
	case EPragma_PlayerData_ExtComponent_ComponentType::NumberData:
		Union.NumberData.GetTypedPtr()->~FPragma_PlayerData_NumberDataProto();
		break;
	case EPragma_PlayerData_ExtComponent_ComponentType::PlayerHiddenComponent:
		Union.PlayerHiddenComponent.GetTypedPtr()->~FPragma_PlayerData_PlayerHiddenComponentProto();
		break;
	case EPragma_PlayerData_ExtComponent_ComponentType::PlayerVisibleComponent:
		Union.PlayerVisibleComponent.GetTypedPtr()->~FPragma_PlayerData_PlayerVisibleComponentProto();
		break;
	case EPragma_PlayerData_ExtComponent_ComponentType::StringComponent:
		Union.StringComponent.GetTypedPtr()->~FPragma_PlayerData_StringComponentProto();
		break;
	case EPragma_PlayerData_ExtComponent_ComponentType::WalletComponent:
		Union.WalletComponent.GetTypedPtr()->~FPragma_PlayerData_WalletComponentProto();
		break;
	case EPragma_PlayerData_ExtComponent_ComponentType::Invalid:
	default:
		break;
	}
	Type = EPragma_PlayerData_ExtComponent_ComponentType::Invalid;
}

const FPragma_PlayerData_DemoComplexTypeProto& FPragma_PlayerData_ExtComponent_Component::DemoComplexType() const {
	checkf(OneOfType() == EPragma_PlayerData_ExtComponent_ComponentType::DemoComplexType, TEXT("Failed to retrieve DemoComplexType from FPragma_PlayerData_ExtComponent_Component. Check the OneOfType() before accessing the value."));
	return *Union.DemoComplexType.GetTypedPtr();
}
const FPragma_PlayerData_MyComponentProto& FPragma_PlayerData_ExtComponent_Component::MyComponent() const {
	checkf(OneOfType() == EPragma_PlayerData_ExtComponent_ComponentType::MyComponent, TEXT("Failed to retrieve MyComponent from FPragma_PlayerData_ExtComponent_Component. Check the OneOfType() before accessing the value."));
	return *Union.MyComponent.GetTypedPtr();
}
const FPragma_PlayerData_MyLiveDataComponentProto& FPragma_PlayerData_ExtComponent_Component::MyLiveDataComponent() const {
	checkf(OneOfType() == EPragma_PlayerData_ExtComponent_ComponentType::MyLiveDataComponent, TEXT("Failed to retrieve MyLiveDataComponent from FPragma_PlayerData_ExtComponent_Component. Check the OneOfType() before accessing the value."));
	return *Union.MyLiveDataComponent.GetTypedPtr();
}
const FPragma_PlayerData_NumberDataProto& FPragma_PlayerData_ExtComponent_Component::NumberData() const {
	checkf(OneOfType() == EPragma_PlayerData_ExtComponent_ComponentType::NumberData, TEXT("Failed to retrieve NumberData from FPragma_PlayerData_ExtComponent_Component. Check the OneOfType() before accessing the value."));
	return *Union.NumberData.GetTypedPtr();
}
const FPragma_PlayerData_PlayerHiddenComponentProto& FPragma_PlayerData_ExtComponent_Component::PlayerHiddenComponent() const {
	checkf(OneOfType() == EPragma_PlayerData_ExtComponent_ComponentType::PlayerHiddenComponent, TEXT("Failed to retrieve PlayerHiddenComponent from FPragma_PlayerData_ExtComponent_Component. Check the OneOfType() before accessing the value."));
	return *Union.PlayerHiddenComponent.GetTypedPtr();
}
const FPragma_PlayerData_PlayerVisibleComponentProto& FPragma_PlayerData_ExtComponent_Component::PlayerVisibleComponent() const {
	checkf(OneOfType() == EPragma_PlayerData_ExtComponent_ComponentType::PlayerVisibleComponent, TEXT("Failed to retrieve PlayerVisibleComponent from FPragma_PlayerData_ExtComponent_Component. Check the OneOfType() before accessing the value."));
	return *Union.PlayerVisibleComponent.GetTypedPtr();
}
const FPragma_PlayerData_StringComponentProto& FPragma_PlayerData_ExtComponent_Component::StringComponent() const {
	checkf(OneOfType() == EPragma_PlayerData_ExtComponent_ComponentType::StringComponent, TEXT("Failed to retrieve StringComponent from FPragma_PlayerData_ExtComponent_Component. Check the OneOfType() before accessing the value."));
	return *Union.StringComponent.GetTypedPtr();
}
const FPragma_PlayerData_WalletComponentProto& FPragma_PlayerData_ExtComponent_Component::WalletComponent() const {
	checkf(OneOfType() == EPragma_PlayerData_ExtComponent_ComponentType::WalletComponent, TEXT("Failed to retrieve WalletComponent from FPragma_PlayerData_ExtComponent_Component. Check the OneOfType() before accessing the value."));
	return *Union.WalletComponent.GetTypedPtr();
}

void FPragma_PlayerData_ExtComponent_Component::SetDemoComplexType(const FPragma_PlayerData_DemoComplexTypeProto& Value)
{
	Reset();
	Type = EPragma_PlayerData_ExtComponent_ComponentType::DemoComplexType;
	new(Union.DemoComplexType.GetTypedPtr()) FPragma_PlayerData_DemoComplexTypeProto(Value);
}
void FPragma_PlayerData_ExtComponent_Component::SetMyComponent(const FPragma_PlayerData_MyComponentProto& Value)
{
	Reset();
	Type = EPragma_PlayerData_ExtComponent_ComponentType::MyComponent;
	new(Union.MyComponent.GetTypedPtr()) FPragma_PlayerData_MyComponentProto(Value);
}
void FPragma_PlayerData_ExtComponent_Component::SetMyLiveDataComponent(const FPragma_PlayerData_MyLiveDataComponentProto& Value)
{
	Reset();
	Type = EPragma_PlayerData_ExtComponent_ComponentType::MyLiveDataComponent;
	new(Union.MyLiveDataComponent.GetTypedPtr()) FPragma_PlayerData_MyLiveDataComponentProto(Value);
}
void FPragma_PlayerData_ExtComponent_Component::SetNumberData(const FPragma_PlayerData_NumberDataProto& Value)
{
	Reset();
	Type = EPragma_PlayerData_ExtComponent_ComponentType::NumberData;
	new(Union.NumberData.GetTypedPtr()) FPragma_PlayerData_NumberDataProto(Value);
}
void FPragma_PlayerData_ExtComponent_Component::SetPlayerHiddenComponent(const FPragma_PlayerData_PlayerHiddenComponentProto& Value)
{
	Reset();
	Type = EPragma_PlayerData_ExtComponent_ComponentType::PlayerHiddenComponent;
	new(Union.PlayerHiddenComponent.GetTypedPtr()) FPragma_PlayerData_PlayerHiddenComponentProto(Value);
}
void FPragma_PlayerData_ExtComponent_Component::SetPlayerVisibleComponent(const FPragma_PlayerData_PlayerVisibleComponentProto& Value)
{
	Reset();
	Type = EPragma_PlayerData_ExtComponent_ComponentType::PlayerVisibleComponent;
	new(Union.PlayerVisibleComponent.GetTypedPtr()) FPragma_PlayerData_PlayerVisibleComponentProto(Value);
}
void FPragma_PlayerData_ExtComponent_Component::SetStringComponent(const FPragma_PlayerData_StringComponentProto& Value)
{
	Reset();
	Type = EPragma_PlayerData_ExtComponent_ComponentType::StringComponent;
	new(Union.StringComponent.GetTypedPtr()) FPragma_PlayerData_StringComponentProto(Value);
}
void FPragma_PlayerData_ExtComponent_Component::SetWalletComponent(const FPragma_PlayerData_WalletComponentProto& Value)
{
	Reset();
	Type = EPragma_PlayerData_ExtComponent_ComponentType::WalletComponent;
	new(Union.WalletComponent.GetTypedPtr()) FPragma_PlayerData_WalletComponentProto(Value);
}

bool FPragma_PlayerData_ExtComponent_Component::SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj)
{
	const FPragma_PlayerData_ExtComponent_Component& OneOf = static_cast<const FPragma_PlayerData_ExtComponent*>(Value)->Component;
	switch (OneOf.OneOfType())
	{
		case EPragma_PlayerData_ExtComponent_ComponentType::DemoComplexType:
			OutJsonObj->SetField("demoComplexType", PragmaJson::StructToJsonValue(*OneOf.Union.DemoComplexType.GetTypedPtr()));
			break;
		case EPragma_PlayerData_ExtComponent_ComponentType::MyComponent:
			OutJsonObj->SetField("myComponent", PragmaJson::StructToJsonValue(*OneOf.Union.MyComponent.GetTypedPtr()));
			break;
		case EPragma_PlayerData_ExtComponent_ComponentType::MyLiveDataComponent:
			OutJsonObj->SetField("myLiveDataComponent", PragmaJson::StructToJsonValue(*OneOf.Union.MyLiveDataComponent.GetTypedPtr()));
			break;
		case EPragma_PlayerData_ExtComponent_ComponentType::NumberData:
			OutJsonObj->SetField("numberData", PragmaJson::StructToJsonValue(*OneOf.Union.NumberData.GetTypedPtr()));
			break;
		case EPragma_PlayerData_ExtComponent_ComponentType::PlayerHiddenComponent:
			OutJsonObj->SetField("playerHiddenComponent", PragmaJson::StructToJsonValue(*OneOf.Union.PlayerHiddenComponent.GetTypedPtr()));
			break;
		case EPragma_PlayerData_ExtComponent_ComponentType::PlayerVisibleComponent:
			OutJsonObj->SetField("playerVisibleComponent", PragmaJson::StructToJsonValue(*OneOf.Union.PlayerVisibleComponent.GetTypedPtr()));
			break;
		case EPragma_PlayerData_ExtComponent_ComponentType::StringComponent:
			OutJsonObj->SetField("stringComponent", PragmaJson::StructToJsonValue(*OneOf.Union.StringComponent.GetTypedPtr()));
			break;
		case EPragma_PlayerData_ExtComponent_ComponentType::WalletComponent:
			OutJsonObj->SetField("walletComponent", PragmaJson::StructToJsonValue(*OneOf.Union.WalletComponent.GetTypedPtr()));
			break;
		default:
			break;
	}
	// Per protobuf spec, it's ok to have empty oneof.
	return true;
}
bool FPragma_PlayerData_ExtComponent_Component::DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue)
{
	FPragma_PlayerData_ExtComponent_Component& OutOneOf = static_cast<FPragma_PlayerData_ExtComponent*>(OutValue)->Component;
	if (JsonObj->HasField("demoComplexType"))
	{
		OutOneOf.Type = EPragma_PlayerData_ExtComponent_ComponentType::DemoComplexType;
		new(OutOneOf.Union.DemoComplexType.GetTypedPtr()) FPragma_PlayerData_DemoComplexTypeProto(PragmaJson::JsonValueToStruct<FPragma_PlayerData_DemoComplexTypeProto>(JsonObj->TryGetField(TEXT("demoComplexType")).ToSharedRef()));
		return true; // return to short circuit any other checks.
	}
	if (JsonObj->HasField("myComponent"))
	{
		OutOneOf.Type = EPragma_PlayerData_ExtComponent_ComponentType::MyComponent;
		new(OutOneOf.Union.MyComponent.GetTypedPtr()) FPragma_PlayerData_MyComponentProto(PragmaJson::JsonValueToStruct<FPragma_PlayerData_MyComponentProto>(JsonObj->TryGetField(TEXT("myComponent")).ToSharedRef()));
		return true; // return to short circuit any other checks.
	}
	if (JsonObj->HasField("myLiveDataComponent"))
	{
		OutOneOf.Type = EPragma_PlayerData_ExtComponent_ComponentType::MyLiveDataComponent;
		new(OutOneOf.Union.MyLiveDataComponent.GetTypedPtr()) FPragma_PlayerData_MyLiveDataComponentProto(PragmaJson::JsonValueToStruct<FPragma_PlayerData_MyLiveDataComponentProto>(JsonObj->TryGetField(TEXT("myLiveDataComponent")).ToSharedRef()));
		return true; // return to short circuit any other checks.
	}
	if (JsonObj->HasField("numberData"))
	{
		OutOneOf.Type = EPragma_PlayerData_ExtComponent_ComponentType::NumberData;
		new(OutOneOf.Union.NumberData.GetTypedPtr()) FPragma_PlayerData_NumberDataProto(PragmaJson::JsonValueToStruct<FPragma_PlayerData_NumberDataProto>(JsonObj->TryGetField(TEXT("numberData")).ToSharedRef()));
		return true; // return to short circuit any other checks.
	}
	if (JsonObj->HasField("playerHiddenComponent"))
	{
		OutOneOf.Type = EPragma_PlayerData_ExtComponent_ComponentType::PlayerHiddenComponent;
		new(OutOneOf.Union.PlayerHiddenComponent.GetTypedPtr()) FPragma_PlayerData_PlayerHiddenComponentProto(PragmaJson::JsonValueToStruct<FPragma_PlayerData_PlayerHiddenComponentProto>(JsonObj->TryGetField(TEXT("playerHiddenComponent")).ToSharedRef()));
		return true; // return to short circuit any other checks.
	}
	if (JsonObj->HasField("playerVisibleComponent"))
	{
		OutOneOf.Type = EPragma_PlayerData_ExtComponent_ComponentType::PlayerVisibleComponent;
		new(OutOneOf.Union.PlayerVisibleComponent.GetTypedPtr()) FPragma_PlayerData_PlayerVisibleComponentProto(PragmaJson::JsonValueToStruct<FPragma_PlayerData_PlayerVisibleComponentProto>(JsonObj->TryGetField(TEXT("playerVisibleComponent")).ToSharedRef()));
		return true; // return to short circuit any other checks.
	}
	if (JsonObj->HasField("stringComponent"))
	{
		OutOneOf.Type = EPragma_PlayerData_ExtComponent_ComponentType::StringComponent;
		new(OutOneOf.Union.StringComponent.GetTypedPtr()) FPragma_PlayerData_StringComponentProto(PragmaJson::JsonValueToStruct<FPragma_PlayerData_StringComponentProto>(JsonObj->TryGetField(TEXT("stringComponent")).ToSharedRef()));
		return true; // return to short circuit any other checks.
	}
	if (JsonObj->HasField("walletComponent"))
	{
		OutOneOf.Type = EPragma_PlayerData_ExtComponent_ComponentType::WalletComponent;
		new(OutOneOf.Union.WalletComponent.GetTypedPtr()) FPragma_PlayerData_WalletComponentProto(PragmaJson::JsonValueToStruct<FPragma_PlayerData_WalletComponentProto>(JsonObj->TryGetField(TEXT("walletComponent")).ToSharedRef()));
		return true; // return to short circuit any other checks.
	}
	// Per protobuf spec, oneofs have _at most_ one of the fields. None set is valid, so we always return true.
	return true;
}

bool operator==(const FPragma_PlayerData_ExtComponent_Component& Lhs, const FPragma_PlayerData_ExtComponent_Component& Rhs)
{
	return Lhs.Equals(Rhs);
}
bool operator!=(const FPragma_PlayerData_ExtComponent_Component& Lhs, const FPragma_PlayerData_ExtComponent_Component& Rhs)
{
	return !(Lhs == Rhs);
}


bool FPragma_PlayerData_ExtComponent::AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj)
{
	bool Success = true;
	Success &= FPragma_PlayerData_ExtComponent_Component::SerializeIntoParentJson(Value, OutJsonObj);
	return Success;
}

bool FPragma_PlayerData_ExtComponent::AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue)
{
	bool Success = true;
	Success &= FPragma_PlayerData_ExtComponent_Component::DeserializeFromParentJson(JsonObj, OutValue);
	return Success;
}

bool operator==(const FPragma_PlayerData_ExtComponent& Lhs, const FPragma_PlayerData_ExtComponent& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Component == Rhs.Component
;
}
bool operator!=(const FPragma_PlayerData_ExtComponent& Lhs, const FPragma_PlayerData_ExtComponent& Rhs)
{
	return !(Lhs == Rhs);
}
