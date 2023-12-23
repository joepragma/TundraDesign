#pragma once

#include "PragmaParty.h"
#include "PragmaPartyProxy.generated.h"

class UPragmaPartyPlayer;

UCLASS(Transient)
class PRAGMASDK_API UPragmaPartyProxy : public UObject
{
	GENERATED_BODY()
public:
	// ************************************ //
	// METHODS								//
	// ************************************ //
	
	virtual bool Initialize(FPragma_Party_BroadcastParty InParty, int64 Version);
	virtual bool UpdateFrom(FPragma_Party_BroadcastParty InParty, int64 Version);
	virtual bool TryReset(int64 Version);
	virtual void ForceReset();
	virtual int64 GetVersion() const { return PartyVersion; }
	bool HasParty() const { return Party != nullptr; }
	virtual UPragmaParty* GetParty() const { return Party; }

	// ************************************ //
	// EVENTS								//
	// ************************************ //
	// These should match 1-1 with events from PragmaPartyService
	
	DECLARE_EVENT_OneParam(UPragmaPartyProxy, FPartyEvent, const UPragmaParty* /* Party */);

	FPartyEvent OnPartyJoined;
	FPartyEvent OnPartyChanged;

	DECLARE_EVENT_OneParam(UPragmaPartyProxy, FInviteCodeEvent, FString /* InviteCode */);

	FInviteCodeEvent OnInviteCodeChanged;

	DECLARE_EVENT_OneParam(UPragmaPartyProxy, FPartySelectionsEvent,
		const FPragma_Party_ExtBroadcastParty& /* ExtBroadcastParty */);

	FPartySelectionsEvent OnExtBroadcastPartyChanged;

	DECLARE_EVENT_OneParam(UPragmaPartyProxy, FPrivatePlayerSelectionsEvent,
		const FPragma_Party_ExtPrivatePlayer& /* ExtPrivatePlayer */);

	FPrivatePlayerSelectionsEvent OnExtPrivatePlayerChanged;

	DECLARE_EVENT_OneParam(UPragmaPartyProxy, FPlayerListEvent, const TArray<const UPragmaPartyPlayer*>& /* Players */);

	FPlayerListEvent OnPlayersChanged;

	DECLARE_EVENT_OneParam(UPragmaPartyProxy, FPlayerEvent, const UPragmaPartyPlayer* /* Player */);

	FPlayerEvent OnPlayerJoined;
	FPlayerEvent OnPlayerDataChanged;
	FPlayerEvent OnPlayerLeft;

	DECLARE_EVENT_OneParam(UPragmaPartyProxy, FPreferredGameServerZonesEvent,
		const TArray<FString>& /* PreferredGameServerZones */)

	FPreferredGameServerZonesEvent OnPreferredGameServerZonesChanged;

	DECLARE_EVENT(UPragmaPartyProxy, FPartyLeftEvent);

	FPartyLeftEvent OnPartyLeft;

protected:
	UPROPERTY(Transient)
	int64 PartyVersion{-1};

	UPROPERTY(Transient)
	UPragmaParty* Party;

private:
	/**
	* ApplyUpdates will be executed only if the supplied version is newer than the current version.
	* Afterwards, the current version will be updated and OnPartyChanged will be fired.
	*
	* Returns true only if the version guard passes.
	*
	* Example of a typical flow when handling a response or notification below.
	*
	* const bool PassedVersionGuard = UpdateWithVersionGuard(Metadata.SequenceNumber, [this]()
	* {
	*     // Modify Party.
	* });
	* if (!PassedVersionGuard)
	* {
	* 		// If we're handling a response: OnComplete.ExecuteIfBound(TPragmaResult<>::Success());
	* 		return;
	* }
	* // Continue doing other logic e.g. firing events, logging success.
	*/
	bool UpdateWithVersionGuard(int64 Version, const TUniqueFunction<void()>& ApplyUpdates);
	bool PassesVersionGuard(int64 NewVersion) const;
	void SetPartyIdAndNotify(FString NewPartyId);
	void SetInviteCodeAndNotify(FString NewInviteCode) const;
	void SetExtBroadcastPartyAndNotify(FPragma_Party_ExtBroadcastParty NewBroadcastParty) const;
	void SetPrivatePlayerAndNotify(FPragma_Party_ExtPrivatePlayer NewPrivatePlayer) const;
	void SetPartyMemberAndNotify(TArray<FPragma_Party_BroadcastPartyMember> NewPlayers) const;
	void SetPreferredGameServerZonesAndNotify(TArray<FString> NewPreferredGameServerZones) const;
};
