// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/Dto.cpp.mustache
// </auto-generated>

#include "PragmaPetDto.h"
#include "CoreMinimal.h"
#include "PragmaJson.h"



bool operator==(const FPragma_Inventory_PetUpdate& Lhs, const FPragma_Inventory_PetUpdate& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.XpDelta == Rhs.XpDelta
			&& Lhs.GearToEquipInstanceId == Rhs.GearToEquipInstanceId
;
}
bool operator!=(const FPragma_Inventory_PetUpdate& Lhs, const FPragma_Inventory_PetUpdate& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Inventory_Pet& Lhs, const FPragma_Inventory_Pet& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Bonus == Rhs.Bonus
			&& Lhs.BonusAmount == Rhs.BonusAmount
			&& Lhs.Xp == Rhs.Xp
			&& Lhs.GearEquippedInstanceId == Rhs.GearEquippedInstanceId
;
}
bool operator!=(const FPragma_Inventory_Pet& Lhs, const FPragma_Inventory_Pet& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Inventory_PetSpec& Lhs, const FPragma_Inventory_PetSpec& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Bonuses == Rhs.Bonuses
			&& Lhs.BonusMin == Rhs.BonusMin
			&& Lhs.BonusMax == Rhs.BonusMax
			&& Lhs.ItemGrants == Rhs.ItemGrants
;
}
bool operator!=(const FPragma_Inventory_PetSpec& Lhs, const FPragma_Inventory_PetSpec& Rhs)
{
	return !(Lhs == Rhs);
}
