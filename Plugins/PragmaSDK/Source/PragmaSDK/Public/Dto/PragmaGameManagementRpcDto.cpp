// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/Dto.cpp.mustache
// </auto-generated>

#include "PragmaGameManagementRpcDto.h"
#include "CoreMinimal.h"
#include "PragmaJson.h"



bool operator==(const FPragma_GameManagement_GetAllLimitedAccessEventsPartnerV1Response& Lhs, const FPragma_GameManagement_GetAllLimitedAccessEventsPartnerV1Response& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.LimitedAccessEvent == Rhs.LimitedAccessEvent
;
}
bool operator!=(const FPragma_GameManagement_GetAllLimitedAccessEventsPartnerV1Response& Lhs, const FPragma_GameManagement_GetAllLimitedAccessEventsPartnerV1Response& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_GameManagement_GetAllLimitedAccessEventsPartnerV1Request& Lhs, const FPragma_GameManagement_GetAllLimitedAccessEventsPartnerV1Request& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.GameShardId == Rhs.GameShardId
;
}
bool operator!=(const FPragma_GameManagement_GetAllLimitedAccessEventsPartnerV1Request& Lhs, const FPragma_GameManagement_GetAllLimitedAccessEventsPartnerV1Request& Rhs)
{
	return !(Lhs == Rhs);
}
