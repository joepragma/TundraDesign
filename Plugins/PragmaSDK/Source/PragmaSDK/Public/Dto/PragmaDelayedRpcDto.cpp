// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/Dto.cpp.mustache
// </auto-generated>

#include "PragmaDelayedRpcDto.h"
#include "CoreMinimal.h"
#include "PragmaJson.h"



bool operator==(const FPragma_Delayed_DelayedV1Response& Lhs, const FPragma_Delayed_DelayedV1Response& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.DelayedFor == Rhs.DelayedFor
;
}
bool operator!=(const FPragma_Delayed_DelayedV1Response& Lhs, const FPragma_Delayed_DelayedV1Response& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Delayed_DelayedV1Request& Lhs, const FPragma_Delayed_DelayedV1Request& Rhs)
{
	return true // Initial true allows simpler codegen.
;
}
bool operator!=(const FPragma_Delayed_DelayedV1Request& Lhs, const FPragma_Delayed_DelayedV1Request& Rhs)
{
	return !(Lhs == Rhs);
}
