#include "Services/Friend/FriendResponseHandler.h"
DEFINE_LOG_CATEGORY_STATIC(LogFriendResponseHandler, Error, All);

TPragmaResult<> FFriendResponseHandler::HandleResponse(const FString& Response)
{
	if (Response == TEXT("Ok"))
	{
		return TPragmaResult<>::Success();
	}

	UE_LOG(LogFriendResponseHandler, Log, TEXT("FriendResponseHandler parsing failure response: %s"), *Response);

	if (Response == TEXT("InviteeNotFound"))
	{
		return TPragmaResult<>::Failure(EPragma_SdkError::Friend_InviteeNotFound);
	}
	if (Response == TEXT("InviteNotFound"))
	{
		return TPragmaResult<>::Failure(EPragma_SdkError::Friend_InviteNotFound);
	}
	if (Response == TEXT("InviterNotFound"))
	{
		return TPragmaResult<>::Failure(EPragma_SdkError::Friend_InviterNotFound);
	}
	if (Response == TEXT("FriendNotFound"))
	{
		return TPragmaResult<>::Failure(EPragma_SdkError::Friend_FriendNotFound);
	}
	if (Response == TEXT("InviteeAlreadyFriend"))
	{
		return TPragmaResult<>::Failure(EPragma_SdkError::Friend_InviteeAlreadyFriend);
	}
	if (Response == TEXT("FriendListFull"))
	{
		return TPragmaResult<>::Failure(EPragma_SdkError::Friend_FriendListFull);
	}
	if (Response == TEXT("InviteeReceivedInvitesFull"))
	{
		return TPragmaResult<>::Failure(EPragma_SdkError::Friend_InviteeReceivedInvitesFull);
	}
	if (Response == TEXT("GameIdentityNotFound"))
	{
		return TPragmaResult<>::Failure(EPragma_SdkError::Presence_GameIdentityNotFound);
	}
	if (Response == TEXT("PresenceNotFound"))
	{
		return TPragmaResult<>::Failure(EPragma_SdkError::Presence_PresenceNotFound);
	}

	return TPragmaResult<>::Failure(EPragma_SdkError::Friend_UnknownError);
}
