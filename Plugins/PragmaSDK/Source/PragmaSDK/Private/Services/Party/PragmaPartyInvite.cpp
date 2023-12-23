#include "Services/Party/PragmaPartyInvite.h"

#include "Dto/PragmaPartyRpcDto.h"

FPragmaPartyInvite::FPragmaPartyInvite(FPragma_Party_InviteReceivedV1Notification Notification)
	: InviteId(MoveTemp(Notification.InviteId)), InviterId(MoveTemp(Notification.InviterInfo.PlayerId))
{
}
