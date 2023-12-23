#pragma once
#include "Dto/PragmaPartyRpcDto.h"

#include "PragmaParty.generated.h"

class UPragmaPartyPlayer;

//
// This class stores all data relevant to a single party.
//
UCLASS(BlueprintType, Category=Pragma)
class PRAGMASDK_API UPragmaParty final : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(FPragma_Party_BroadcastParty PartyPlayer);

	const FString& GetId() const { return Id; }
	const FPragma_Party_ExtBroadcastParty& GetExt() const { return Ext; }
	const FPragma_Party_ExtPrivatePlayer& GetExtPrivatePlayer() const { return ExtPrivatePlayer; }
	int GetNumPlayers() const { return Players.Num(); }
	const TArray<UPragmaPartyPlayer*>& GetPlayers() const { return Players; }
	const UPragmaPartyPlayer* GetLeader() const;
	TArray<UPragmaPartyPlayer*> GetAllLeaders() const; 
	const UPragmaPartyPlayer* GetPlayer(const FString& PlayerPragmaId) const;
	TArray<FString> GetPreferredGameServerZones() const;

	bool AreAllPlayersReady() const;
	const FString& GetInviteCode() const { return InviteCode; }
	bool Equals(const UPragmaParty& Other) const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString Id;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FPragma_Party_ExtBroadcastParty Ext;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FPragma_Party_ExtPrivatePlayer ExtPrivatePlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<UPragmaPartyPlayer*> Players;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString InviteCode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	TArray<FString> PreferredGameServerZones;

	// PartyService handles directly updating state. This way we can see if anything changed and fire off relevant events.
	friend class UPragmaPartyProxy;
};

bool operator==(const UPragmaParty& Lhs, const UPragmaParty& Rhs);
