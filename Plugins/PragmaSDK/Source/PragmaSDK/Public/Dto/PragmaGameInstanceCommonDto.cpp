// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/Dto.cpp.mustache
// </auto-generated>

#include "PragmaGameInstanceCommonDto.h"
#include "CoreMinimal.h"
#include "PragmaJson.h"



bool operator==(const FPragma_GameInstance_PlayerGameResult& Lhs, const FPragma_GameInstance_PlayerGameResult& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.PlayerId == Rhs.PlayerId
			&& Lhs.Ext == Rhs.Ext
;
}
bool operator!=(const FPragma_GameInstance_PlayerGameResult& Lhs, const FPragma_GameInstance_PlayerGameResult& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_GameInstance_PlayerToRemove& Lhs, const FPragma_GameInstance_PlayerToRemove& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.PlayerId == Rhs.PlayerId
			&& Lhs.Ext == Rhs.Ext
;
}
bool operator!=(const FPragma_GameInstance_PlayerToRemove& Lhs, const FPragma_GameInstance_PlayerToRemove& Rhs)
{
	return !(Lhs == Rhs);
}
