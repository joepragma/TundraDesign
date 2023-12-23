#include "Services/Party/PragmaPartyPlayer.h"

bool operator==(const UPragmaPartyPlayer& Lhs, const UPragmaPartyPlayer& Rhs)
{
	return Lhs.Equals(Rhs);
}

bool UPragmaPartyPlayer::Equals(const UPragmaPartyPlayer& Other) const
{
	return GetPlayerId() == Other.GetPlayerId() &&
		GetSocialId() == Other.GetSocialId() &&
		GetDisplayName() == Other.GetDisplayName() &&
		IsReady() == Other.IsReady() &&
		GetExt() == Other.GetExt() &&
		IsLeader() == Other.IsLeader();
}

void UPragmaPartyPlayer::Initialize(FPragma_Party_BroadcastPartyMember PartyPlayer)
{
	PlayerId = MoveTemp(PartyPlayer.PlayerId);
	SocialId = MoveTemp(PartyPlayer.SocialId);
	DisplayName = MoveTemp(PartyPlayer.DisplayName);
	Ext = MoveTemp(PartyPlayer.Ext);
	bIsReady = PartyPlayer.IsReady;
	bIsLeader = PartyPlayer.IsLeader;
}

bool UPragmaPartyPlayer::UpdateFrom(FPragma_Party_BroadcastPartyMember PartyPlayer)
{
	bool HasChanges = false;
	if (PartyPlayer.PlayerId != PlayerId)
	{
		PlayerId = MoveTemp(PartyPlayer.PlayerId);
		HasChanges = true;
	}
	if (PartyPlayer.DisplayName != DisplayName)
	{
		DisplayName = MoveTemp(PartyPlayer.DisplayName);
		HasChanges = true;
	}
	if (PartyPlayer.IsReady != bIsReady)
	{
		bIsReady = PartyPlayer.IsReady;
		HasChanges = true;
	}
	if (PartyPlayer.IsLeader != bIsLeader)
	{
		bIsLeader = PartyPlayer.IsLeader;
		HasChanges = true;
	}
	if (PartyPlayer.Ext != Ext)
	{
		Ext = MoveTemp(PartyPlayer.Ext);
		HasChanges = true;
	}
	return HasChanges;
}
