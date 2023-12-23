// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/Dto.cpp.mustache
// </auto-generated>

#include "PragmaPartyRpcDto.h"
#include "CoreMinimal.h"
#include "PragmaJson.h"



bool operator==(const FPragma_Party_RemovedV1Notification& Lhs, const FPragma_Party_RemovedV1Notification& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.PartyId == Rhs.PartyId
			&& Lhs.RemovalReason == Rhs.RemovalReason
;
}
bool operator!=(const FPragma_Party_RemovedV1Notification& Lhs, const FPragma_Party_RemovedV1Notification& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_PartyClientVersionMismatchV1Notification& Lhs, const FPragma_Party_PartyClientVersionMismatchV1Notification& Rhs)
{
	return true // Initial true allows simpler codegen.
;
}
bool operator!=(const FPragma_Party_PartyClientVersionMismatchV1Notification& Lhs, const FPragma_Party_PartyClientVersionMismatchV1Notification& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_GameClientNotSupportedV1Notification& Lhs, const FPragma_Party_GameClientNotSupportedV1Notification& Rhs)
{
	return true // Initial true allows simpler codegen.
;
}
bool operator!=(const FPragma_Party_GameClientNotSupportedV1Notification& Lhs, const FPragma_Party_GameClientNotSupportedV1Notification& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_InviteResponseV1Notification& Lhs, const FPragma_Party_InviteResponseV1Notification& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.InviteId == Rhs.InviteId
			&& Lhs.Accepted == Rhs.Accepted
;
}
bool operator!=(const FPragma_Party_InviteResponseV1Notification& Lhs, const FPragma_Party_InviteResponseV1Notification& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_InviterInfo& Lhs, const FPragma_Party_InviterInfo& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.PlayerId == Rhs.PlayerId
			&& Lhs.DisplayName == Rhs.DisplayName
;
}
bool operator!=(const FPragma_Party_InviterInfo& Lhs, const FPragma_Party_InviterInfo& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_InviteReceivedV1Notification& Lhs, const FPragma_Party_InviteReceivedV1Notification& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.InviteId == Rhs.InviteId
			&& Lhs.InviterInfo == Rhs.InviterInfo
;
}
bool operator!=(const FPragma_Party_InviteReceivedV1Notification& Lhs, const FPragma_Party_InviteReceivedV1Notification& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_PartyDetailsV1Notification& Lhs, const FPragma_Party_PartyDetailsV1Notification& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Party == Rhs.Party
;
}
bool operator!=(const FPragma_Party_PartyDetailsV1Notification& Lhs, const FPragma_Party_PartyDetailsV1Notification& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_EnterMatchmakingV1Response& Lhs, const FPragma_Party_EnterMatchmakingV1Response& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Party == Rhs.Party
;
}
bool operator!=(const FPragma_Party_EnterMatchmakingV1Response& Lhs, const FPragma_Party_EnterMatchmakingV1Response& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_EnterMatchmakingV1Request& Lhs, const FPragma_Party_EnterMatchmakingV1Request& Rhs)
{
	return true // Initial true allows simpler codegen.
;
}
bool operator!=(const FPragma_Party_EnterMatchmakingV1Request& Lhs, const FPragma_Party_EnterMatchmakingV1Request& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_DevelopmentOverrideGameServerVersionV1Response& Lhs, const FPragma_Party_DevelopmentOverrideGameServerVersionV1Response& Rhs)
{
	return true // Initial true allows simpler codegen.
;
}
bool operator!=(const FPragma_Party_DevelopmentOverrideGameServerVersionV1Response& Lhs, const FPragma_Party_DevelopmentOverrideGameServerVersionV1Response& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_DevelopmentOverrideGameServerVersionV1Request& Lhs, const FPragma_Party_DevelopmentOverrideGameServerVersionV1Request& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.GameServerVersion == Rhs.GameServerVersion
;
}
bool operator!=(const FPragma_Party_DevelopmentOverrideGameServerVersionV1Request& Lhs, const FPragma_Party_DevelopmentOverrideGameServerVersionV1Request& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_UpdatePartyV1Response& Lhs, const FPragma_Party_UpdatePartyV1Response& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Party == Rhs.Party
;
}
bool operator!=(const FPragma_Party_UpdatePartyV1Response& Lhs, const FPragma_Party_UpdatePartyV1Response& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_UpdatePartyV1Request& Lhs, const FPragma_Party_UpdatePartyV1Request& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.RequestExt == Rhs.RequestExt
;
}
bool operator!=(const FPragma_Party_UpdatePartyV1Request& Lhs, const FPragma_Party_UpdatePartyV1Request& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_UpdatePartyPlayerV1Response& Lhs, const FPragma_Party_UpdatePartyPlayerV1Response& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Party == Rhs.Party
;
}
bool operator!=(const FPragma_Party_UpdatePartyPlayerV1Response& Lhs, const FPragma_Party_UpdatePartyPlayerV1Response& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_UpdatePartyPlayerV1Request& Lhs, const FPragma_Party_UpdatePartyPlayerV1Request& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.RequestExt == Rhs.RequestExt
;
}
bool operator!=(const FPragma_Party_UpdatePartyPlayerV1Request& Lhs, const FPragma_Party_UpdatePartyPlayerV1Request& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_SetPreferredGameServerZonesV1Response& Lhs, const FPragma_Party_SetPreferredGameServerZonesV1Response& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Party == Rhs.Party
;
}
bool operator!=(const FPragma_Party_SetPreferredGameServerZonesV1Response& Lhs, const FPragma_Party_SetPreferredGameServerZonesV1Response& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_SetGameServerZoneToPingV1Response& Lhs, const FPragma_Party_SetGameServerZoneToPingV1Response& Rhs)
{
	return true // Initial true allows simpler codegen.
;
}
bool operator!=(const FPragma_Party_SetGameServerZoneToPingV1Response& Lhs, const FPragma_Party_SetGameServerZoneToPingV1Response& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_SetGameServerZoneToPingV1Request& Lhs, const FPragma_Party_SetGameServerZoneToPingV1Request& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.GameServerZoneToPing.OrderIndependentCompareEqual(Rhs.GameServerZoneToPing)
;
}
bool operator!=(const FPragma_Party_SetGameServerZoneToPingV1Request& Lhs, const FPragma_Party_SetGameServerZoneToPingV1Request& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_SetPreferredGameServerZonesV1Request& Lhs, const FPragma_Party_SetPreferredGameServerZonesV1Request& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.PreferredGameServerZones == Rhs.PreferredGameServerZones
;
}
bool operator!=(const FPragma_Party_SetPreferredGameServerZonesV1Request& Lhs, const FPragma_Party_SetPreferredGameServerZonesV1Request& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_SetReadyStateV1Response& Lhs, const FPragma_Party_SetReadyStateV1Response& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Party == Rhs.Party
;
}
bool operator!=(const FPragma_Party_SetReadyStateV1Response& Lhs, const FPragma_Party_SetReadyStateV1Response& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_SetReadyStateV1Request& Lhs, const FPragma_Party_SetReadyStateV1Request& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Ready == Rhs.Ready
;
}
bool operator!=(const FPragma_Party_SetReadyStateV1Request& Lhs, const FPragma_Party_SetReadyStateV1Request& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_LeaveV1Response& Lhs, const FPragma_Party_LeaveV1Response& Rhs)
{
	return true // Initial true allows simpler codegen.
;
}
bool operator!=(const FPragma_Party_LeaveV1Response& Lhs, const FPragma_Party_LeaveV1Response& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_LeaveV1Request& Lhs, const FPragma_Party_LeaveV1Request& Rhs)
{
	return true // Initial true allows simpler codegen.
;
}
bool operator!=(const FPragma_Party_LeaveV1Request& Lhs, const FPragma_Party_LeaveV1Request& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_KickV1Response& Lhs, const FPragma_Party_KickV1Response& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Party == Rhs.Party
;
}
bool operator!=(const FPragma_Party_KickV1Response& Lhs, const FPragma_Party_KickV1Response& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_KickV1Request& Lhs, const FPragma_Party_KickV1Request& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.PlayerId == Rhs.PlayerId
;
}
bool operator!=(const FPragma_Party_KickV1Request& Lhs, const FPragma_Party_KickV1Request& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_AssignPartyLeaderV1Response& Lhs, const FPragma_Party_AssignPartyLeaderV1Response& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Party == Rhs.Party
;
}
bool operator!=(const FPragma_Party_AssignPartyLeaderV1Response& Lhs, const FPragma_Party_AssignPartyLeaderV1Response& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_AssignPartyLeaderV1Request& Lhs, const FPragma_Party_AssignPartyLeaderV1Request& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.PlayerId == Rhs.PlayerId
;
}
bool operator!=(const FPragma_Party_AssignPartyLeaderV1Request& Lhs, const FPragma_Party_AssignPartyLeaderV1Request& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_JoinWithPartyIdV1Response& Lhs, const FPragma_Party_JoinWithPartyIdV1Response& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Party == Rhs.Party
;
}
bool operator!=(const FPragma_Party_JoinWithPartyIdV1Response& Lhs, const FPragma_Party_JoinWithPartyIdV1Response& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_JoinWithPartyIdV1Request& Lhs, const FPragma_Party_JoinWithPartyIdV1Request& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.RequestExt == Rhs.RequestExt
			&& Lhs.GameClientVersion == Rhs.GameClientVersion
			&& Lhs.PartyId == Rhs.PartyId
			&& Lhs.GameServerZoneToPing.OrderIndependentCompareEqual(Rhs.GameServerZoneToPing)
;
}
bool operator!=(const FPragma_Party_JoinWithPartyIdV1Request& Lhs, const FPragma_Party_JoinWithPartyIdV1Request& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_JoinWithInviteCodeV1Response& Lhs, const FPragma_Party_JoinWithInviteCodeV1Response& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Party == Rhs.Party
;
}
bool operator!=(const FPragma_Party_JoinWithInviteCodeV1Response& Lhs, const FPragma_Party_JoinWithInviteCodeV1Response& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_JoinWithInviteCodeV1Request& Lhs, const FPragma_Party_JoinWithInviteCodeV1Request& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.RequestExt == Rhs.RequestExt
			&& Lhs.InviteCode == Rhs.InviteCode
			&& Lhs.GameClientVersion == Rhs.GameClientVersion
			&& Lhs.GameServerZoneToPing.OrderIndependentCompareEqual(Rhs.GameServerZoneToPing)
;
}
bool operator!=(const FPragma_Party_JoinWithInviteCodeV1Request& Lhs, const FPragma_Party_JoinWithInviteCodeV1Request& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_RespondToInviteV1Response& Lhs, const FPragma_Party_RespondToInviteV1Response& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Party == Rhs.Party
;
}
bool operator!=(const FPragma_Party_RespondToInviteV1Response& Lhs, const FPragma_Party_RespondToInviteV1Response& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_RespondToInviteV1Request& Lhs, const FPragma_Party_RespondToInviteV1Request& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.RequestExt == Rhs.RequestExt
			&& Lhs.InviteId == Rhs.InviteId
			&& Lhs.Accepted == Rhs.Accepted
			&& Lhs.GameClientVersion == Rhs.GameClientVersion
			&& Lhs.GameServerZoneToPing.OrderIndependentCompareEqual(Rhs.GameServerZoneToPing)
;
}
bool operator!=(const FPragma_Party_RespondToInviteV1Request& Lhs, const FPragma_Party_RespondToInviteV1Request& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_SendInviteV1Response& Lhs, const FPragma_Party_SendInviteV1Response& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.InviteId == Rhs.InviteId
;
}
bool operator!=(const FPragma_Party_SendInviteV1Response& Lhs, const FPragma_Party_SendInviteV1Response& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_SendInviteV1Request& Lhs, const FPragma_Party_SendInviteV1Request& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.InviteePlayerId == Rhs.InviteePlayerId
;
}
bool operator!=(const FPragma_Party_SendInviteV1Request& Lhs, const FPragma_Party_SendInviteV1Request& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_CreateV1Response& Lhs, const FPragma_Party_CreateV1Response& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Party == Rhs.Party
;
}
bool operator!=(const FPragma_Party_CreateV1Response& Lhs, const FPragma_Party_CreateV1Response& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Party_CreateV1Request& Lhs, const FPragma_Party_CreateV1Request& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.CreateRequestExt == Rhs.CreateRequestExt
			&& Lhs.PlayerJoinRequestExt == Rhs.PlayerJoinRequestExt
			&& Lhs.GameClientVersion == Rhs.GameClientVersion
			&& Lhs.PreferredGameServerZones == Rhs.PreferredGameServerZones
			&& Lhs.GameServerZoneToPing.OrderIndependentCompareEqual(Rhs.GameServerZoneToPing)
;
}
bool operator!=(const FPragma_Party_CreateV1Request& Lhs, const FPragma_Party_CreateV1Request& Rhs)
{
	return !(Lhs == Rhs);
}
