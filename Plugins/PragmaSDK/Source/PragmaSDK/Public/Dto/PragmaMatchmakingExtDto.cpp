// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/Dto.cpp.mustache
// </auto-generated>

#include "PragmaMatchmakingExtDto.h"
#include "CoreMinimal.h"
#include "PragmaJson.h"



bool operator==(const FPragma_Matchmaking_ExtMatchmakingKey& Lhs, const FPragma_Matchmaking_ExtMatchmakingKey& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.ExampleString == Rhs.ExampleString
			&& Lhs.ExampleLong == Rhs.ExampleLong
			&& Lhs.ExampleProto == Rhs.ExampleProto
;
}
bool operator!=(const FPragma_Matchmaking_ExtMatchmakingKey& Lhs, const FPragma_Matchmaking_ExtMatchmakingKey& Rhs)
{
	return !(Lhs == Rhs);
}