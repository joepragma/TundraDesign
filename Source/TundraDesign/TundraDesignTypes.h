#pragma once

#include "Dto/PragmaPartyRpcExtDto.h"
#include "Services/Party/PragmaParty.h"
#include "Services/Party/PragmaPartyPlayer.h"
#include "TundraDesignTypes.generated.h"

USTRUCT(BlueprintType, Category="TundraDesign")
struct FTundraDesignPlayer
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="TundraDesign")
	FString PlayerId;

	UPROPERTY(BlueprintReadOnly, Category="TundraDesign")
	FString SocialId;

	UPROPERTY(BlueprintReadOnly, Category="TundraDesign")
	FString Username;
};

UENUM(BlueprintType, Category="TundraDesign")
enum class ETundraDesignGameMode : uint8
{
	Normal = 0,
	Ranked = 1
};

inline ETundraDesignGameMode ToTundraDesignGameMode(EPragma_Party_GameMode GameMode)
{
	switch (GameMode)
	{
	case EPragma_Party_GameMode::NORMAL:
		return ETundraDesignGameMode::Normal;
	case EPragma_Party_GameMode::RANKED:
		return ETundraDesignGameMode::Ranked;
	default:
		throw std::invalid_argument("Invalid EPragma_Party_GameMode");
	}
}

inline EPragma_Party_GameMode ToPragmaGameMode(ETundraDesignGameMode GameMode)
{
	switch (GameMode)
	{
	case ETundraDesignGameMode::Normal:
		return EPragma_Party_GameMode::NORMAL;
	case ETundraDesignGameMode::Ranked:
		return EPragma_Party_GameMode::RANKED;
	default:
		return EPragma_Party_GameMode::GAME_MODE_UNSPECIFIED;
	}
}

USTRUCT(BlueprintType, Category="TundraDesign")
struct FTundraDesignPartyPlayer
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="TundraDesign")
	FString PlayerId;

	UPROPERTY(BlueprintReadOnly, Category="TundraDesign")
	FString SocialId;

	UPROPERTY(BlueprintReadOnly, Category="TundraDesign")
	FString Username;

	UPROPERTY(BlueprintReadOnly, Category="TundraDesign")
	bool IsLeader;

	UPROPERTY(BlueprintReadOnly, Category="TundraDesign")
	bool IsReady;
};

inline FTundraDesignPartyPlayer ToTundraDesignPartyPlayer(const UPragmaPartyPlayer* PartyPlayer)
{
	FTundraDesignPartyPlayer TundraPartyPlayer;
	TundraPartyPlayer.PlayerId = PartyPlayer->GetPlayerId();
	TundraPartyPlayer.SocialId = PartyPlayer->GetSocialId();
	TundraPartyPlayer.Username = FString::Printf(TEXT("%s#%s"), *PartyPlayer->GetDisplayName().DisplayName,
	                                             *PartyPlayer->GetDisplayName().Discriminator);
	TundraPartyPlayer.IsLeader = PartyPlayer->IsLeader();
	TundraPartyPlayer.IsReady = PartyPlayer->IsReady();
	return TundraPartyPlayer;
}

USTRUCT(BlueprintType, Category="TundraDesign")
struct FTundraDesignParty
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="TundraDesign")
	FString Id;

	UPROPERTY(BlueprintReadOnly, Category="TundraDesign")
	FString InviteCode;

	UPROPERTY(BlueprintReadOnly, Category="TundraDesign")
	ETundraDesignGameMode GameMode;

	UPROPERTY(BlueprintReadOnly, Category="TundraDesign")
	TArray<FTundraDesignPartyPlayer> Players;
};

inline FTundraDesignParty ToTundraDesignParty(const UPragmaParty* Party)
{
	FTundraDesignParty TundraParty;
	TundraParty.Id = Party->GetId();
	TundraParty.InviteCode = Party->GetInviteCode();
	TundraParty.GameMode = ToTundraDesignGameMode(Party->GetExt().GameMode);
	for (const auto* Player : Party->GetPlayers())
	{
		TundraParty.Players.Add(ToTundraDesignPartyPlayer(Player));
	}
	return TundraParty;
}