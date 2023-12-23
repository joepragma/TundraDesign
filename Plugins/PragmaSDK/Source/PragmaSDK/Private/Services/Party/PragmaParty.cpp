#include "Services/Party/PragmaParty.h"

#include "Services/Party/PragmaPartyPlayer.h"

bool operator==(const UPragmaParty& Lhs, const UPragmaParty& Rhs)
{
	return Lhs.Equals(Rhs);
}

bool UPragmaParty::Equals(const UPragmaParty& Other) const
{
	for (const auto& OtherPlayer : Other.GetPlayers())
	{
		const auto PartyPlayer = GetPlayer(OtherPlayer->GetPlayerId());
		if (!PartyPlayer || OtherPlayer != PartyPlayer)
		{
			return false;
		}
	}
	return Id == Other.Id &&
		Ext == Other.Ext &&
		InviteCode == Other.InviteCode &&
		GetNumPlayers() == Other.GetNumPlayers();
}

void UPragmaParty::Initialize(FPragma_Party_BroadcastParty InParty)
{
	Id = MoveTemp(InParty.PartyId);
	InviteCode = MoveTemp(InParty.InviteCode);
	Ext = MoveTemp(InParty.ExtBroadcastParty);
	PreferredGameServerZones = MoveTemp(InParty.PreferredGameServerZones);

	for (auto Player : InParty.PartyMembers)
	{
		auto* PartyPlayer = NewObject<UPragmaPartyPlayer>();
		PartyPlayer->Initialize(MoveTemp(Player));
		Players.Emplace(PartyPlayer);
	}
}

const UPragmaPartyPlayer* UPragmaParty::GetLeader() const
{
	for (const auto* Player : Players)
	{
		if (Player->IsLeader())
		{
			return Player;
		}
	}
	return nullptr;
}

TArray<UPragmaPartyPlayer*> UPragmaParty::GetAllLeaders() const
{
	return Players.FilterByPredicate([](const UPragmaPartyPlayer* Player) { return Player->IsLeader(); });
}

const UPragmaPartyPlayer* UPragmaParty::GetPlayer(const FString& PlayerPragmaId) const
{
	for (const auto* Player : Players)
	{
		if (Player->GetPlayerId() == PlayerPragmaId)
		{
			return Player;
		}
	}
	return nullptr;
}

TArray<FString> UPragmaParty::GetPreferredGameServerZones() const
{
	return PreferredGameServerZones;
}

bool UPragmaParty::AreAllPlayersReady() const
{
	for (const auto* Player : Players)
	{
		if (!Player->IsReady())
		{
			return false;
		}
	}
	return true;
}
