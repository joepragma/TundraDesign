#pragma once

#include "PragmaPartyInvite.generated.h"

struct FPragma_Party_InviteReceivedV1Notification;

USTRUCT(BlueprintType)
struct FPragmaPartyInvite
{
	GENERATED_BODY()

	FPragmaPartyInvite()
	{
	}

	explicit FPragmaPartyInvite(FPragma_Party_InviteReceivedV1Notification Notification);

	const FString& GetInviteId() const { return InviteId; }
	const FString& GetInviterId() const { return InviterId; }

protected:
	UPROPERTY(BlueprintReadOnly, Category=Pragma)
	FString InviteId;

	UPROPERTY(BlueprintReadOnly, Category=Pragma)
	FString InviterId;
};
