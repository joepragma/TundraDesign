#pragma once
#include "Dto/PragmaPartyRpcDto.h"

#include "PragmaPartyPlayer.generated.h"

//
// Information about your own state within the party
//
UCLASS(BlueprintType, Category=Pragma)
class PRAGMASDK_API UPragmaPartyPlayer final : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(FPragma_Party_BroadcastPartyMember PartyPlayer);
	bool UpdateFrom(FPragma_Party_BroadcastPartyMember PartyPlayer);

	const FString& GetPlayerId() const { return PlayerId; }
	const FString& GetSocialId() const { return SocialId; }
	const FPragma_Account_DisplayName& GetDisplayName() const { return DisplayName; }
	bool IsReady() const { return bIsReady; }
	bool IsLeader() const { return bIsLeader; }
	const FPragma_Party_ExtBroadcastPlayer& GetExt() const { return Ext; }

	bool Equals(const UPragmaPartyPlayer& ) const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString PlayerId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString SocialId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FPragma_Account_DisplayName DisplayName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	bool bIsReady{false};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	bool bIsLeader{false};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FPragma_Party_ExtBroadcastPlayer Ext;

	// PartyService handles directly updating state. This way we can see if data changed and fire off relevant events.
	friend class UPragmaGameLoopApi;
};


bool operator==(const UPragmaPartyPlayer& Lhs, const UPragmaPartyPlayer& Rhs);
