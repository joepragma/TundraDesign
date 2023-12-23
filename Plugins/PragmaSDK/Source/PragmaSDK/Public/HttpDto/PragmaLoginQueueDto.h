#pragma once

#include "CoreMinimal.h"
#include "PragmaLoginQueueDto.generated.h"

//
// The /v1/loginqueue/getinqueuev1 call is a http/json only call because it is the call that grabs a position/token
// for loginqueue and is artisinally crafted
//

USTRUCT()
struct FPragma_GetInQueueV1_Response
{
	GENERATED_BODY()

	UPROPERTY()
	FString Token;

	UPROPERTY()
	FString Eta;

	UPROPERTY()
	int32 PositionInQueue{0};

	UPROPERTY()
	int32 PollNextDuration{0};

	UPROPERTY()
	bool IsAllowedIn{false};
};

USTRUCT()
struct FPragma_CheckTicketV1_Request
{
	GENERATED_BODY()

	UPROPERTY()
	FString Token;
};


USTRUCT()
struct FPragma_CheckTicketV1_Response
{
	GENERATED_BODY()

	UPROPERTY()
	FString Token;

	UPROPERTY()
	FString Eta;

	UPROPERTY()
	int32 PositionInQueue{0};

	UPROPERTY()
	int32 PollNextDuration{0};

	UPROPERTY()
	bool IsAllowedIn{false};
};
