// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/Dto.cpp.mustache
// </auto-generated>

#include "PragmaInventoryCommonDto.h"
#include "CoreMinimal.h"
#include "PragmaJson.h"



bool operator==(const FPragma_Inventory_PurchaseRequirements& Lhs, const FPragma_Inventory_PurchaseRequirements& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Ext == Rhs.Ext
			&& Lhs.RequiredCatalogIds == Rhs.RequiredCatalogIds
;
}
bool operator!=(const FPragma_Inventory_PurchaseRequirements& Lhs, const FPragma_Inventory_PurchaseRequirements& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Inventory_CostEntry& Lhs, const FPragma_Inventory_CostEntry& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Cost == Rhs.Cost
			&& Lhs.Tags == Rhs.Tags
;
}
bool operator!=(const FPragma_Inventory_CostEntry& Lhs, const FPragma_Inventory_CostEntry& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Inventory_AnnotatedStoreEntry& Lhs, const FPragma_Inventory_AnnotatedStoreEntry& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Id == Rhs.Id
			&& Lhs.ReceivedQuantityByCatalogId.OrderIndependentCompareEqual(Rhs.ReceivedQuantityByCatalogId)
			&& Lhs.CostByCatalogId.OrderIndependentCompareEqual(Rhs.CostByCatalogId)
			&& Lhs.Tags == Rhs.Tags
			&& Lhs.Requirements == Rhs.Requirements
			&& Lhs.CanPurchase == Rhs.CanPurchase
;
}
bool operator!=(const FPragma_Inventory_AnnotatedStoreEntry& Lhs, const FPragma_Inventory_AnnotatedStoreEntry& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Inventory_AnnotatedStore& Lhs, const FPragma_Inventory_AnnotatedStore& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Id == Rhs.Id
			&& Lhs.Name == Rhs.Name
			&& Lhs.StoreEntries == Rhs.StoreEntries
;
}
bool operator!=(const FPragma_Inventory_AnnotatedStore& Lhs, const FPragma_Inventory_AnnotatedStore& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Inventory_RewardGrant& Lhs, const FPragma_Inventory_RewardGrant& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.TableId == Rhs.TableId
			&& Lhs.Count == Rhs.Count
			&& Lhs.Tags == Rhs.Tags
;
}
bool operator!=(const FPragma_Inventory_RewardGrant& Lhs, const FPragma_Inventory_RewardGrant& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Inventory_ServerInstancedItemUpdate& Lhs, const FPragma_Inventory_ServerInstancedItemUpdate& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.RequestExt == Rhs.RequestExt
			&& Lhs.InstanceId == Rhs.InstanceId
			&& Lhs.UpdateEntryId == Rhs.UpdateEntryId
			&& Lhs.Tags == Rhs.Tags
;
}
bool operator!=(const FPragma_Inventory_ServerInstancedItemUpdate& Lhs, const FPragma_Inventory_ServerInstancedItemUpdate& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Inventory_StackableItemUpdate& Lhs, const FPragma_Inventory_StackableItemUpdate& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.CatalogId == Rhs.CatalogId
			&& Lhs.InstanceId == Rhs.InstanceId
			&& Lhs.AmountChanged == Rhs.AmountChanged
			&& Lhs.Tags == Rhs.Tags
;
}
bool operator!=(const FPragma_Inventory_StackableItemUpdate& Lhs, const FPragma_Inventory_StackableItemUpdate& Rhs)
{
	return !(Lhs == Rhs);
}

FPragma_Inventory_ServerItemUpdateV2_Item::FPragma_Inventory_ServerItemUpdateV2_Item() {}
FPragma_Inventory_ServerItemUpdateV2_Item::~FPragma_Inventory_ServerItemUpdateV2_Item()
{
	Reset();
}

FPragma_Inventory_ServerItemUpdateV2_Item::FPragma_Inventory_ServerItemUpdateV2_Item(const FPragma_Inventory_ServerItemUpdateV2_Item& Other)
{
	*this = Other;
}

bool FPragma_Inventory_ServerItemUpdateV2_Item::Equals(const FPragma_Inventory_ServerItemUpdateV2_Item& Other) const
{
    if (OneOfType() != Other.OneOfType())
    {
        return false;
    }
	switch (OneOfType())
	{
	case EPragma_Inventory_ServerItemUpdateV2_ItemType::Instanced:
		return Instanced() == Other.Instanced();
	case EPragma_Inventory_ServerItemUpdateV2_ItemType::Stackable:
		return Stackable() == Other.Stackable();
	case EPragma_Inventory_ServerItemUpdateV2_ItemType::Invalid:
        return true;
	default:
		return false;
	}
}

FPragma_Inventory_ServerItemUpdateV2_Item& FPragma_Inventory_ServerItemUpdateV2_Item::operator=(const FPragma_Inventory_ServerItemUpdateV2_Item& Other)
{
	Reset();
	Type = Other.OneOfType();
	switch (Other.OneOfType())
	{
	case EPragma_Inventory_ServerItemUpdateV2_ItemType::Instanced:
		new(Union.Instanced.GetTypedPtr()) FPragma_Inventory_ServerInstancedItemUpdate(Other.Instanced());
		break;
	case EPragma_Inventory_ServerItemUpdateV2_ItemType::Stackable:
		new(Union.Stackable.GetTypedPtr()) FPragma_Inventory_StackableItemUpdate(Other.Stackable());
		break;
	case EPragma_Inventory_ServerItemUpdateV2_ItemType::Invalid:
	default:
		break;
	}
	return *this;
}

void FPragma_Inventory_ServerItemUpdateV2_Item::Reset()
{
	switch (OneOfType())
	{
	case EPragma_Inventory_ServerItemUpdateV2_ItemType::Instanced:
		Union.Instanced.GetTypedPtr()->~FPragma_Inventory_ServerInstancedItemUpdate();
		break;
	case EPragma_Inventory_ServerItemUpdateV2_ItemType::Stackable:
		Union.Stackable.GetTypedPtr()->~FPragma_Inventory_StackableItemUpdate();
		break;
	case EPragma_Inventory_ServerItemUpdateV2_ItemType::Invalid:
	default:
		break;
	}
	Type = EPragma_Inventory_ServerItemUpdateV2_ItemType::Invalid;
}

const FPragma_Inventory_ServerInstancedItemUpdate& FPragma_Inventory_ServerItemUpdateV2_Item::Instanced() const {
	checkf(OneOfType() == EPragma_Inventory_ServerItemUpdateV2_ItemType::Instanced, TEXT("Failed to retrieve Instanced from FPragma_Inventory_ServerItemUpdateV2_Item. Check the OneOfType() before accessing the value."));
	return *Union.Instanced.GetTypedPtr();
}
const FPragma_Inventory_StackableItemUpdate& FPragma_Inventory_ServerItemUpdateV2_Item::Stackable() const {
	checkf(OneOfType() == EPragma_Inventory_ServerItemUpdateV2_ItemType::Stackable, TEXT("Failed to retrieve Stackable from FPragma_Inventory_ServerItemUpdateV2_Item. Check the OneOfType() before accessing the value."));
	return *Union.Stackable.GetTypedPtr();
}

void FPragma_Inventory_ServerItemUpdateV2_Item::SetInstanced(const FPragma_Inventory_ServerInstancedItemUpdate& Value)
{
	Reset();
	Type = EPragma_Inventory_ServerItemUpdateV2_ItemType::Instanced;
	new(Union.Instanced.GetTypedPtr()) FPragma_Inventory_ServerInstancedItemUpdate(Value);
}
void FPragma_Inventory_ServerItemUpdateV2_Item::SetStackable(const FPragma_Inventory_StackableItemUpdate& Value)
{
	Reset();
	Type = EPragma_Inventory_ServerItemUpdateV2_ItemType::Stackable;
	new(Union.Stackable.GetTypedPtr()) FPragma_Inventory_StackableItemUpdate(Value);
}

bool FPragma_Inventory_ServerItemUpdateV2_Item::SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj)
{
	const FPragma_Inventory_ServerItemUpdateV2_Item& OneOf = static_cast<const FPragma_Inventory_ServerItemUpdateV2*>(Value)->Item;
	switch (OneOf.OneOfType())
	{
		case EPragma_Inventory_ServerItemUpdateV2_ItemType::Instanced:
			OutJsonObj->SetField("instanced", PragmaJson::StructToJsonValue(*OneOf.Union.Instanced.GetTypedPtr()));
			break;
		case EPragma_Inventory_ServerItemUpdateV2_ItemType::Stackable:
			OutJsonObj->SetField("stackable", PragmaJson::StructToJsonValue(*OneOf.Union.Stackable.GetTypedPtr()));
			break;
		default:
			break;
	}
	// Per protobuf spec, it's ok to have empty oneof.
	return true;
}
bool FPragma_Inventory_ServerItemUpdateV2_Item::DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue)
{
	FPragma_Inventory_ServerItemUpdateV2_Item& OutOneOf = static_cast<FPragma_Inventory_ServerItemUpdateV2*>(OutValue)->Item;
	if (JsonObj->HasField("instanced"))
	{
		OutOneOf.Type = EPragma_Inventory_ServerItemUpdateV2_ItemType::Instanced;
		new(OutOneOf.Union.Instanced.GetTypedPtr()) FPragma_Inventory_ServerInstancedItemUpdate(PragmaJson::JsonValueToStruct<FPragma_Inventory_ServerInstancedItemUpdate>(JsonObj->TryGetField(TEXT("instanced")).ToSharedRef()));
		return true; // return to short circuit any other checks.
	}
	if (JsonObj->HasField("stackable"))
	{
		OutOneOf.Type = EPragma_Inventory_ServerItemUpdateV2_ItemType::Stackable;
		new(OutOneOf.Union.Stackable.GetTypedPtr()) FPragma_Inventory_StackableItemUpdate(PragmaJson::JsonValueToStruct<FPragma_Inventory_StackableItemUpdate>(JsonObj->TryGetField(TEXT("stackable")).ToSharedRef()));
		return true; // return to short circuit any other checks.
	}
	// Per protobuf spec, oneofs have _at most_ one of the fields. None set is valid, so we always return true.
	return true;
}

bool operator==(const FPragma_Inventory_ServerItemUpdateV2_Item& Lhs, const FPragma_Inventory_ServerItemUpdateV2_Item& Rhs)
{
	return Lhs.Equals(Rhs);
}
bool operator!=(const FPragma_Inventory_ServerItemUpdateV2_Item& Lhs, const FPragma_Inventory_ServerItemUpdateV2_Item& Rhs)
{
	return !(Lhs == Rhs);
}


bool FPragma_Inventory_ServerItemUpdateV2::AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj)
{
	bool Success = true;
	Success &= FPragma_Inventory_ServerItemUpdateV2_Item::SerializeIntoParentJson(Value, OutJsonObj);
	return Success;
}

bool FPragma_Inventory_ServerItemUpdateV2::AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue)
{
	bool Success = true;
	Success &= FPragma_Inventory_ServerItemUpdateV2_Item::DeserializeFromParentJson(JsonObj, OutValue);
	return Success;
}

bool operator==(const FPragma_Inventory_ServerItemUpdateV2& Lhs, const FPragma_Inventory_ServerItemUpdateV2& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Item == Rhs.Item
;
}
bool operator!=(const FPragma_Inventory_ServerItemUpdateV2& Lhs, const FPragma_Inventory_ServerItemUpdateV2& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Inventory_InstancedItemUpdate& Lhs, const FPragma_Inventory_InstancedItemUpdate& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Ext == Rhs.Ext
			&& Lhs.InstanceId == Rhs.InstanceId
			&& Lhs.UpdateEntryId == Rhs.UpdateEntryId
			&& Lhs.Tags == Rhs.Tags
;
}
bool operator!=(const FPragma_Inventory_InstancedItemUpdate& Lhs, const FPragma_Inventory_InstancedItemUpdate& Rhs)
{
	return !(Lhs == Rhs);
}

FPragma_Inventory_ItemUpdateV2_Item::FPragma_Inventory_ItemUpdateV2_Item() {}
FPragma_Inventory_ItemUpdateV2_Item::~FPragma_Inventory_ItemUpdateV2_Item()
{
	Reset();
}

FPragma_Inventory_ItemUpdateV2_Item::FPragma_Inventory_ItemUpdateV2_Item(const FPragma_Inventory_ItemUpdateV2_Item& Other)
{
	*this = Other;
}

bool FPragma_Inventory_ItemUpdateV2_Item::Equals(const FPragma_Inventory_ItemUpdateV2_Item& Other) const
{
    if (OneOfType() != Other.OneOfType())
    {
        return false;
    }
	switch (OneOfType())
	{
	case EPragma_Inventory_ItemUpdateV2_ItemType::Instanced:
		return Instanced() == Other.Instanced();
	case EPragma_Inventory_ItemUpdateV2_ItemType::Stackable:
		return Stackable() == Other.Stackable();
	case EPragma_Inventory_ItemUpdateV2_ItemType::Invalid:
        return true;
	default:
		return false;
	}
}

FPragma_Inventory_ItemUpdateV2_Item& FPragma_Inventory_ItemUpdateV2_Item::operator=(const FPragma_Inventory_ItemUpdateV2_Item& Other)
{
	Reset();
	Type = Other.OneOfType();
	switch (Other.OneOfType())
	{
	case EPragma_Inventory_ItemUpdateV2_ItemType::Instanced:
		new(Union.Instanced.GetTypedPtr()) FPragma_Inventory_InstancedItemUpdate(Other.Instanced());
		break;
	case EPragma_Inventory_ItemUpdateV2_ItemType::Stackable:
		new(Union.Stackable.GetTypedPtr()) FPragma_Inventory_StackableItemUpdate(Other.Stackable());
		break;
	case EPragma_Inventory_ItemUpdateV2_ItemType::Invalid:
	default:
		break;
	}
	return *this;
}

void FPragma_Inventory_ItemUpdateV2_Item::Reset()
{
	switch (OneOfType())
	{
	case EPragma_Inventory_ItemUpdateV2_ItemType::Instanced:
		Union.Instanced.GetTypedPtr()->~FPragma_Inventory_InstancedItemUpdate();
		break;
	case EPragma_Inventory_ItemUpdateV2_ItemType::Stackable:
		Union.Stackable.GetTypedPtr()->~FPragma_Inventory_StackableItemUpdate();
		break;
	case EPragma_Inventory_ItemUpdateV2_ItemType::Invalid:
	default:
		break;
	}
	Type = EPragma_Inventory_ItemUpdateV2_ItemType::Invalid;
}

const FPragma_Inventory_InstancedItemUpdate& FPragma_Inventory_ItemUpdateV2_Item::Instanced() const {
	checkf(OneOfType() == EPragma_Inventory_ItemUpdateV2_ItemType::Instanced, TEXT("Failed to retrieve Instanced from FPragma_Inventory_ItemUpdateV2_Item. Check the OneOfType() before accessing the value."));
	return *Union.Instanced.GetTypedPtr();
}
const FPragma_Inventory_StackableItemUpdate& FPragma_Inventory_ItemUpdateV2_Item::Stackable() const {
	checkf(OneOfType() == EPragma_Inventory_ItemUpdateV2_ItemType::Stackable, TEXT("Failed to retrieve Stackable from FPragma_Inventory_ItemUpdateV2_Item. Check the OneOfType() before accessing the value."));
	return *Union.Stackable.GetTypedPtr();
}

void FPragma_Inventory_ItemUpdateV2_Item::SetInstanced(const FPragma_Inventory_InstancedItemUpdate& Value)
{
	Reset();
	Type = EPragma_Inventory_ItemUpdateV2_ItemType::Instanced;
	new(Union.Instanced.GetTypedPtr()) FPragma_Inventory_InstancedItemUpdate(Value);
}
void FPragma_Inventory_ItemUpdateV2_Item::SetStackable(const FPragma_Inventory_StackableItemUpdate& Value)
{
	Reset();
	Type = EPragma_Inventory_ItemUpdateV2_ItemType::Stackable;
	new(Union.Stackable.GetTypedPtr()) FPragma_Inventory_StackableItemUpdate(Value);
}

bool FPragma_Inventory_ItemUpdateV2_Item::SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj)
{
	const FPragma_Inventory_ItemUpdateV2_Item& OneOf = static_cast<const FPragma_Inventory_ItemUpdateV2*>(Value)->Item;
	switch (OneOf.OneOfType())
	{
		case EPragma_Inventory_ItemUpdateV2_ItemType::Instanced:
			OutJsonObj->SetField("instanced", PragmaJson::StructToJsonValue(*OneOf.Union.Instanced.GetTypedPtr()));
			break;
		case EPragma_Inventory_ItemUpdateV2_ItemType::Stackable:
			OutJsonObj->SetField("stackable", PragmaJson::StructToJsonValue(*OneOf.Union.Stackable.GetTypedPtr()));
			break;
		default:
			break;
	}
	// Per protobuf spec, it's ok to have empty oneof.
	return true;
}
bool FPragma_Inventory_ItemUpdateV2_Item::DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue)
{
	FPragma_Inventory_ItemUpdateV2_Item& OutOneOf = static_cast<FPragma_Inventory_ItemUpdateV2*>(OutValue)->Item;
	if (JsonObj->HasField("instanced"))
	{
		OutOneOf.Type = EPragma_Inventory_ItemUpdateV2_ItemType::Instanced;
		new(OutOneOf.Union.Instanced.GetTypedPtr()) FPragma_Inventory_InstancedItemUpdate(PragmaJson::JsonValueToStruct<FPragma_Inventory_InstancedItemUpdate>(JsonObj->TryGetField(TEXT("instanced")).ToSharedRef()));
		return true; // return to short circuit any other checks.
	}
	if (JsonObj->HasField("stackable"))
	{
		OutOneOf.Type = EPragma_Inventory_ItemUpdateV2_ItemType::Stackable;
		new(OutOneOf.Union.Stackable.GetTypedPtr()) FPragma_Inventory_StackableItemUpdate(PragmaJson::JsonValueToStruct<FPragma_Inventory_StackableItemUpdate>(JsonObj->TryGetField(TEXT("stackable")).ToSharedRef()));
		return true; // return to short circuit any other checks.
	}
	// Per protobuf spec, oneofs have _at most_ one of the fields. None set is valid, so we always return true.
	return true;
}

bool operator==(const FPragma_Inventory_ItemUpdateV2_Item& Lhs, const FPragma_Inventory_ItemUpdateV2_Item& Rhs)
{
	return Lhs.Equals(Rhs);
}
bool operator!=(const FPragma_Inventory_ItemUpdateV2_Item& Lhs, const FPragma_Inventory_ItemUpdateV2_Item& Rhs)
{
	return !(Lhs == Rhs);
}


bool FPragma_Inventory_ItemUpdateV2::AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj)
{
	bool Success = true;
	Success &= FPragma_Inventory_ItemUpdateV2_Item::SerializeIntoParentJson(Value, OutJsonObj);
	return Success;
}

bool FPragma_Inventory_ItemUpdateV2::AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue)
{
	bool Success = true;
	Success &= FPragma_Inventory_ItemUpdateV2_Item::DeserializeFromParentJson(JsonObj, OutValue);
	return Success;
}

bool operator==(const FPragma_Inventory_ItemUpdateV2& Lhs, const FPragma_Inventory_ItemUpdateV2& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Item == Rhs.Item
;
}
bool operator!=(const FPragma_Inventory_ItemUpdateV2& Lhs, const FPragma_Inventory_ItemUpdateV2& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Inventory_InstancedItemServerDestroy& Lhs, const FPragma_Inventory_InstancedItemServerDestroy& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.InstanceId == Rhs.InstanceId
;
}
bool operator!=(const FPragma_Inventory_InstancedItemServerDestroy& Lhs, const FPragma_Inventory_InstancedItemServerDestroy& Rhs)
{
	return !(Lhs == Rhs);
}

FPragma_Inventory_ItemServerDestroyV1_Item::FPragma_Inventory_ItemServerDestroyV1_Item() {}
FPragma_Inventory_ItemServerDestroyV1_Item::~FPragma_Inventory_ItemServerDestroyV1_Item()
{
	Reset();
}

FPragma_Inventory_ItemServerDestroyV1_Item::FPragma_Inventory_ItemServerDestroyV1_Item(const FPragma_Inventory_ItemServerDestroyV1_Item& Other)
{
	*this = Other;
}

bool FPragma_Inventory_ItemServerDestroyV1_Item::Equals(const FPragma_Inventory_ItemServerDestroyV1_Item& Other) const
{
    if (OneOfType() != Other.OneOfType())
    {
        return false;
    }
	switch (OneOfType())
	{
	case EPragma_Inventory_ItemServerDestroyV1_ItemType::Instanced:
		return Instanced() == Other.Instanced();
	case EPragma_Inventory_ItemServerDestroyV1_ItemType::Invalid:
        return true;
	default:
		return false;
	}
}

FPragma_Inventory_ItemServerDestroyV1_Item& FPragma_Inventory_ItemServerDestroyV1_Item::operator=(const FPragma_Inventory_ItemServerDestroyV1_Item& Other)
{
	Reset();
	Type = Other.OneOfType();
	switch (Other.OneOfType())
	{
	case EPragma_Inventory_ItemServerDestroyV1_ItemType::Instanced:
		new(Union.Instanced.GetTypedPtr()) FPragma_Inventory_InstancedItemServerDestroy(Other.Instanced());
		break;
	case EPragma_Inventory_ItemServerDestroyV1_ItemType::Invalid:
	default:
		break;
	}
	return *this;
}

void FPragma_Inventory_ItemServerDestroyV1_Item::Reset()
{
	switch (OneOfType())
	{
	case EPragma_Inventory_ItemServerDestroyV1_ItemType::Instanced:
		Union.Instanced.GetTypedPtr()->~FPragma_Inventory_InstancedItemServerDestroy();
		break;
	case EPragma_Inventory_ItemServerDestroyV1_ItemType::Invalid:
	default:
		break;
	}
	Type = EPragma_Inventory_ItemServerDestroyV1_ItemType::Invalid;
}

const FPragma_Inventory_InstancedItemServerDestroy& FPragma_Inventory_ItemServerDestroyV1_Item::Instanced() const {
	checkf(OneOfType() == EPragma_Inventory_ItemServerDestroyV1_ItemType::Instanced, TEXT("Failed to retrieve Instanced from FPragma_Inventory_ItemServerDestroyV1_Item. Check the OneOfType() before accessing the value."));
	return *Union.Instanced.GetTypedPtr();
}

void FPragma_Inventory_ItemServerDestroyV1_Item::SetInstanced(const FPragma_Inventory_InstancedItemServerDestroy& Value)
{
	Reset();
	Type = EPragma_Inventory_ItemServerDestroyV1_ItemType::Instanced;
	new(Union.Instanced.GetTypedPtr()) FPragma_Inventory_InstancedItemServerDestroy(Value);
}

bool FPragma_Inventory_ItemServerDestroyV1_Item::SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj)
{
	const FPragma_Inventory_ItemServerDestroyV1_Item& OneOf = static_cast<const FPragma_Inventory_ItemServerDestroyV1*>(Value)->Item;
	switch (OneOf.OneOfType())
	{
		case EPragma_Inventory_ItemServerDestroyV1_ItemType::Instanced:
			OutJsonObj->SetField("instanced", PragmaJson::StructToJsonValue(*OneOf.Union.Instanced.GetTypedPtr()));
			break;
		default:
			break;
	}
	// Per protobuf spec, it's ok to have empty oneof.
	return true;
}
bool FPragma_Inventory_ItemServerDestroyV1_Item::DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue)
{
	FPragma_Inventory_ItemServerDestroyV1_Item& OutOneOf = static_cast<FPragma_Inventory_ItemServerDestroyV1*>(OutValue)->Item;
	if (JsonObj->HasField("instanced"))
	{
		OutOneOf.Type = EPragma_Inventory_ItemServerDestroyV1_ItemType::Instanced;
		new(OutOneOf.Union.Instanced.GetTypedPtr()) FPragma_Inventory_InstancedItemServerDestroy(PragmaJson::JsonValueToStruct<FPragma_Inventory_InstancedItemServerDestroy>(JsonObj->TryGetField(TEXT("instanced")).ToSharedRef()));
		return true; // return to short circuit any other checks.
	}
	// Per protobuf spec, oneofs have _at most_ one of the fields. None set is valid, so we always return true.
	return true;
}

bool operator==(const FPragma_Inventory_ItemServerDestroyV1_Item& Lhs, const FPragma_Inventory_ItemServerDestroyV1_Item& Rhs)
{
	return Lhs.Equals(Rhs);
}
bool operator!=(const FPragma_Inventory_ItemServerDestroyV1_Item& Lhs, const FPragma_Inventory_ItemServerDestroyV1_Item& Rhs)
{
	return !(Lhs == Rhs);
}


bool FPragma_Inventory_ItemServerDestroyV1::AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj)
{
	bool Success = true;
	Success &= FPragma_Inventory_ItemServerDestroyV1_Item::SerializeIntoParentJson(Value, OutJsonObj);
	return Success;
}

bool FPragma_Inventory_ItemServerDestroyV1::AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue)
{
	bool Success = true;
	Success &= FPragma_Inventory_ItemServerDestroyV1_Item::DeserializeFromParentJson(JsonObj, OutValue);
	return Success;
}

bool operator==(const FPragma_Inventory_ItemServerDestroyV1& Lhs, const FPragma_Inventory_ItemServerDestroyV1& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Item == Rhs.Item
;
}
bool operator!=(const FPragma_Inventory_ItemServerDestroyV1& Lhs, const FPragma_Inventory_ItemServerDestroyV1& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Inventory_ItemBundleGrant& Lhs, const FPragma_Inventory_ItemBundleGrant& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Id == Rhs.Id
;
}
bool operator!=(const FPragma_Inventory_ItemBundleGrant& Lhs, const FPragma_Inventory_ItemBundleGrant& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Inventory_StackableItemGrant& Lhs, const FPragma_Inventory_StackableItemGrant& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.CatalogId == Rhs.CatalogId
			&& Lhs.Amount == Rhs.Amount
			&& Lhs.Tags == Rhs.Tags
;
}
bool operator!=(const FPragma_Inventory_StackableItemGrant& Lhs, const FPragma_Inventory_StackableItemGrant& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Inventory_InstancedItemServerGrant& Lhs, const FPragma_Inventory_InstancedItemServerGrant& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Ext == Rhs.Ext
			&& Lhs.CatalogId == Rhs.CatalogId
			&& Lhs.Tags == Rhs.Tags
;
}
bool operator!=(const FPragma_Inventory_InstancedItemServerGrant& Lhs, const FPragma_Inventory_InstancedItemServerGrant& Rhs)
{
	return !(Lhs == Rhs);
}

FPragma_Inventory_ItemServerGrantV1_Item::FPragma_Inventory_ItemServerGrantV1_Item() {}
FPragma_Inventory_ItemServerGrantV1_Item::~FPragma_Inventory_ItemServerGrantV1_Item()
{
	Reset();
}

FPragma_Inventory_ItemServerGrantV1_Item::FPragma_Inventory_ItemServerGrantV1_Item(const FPragma_Inventory_ItemServerGrantV1_Item& Other)
{
	*this = Other;
}

bool FPragma_Inventory_ItemServerGrantV1_Item::Equals(const FPragma_Inventory_ItemServerGrantV1_Item& Other) const
{
    if (OneOfType() != Other.OneOfType())
    {
        return false;
    }
	switch (OneOfType())
	{
	case EPragma_Inventory_ItemServerGrantV1_ItemType::Instanced:
		return Instanced() == Other.Instanced();
	case EPragma_Inventory_ItemServerGrantV1_ItemType::Stackable:
		return Stackable() == Other.Stackable();
	case EPragma_Inventory_ItemServerGrantV1_ItemType::Invalid:
        return true;
	default:
		return false;
	}
}

FPragma_Inventory_ItemServerGrantV1_Item& FPragma_Inventory_ItemServerGrantV1_Item::operator=(const FPragma_Inventory_ItemServerGrantV1_Item& Other)
{
	Reset();
	Type = Other.OneOfType();
	switch (Other.OneOfType())
	{
	case EPragma_Inventory_ItemServerGrantV1_ItemType::Instanced:
		new(Union.Instanced.GetTypedPtr()) FPragma_Inventory_InstancedItemServerGrant(Other.Instanced());
		break;
	case EPragma_Inventory_ItemServerGrantV1_ItemType::Stackable:
		new(Union.Stackable.GetTypedPtr()) FPragma_Inventory_StackableItemGrant(Other.Stackable());
		break;
	case EPragma_Inventory_ItemServerGrantV1_ItemType::Invalid:
	default:
		break;
	}
	return *this;
}

void FPragma_Inventory_ItemServerGrantV1_Item::Reset()
{
	switch (OneOfType())
	{
	case EPragma_Inventory_ItemServerGrantV1_ItemType::Instanced:
		Union.Instanced.GetTypedPtr()->~FPragma_Inventory_InstancedItemServerGrant();
		break;
	case EPragma_Inventory_ItemServerGrantV1_ItemType::Stackable:
		Union.Stackable.GetTypedPtr()->~FPragma_Inventory_StackableItemGrant();
		break;
	case EPragma_Inventory_ItemServerGrantV1_ItemType::Invalid:
	default:
		break;
	}
	Type = EPragma_Inventory_ItemServerGrantV1_ItemType::Invalid;
}

const FPragma_Inventory_InstancedItemServerGrant& FPragma_Inventory_ItemServerGrantV1_Item::Instanced() const {
	checkf(OneOfType() == EPragma_Inventory_ItemServerGrantV1_ItemType::Instanced, TEXT("Failed to retrieve Instanced from FPragma_Inventory_ItemServerGrantV1_Item. Check the OneOfType() before accessing the value."));
	return *Union.Instanced.GetTypedPtr();
}
const FPragma_Inventory_StackableItemGrant& FPragma_Inventory_ItemServerGrantV1_Item::Stackable() const {
	checkf(OneOfType() == EPragma_Inventory_ItemServerGrantV1_ItemType::Stackable, TEXT("Failed to retrieve Stackable from FPragma_Inventory_ItemServerGrantV1_Item. Check the OneOfType() before accessing the value."));
	return *Union.Stackable.GetTypedPtr();
}

void FPragma_Inventory_ItemServerGrantV1_Item::SetInstanced(const FPragma_Inventory_InstancedItemServerGrant& Value)
{
	Reset();
	Type = EPragma_Inventory_ItemServerGrantV1_ItemType::Instanced;
	new(Union.Instanced.GetTypedPtr()) FPragma_Inventory_InstancedItemServerGrant(Value);
}
void FPragma_Inventory_ItemServerGrantV1_Item::SetStackable(const FPragma_Inventory_StackableItemGrant& Value)
{
	Reset();
	Type = EPragma_Inventory_ItemServerGrantV1_ItemType::Stackable;
	new(Union.Stackable.GetTypedPtr()) FPragma_Inventory_StackableItemGrant(Value);
}

bool FPragma_Inventory_ItemServerGrantV1_Item::SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj)
{
	const FPragma_Inventory_ItemServerGrantV1_Item& OneOf = static_cast<const FPragma_Inventory_ItemServerGrantV1*>(Value)->Item;
	switch (OneOf.OneOfType())
	{
		case EPragma_Inventory_ItemServerGrantV1_ItemType::Instanced:
			OutJsonObj->SetField("instanced", PragmaJson::StructToJsonValue(*OneOf.Union.Instanced.GetTypedPtr()));
			break;
		case EPragma_Inventory_ItemServerGrantV1_ItemType::Stackable:
			OutJsonObj->SetField("stackable", PragmaJson::StructToJsonValue(*OneOf.Union.Stackable.GetTypedPtr()));
			break;
		default:
			break;
	}
	// Per protobuf spec, it's ok to have empty oneof.
	return true;
}
bool FPragma_Inventory_ItemServerGrantV1_Item::DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue)
{
	FPragma_Inventory_ItemServerGrantV1_Item& OutOneOf = static_cast<FPragma_Inventory_ItemServerGrantV1*>(OutValue)->Item;
	if (JsonObj->HasField("instanced"))
	{
		OutOneOf.Type = EPragma_Inventory_ItemServerGrantV1_ItemType::Instanced;
		new(OutOneOf.Union.Instanced.GetTypedPtr()) FPragma_Inventory_InstancedItemServerGrant(PragmaJson::JsonValueToStruct<FPragma_Inventory_InstancedItemServerGrant>(JsonObj->TryGetField(TEXT("instanced")).ToSharedRef()));
		return true; // return to short circuit any other checks.
	}
	if (JsonObj->HasField("stackable"))
	{
		OutOneOf.Type = EPragma_Inventory_ItemServerGrantV1_ItemType::Stackable;
		new(OutOneOf.Union.Stackable.GetTypedPtr()) FPragma_Inventory_StackableItemGrant(PragmaJson::JsonValueToStruct<FPragma_Inventory_StackableItemGrant>(JsonObj->TryGetField(TEXT("stackable")).ToSharedRef()));
		return true; // return to short circuit any other checks.
	}
	// Per protobuf spec, oneofs have _at most_ one of the fields. None set is valid, so we always return true.
	return true;
}

bool operator==(const FPragma_Inventory_ItemServerGrantV1_Item& Lhs, const FPragma_Inventory_ItemServerGrantV1_Item& Rhs)
{
	return Lhs.Equals(Rhs);
}
bool operator!=(const FPragma_Inventory_ItemServerGrantV1_Item& Lhs, const FPragma_Inventory_ItemServerGrantV1_Item& Rhs)
{
	return !(Lhs == Rhs);
}


bool FPragma_Inventory_ItemServerGrantV1::AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj)
{
	bool Success = true;
	Success &= FPragma_Inventory_ItemServerGrantV1_Item::SerializeIntoParentJson(Value, OutJsonObj);
	return Success;
}

bool FPragma_Inventory_ItemServerGrantV1::AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue)
{
	bool Success = true;
	Success &= FPragma_Inventory_ItemServerGrantV1_Item::DeserializeFromParentJson(JsonObj, OutValue);
	return Success;
}

bool operator==(const FPragma_Inventory_ItemServerGrantV1& Lhs, const FPragma_Inventory_ItemServerGrantV1& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Item == Rhs.Item
;
}
bool operator!=(const FPragma_Inventory_ItemServerGrantV1& Lhs, const FPragma_Inventory_ItemServerGrantV1& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Inventory_InstancedItem& Lhs, const FPragma_Inventory_InstancedItem& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.CatalogId == Rhs.CatalogId
			&& Lhs.InstanceId == Rhs.InstanceId
			&& Lhs.Ext == Rhs.Ext
;
}
bool operator!=(const FPragma_Inventory_InstancedItem& Lhs, const FPragma_Inventory_InstancedItem& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Inventory_StackableItem& Lhs, const FPragma_Inventory_StackableItem& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.CatalogId == Rhs.CatalogId
			&& Lhs.InstanceId == Rhs.InstanceId
			&& Lhs.Amount == Rhs.Amount
;
}
bool operator!=(const FPragma_Inventory_StackableItem& Lhs, const FPragma_Inventory_StackableItem& Rhs)
{
	return !(Lhs == Rhs);
}