// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/Dto.cpp.mustache
// </auto-generated>

#include "PragmaPlayerDataDto.h"
#include "CoreMinimal.h"
#include "PragmaJson.h"



bool operator==(const FPragma_PlayerData_EntityName& Lhs, const FPragma_PlayerData_EntityName& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Name == Rhs.Name
;
}
bool operator!=(const FPragma_PlayerData_EntityName& Lhs, const FPragma_PlayerData_EntityName& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_PlayerData_Entity& Lhs, const FPragma_PlayerData_Entity& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.EntityName == Rhs.EntityName
			&& Lhs.InstanceId == Rhs.InstanceId
			&& Lhs.Components == Rhs.Components
;
}
bool operator!=(const FPragma_PlayerData_Entity& Lhs, const FPragma_PlayerData_Entity& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_PlayerData_PlayerData& Lhs, const FPragma_PlayerData_PlayerData& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Entities == Rhs.Entities
			&& Lhs.Version == Rhs.Version
;
}
bool operator!=(const FPragma_PlayerData_PlayerData& Lhs, const FPragma_PlayerData_PlayerData& Rhs)
{
	return !(Lhs == Rhs);
}