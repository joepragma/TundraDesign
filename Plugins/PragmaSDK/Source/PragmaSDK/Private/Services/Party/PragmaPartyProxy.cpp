#include "Services/Party/PragmaPartyProxy.h"

#include "PragmaLog.h"
#include "Services/Party/PragmaPartyPlayer.h"

DEFINE_LOG_CATEGORY_STATIC(LogPragmaPartyProxy, Error, All);

#define PARTY_PROXY_LOG(Verbosity, Format, ...) { PRAGMA_BASE_LOG(LogPragmaPartyProxy, Verbosity, Format, ##__VA_ARGS__); }

// ************************************************************
// *** THIS FILE WAS STUBBED FOR THIS TUNDRA DESIGN PROJECT ***
// ************************************************************

bool UPragmaPartyProxy::Initialize(FPragma_Party_BroadcastParty InParty, int64 Version)
{
	return UpdateWithVersionGuard(Version, [this, PartyDetailsV1 = MoveTemp(InParty)]
	{
		if (Party != nullptr)
		{
			OnPartyLeft.Broadcast();
		}

		Party = NewObject<UPragmaParty>();
		Party->Initialize(PartyDetailsV1);
		OnPartyJoined.Broadcast(Party);
		OnInviteCodeChanged.Broadcast(Party->InviteCode);
		OnExtBroadcastPartyChanged.Broadcast(Party->Ext);
		OnExtPrivatePlayerChanged.Broadcast(Party->ExtPrivatePlayer);
		OnPreferredGameServerZonesChanged.Broadcast(Party->PreferredGameServerZones);

		// Copy ptr for const interface.
		TArray<const UPragmaPartyPlayer*> Players;
		for (const auto* Player : Party->Players)
		{
			Players.Add(Player);
			OnPlayerJoined.Broadcast(Player);
		}
		OnPlayersChanged.Broadcast(Players);
	});
}

bool UPragmaPartyProxy::UpdateFrom(FPragma_Party_BroadcastParty InParty, int64 Version)
{
	if (Party == nullptr || InParty.PartyId != Party->Id)
	{
		return Initialize(InParty, Version);
	}

	return UpdateWithVersionGuard(Version, [this, PartyDetailsV1 = MoveTemp(InParty)]() mutable
	{
		SetExtBroadcastPartyAndNotify(MoveTemp(PartyDetailsV1.ExtBroadcastParty));
		SetInviteCodeAndNotify(MoveTemp(PartyDetailsV1.InviteCode));
		SetPartyMemberAndNotify(MoveTemp(PartyDetailsV1.PartyMembers));
		SetPreferredGameServerZonesAndNotify(MoveTemp(PartyDetailsV1.PreferredGameServerZones));
	});
}

bool UPragmaPartyProxy::PassesVersionGuard(int64 NewVersion) const
{
	return true;
}

bool UPragmaPartyProxy::UpdateWithVersionGuard(int64 Version, const TUniqueFunction<void()>& ApplyUpdates)
{
	if (!PassesVersionGuard(Version)) return false;

	PartyVersion = Version;
	ApplyUpdates();
	if(GetParty() != nullptr) {
		OnPartyChanged.Broadcast(GetParty());
	}
	return true;
}

bool UPragmaPartyProxy::TryReset(int64 Version)
{
	if (!PassesVersionGuard(Version)) return false;

	ForceReset();
	return true;
}

void UPragmaPartyProxy::ForceReset()
{
	if (Party == nullptr)
	{
		return;
	}

	Party = nullptr;
	PartyVersion = -1;
	OnPartyLeft.Broadcast();
}

void UPragmaPartyProxy::SetPartyIdAndNotify(FString NewPartyId)
{
	if (Party == nullptr)
	{
		Party = NewObject<UPragmaParty>();
		return;
	}
	if (Party->Id == NewPartyId)
	{
		return;
	}
	Party->Id = MoveTemp(NewPartyId);
	OnPartyJoined.Broadcast(Party);
}

void UPragmaPartyProxy::SetInviteCodeAndNotify(FString NewInviteCode) const
{
	if (!ensure(Party))
	{
		return;
	}
	if (Party->InviteCode == NewInviteCode)
	{
		return;
	}
	Party->InviteCode = MoveTemp(NewInviteCode);
	OnInviteCodeChanged.Broadcast(NewInviteCode);
}

void UPragmaPartyProxy::SetExtBroadcastPartyAndNotify(FPragma_Party_ExtBroadcastParty NewExtBroadcastParty) const
{
	if (!ensure(Party))
	{
		return;
	}
	if (Party->Ext == NewExtBroadcastParty)
	{
		return;
	}
	Party->Ext = MoveTemp(NewExtBroadcastParty);
	OnExtBroadcastPartyChanged.Broadcast(Party->Ext);
}

void UPragmaPartyProxy::SetPreferredGameServerZonesAndNotify(TArray<FString> NewPreferredGameServerZones) const
{
	if (!ensure(Party))
	{
		return;
	}
	if (Party->PreferredGameServerZones == NewPreferredGameServerZones)
	{
		return;
	}
	Party->PreferredGameServerZones = MoveTemp(NewPreferredGameServerZones);
	OnPreferredGameServerZonesChanged.Broadcast(Party->PreferredGameServerZones);
}

void UPragmaPartyProxy::SetPrivatePlayerAndNotify(
	FPragma_Party_ExtPrivatePlayer NewPrivatePlayer) const
{
	if (Party == nullptr)
	{
		return;
	}
	if (Party->ExtPrivatePlayer == NewPrivatePlayer)
	{
		return;
	}
	Party->ExtPrivatePlayer = MoveTemp(NewPrivatePlayer);
	OnExtPrivatePlayerChanged.Broadcast(Party->ExtPrivatePlayer);
}

void UPragmaPartyProxy::SetPartyMemberAndNotify(TArray<FPragma_Party_BroadcastPartyMember> NewPlayers) const
{
	if (!ensure(Party))
	{
		return;
	}

	TSet<FString> ExpectedFinalPlayerIds;
	bool AddedOrRemovedPlayers = false;
	for (auto NewPlayer : NewPlayers)
	{
		ExpectedFinalPlayerIds.Add(NewPlayer.PlayerId);
		const auto* CurrentPlayer = Party->Players.FindByPredicate([&NewPlayer](const UPragmaPartyPlayer* Player)
		{
			return Player->GetPlayerId() == NewPlayer.PlayerId;
		});
		if (CurrentPlayer != nullptr)
		{
			// Existing player.
			if ((*CurrentPlayer)->UpdateFrom(MoveTemp(NewPlayer)))
			{
				OnPlayerDataChanged.Broadcast(*CurrentPlayer);
			}
		}
		else
		{
			// New player.
			PARTY_PROXY_LOG(Verbose, "Player joined party: %s (%s)", *NewPlayer.DisplayName.DisplayName,
				*NewPlayer.PlayerId);
			auto* Player = NewObject<UPragmaPartyPlayer>();
			Player->Initialize(MoveTemp(NewPlayer));
			Party->Players.Emplace(Player);
			OnPlayerJoined.Broadcast(GetParty()->Players.Last());
			AddedOrRemovedPlayers = true;
		}
	}

	// Remove players no longer in party.
	for (int i = 0; i < Party->Players.Num(); /* ++i */)
	{
		if (ExpectedFinalPlayerIds.Contains(Party->Players[i]->GetPlayerId()))
		{
			++i;
		}
		else
		{
			// Removed player.
			PARTY_PROXY_LOG(Verbose, "Player left party: %s (%s)", *Party->Players[i]->GetDisplayName().DisplayName,
				*Party->Players[i]->GetPlayerId());
			auto Player = Party->Players[i];
			Party->Players.RemoveAt(i);
			OnPlayerLeft.Broadcast(MoveTemp(Player));
			AddedOrRemovedPlayers = true;
		}
	}

	if (AddedOrRemovedPlayers)
	{
		// Copy ptr for const interface.
		TArray<const UPragmaPartyPlayer*> Players;
		for (const auto* Player : Party->Players)
		{
			Players.Add(Player);
		}
		OnPlayersChanged.Broadcast(Players);
	}
}
