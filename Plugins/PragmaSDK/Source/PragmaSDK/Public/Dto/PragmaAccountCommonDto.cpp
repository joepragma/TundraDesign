// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/Dto.cpp.mustache
// </auto-generated>

#include "PragmaAccountCommonDto.h"
#include "CoreMinimal.h"
#include "PragmaJson.h"



bool operator==(const FPragma_Account_DisplayName& Lhs, const FPragma_Account_DisplayName& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.DisplayName == Rhs.DisplayName
			&& Lhs.Discriminator == Rhs.Discriminator
;
}
bool operator!=(const FPragma_Account_DisplayName& Lhs, const FPragma_Account_DisplayName& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Account_PragmaAccountOverview& Lhs, const FPragma_Account_PragmaAccountOverview& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.PragmaSocialId == Rhs.PragmaSocialId
			&& Lhs.DisplayName == Rhs.DisplayName
;
}
bool operator!=(const FPragma_Account_PragmaAccountOverview& Lhs, const FPragma_Account_PragmaAccountOverview& Rhs)
{
	return !(Lhs == Rhs);
}
