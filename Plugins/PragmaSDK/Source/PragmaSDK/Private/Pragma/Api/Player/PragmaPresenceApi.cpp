#include "Pragma/Api/Player/PragmaPresenceApi.h"
#include "PragmaSessionV2.h"
#include "Services/Friend/FriendResponseHandler.h"

DEFINE_LOG_CATEGORY_STATIC(LogPragmaPresenceApi, Error, All);

#define PRESENCE_API_LOG(Verbosity, Format, ...) { PRAGMA_BASE_LOG(LogPragmaPresenceApi, Verbosity, Format, ##__VA_ARGS__); }

void UPragmaPresenceApi::SetDependencies(UPragmaFriendServiceRaw* InFriendServiceRaw)
{
	FriendServiceRaw = InFriendServiceRaw;

	FriendServiceRaw->OnPresenceUpdateV1.AddUObject(this, &UPragmaPresenceApi::HandleOnPresenceUpdated);
}

void UPragmaPresenceApi::SetAsOnline(const FOnCompleteDelegate& OnComplete)
{
	SetPresence(EBasicPresence::Online, FPragma_Presence_ExtRichPresence{}, OnComplete);
}

TFuture<TPragmaResult<>> UPragmaPresenceApi::SetAsOnline()
{
	auto                                                                    Promise = MakeShared<TPromise<TPragmaResult<>>>();
	SetAsOnline(FOnCompleteDelegate::CreateLambda([Promise](TPragmaResult<> Result) { Promise->SetValue(MoveTemp(Result)); }));
	return Promise->GetFuture();
}

void UPragmaPresenceApi::SetAsAway(const FOnCompleteDelegate& OnComplete)
{
	auto Ext = FPragma_Presence_ExtRichPresence{};
	if (Presence.IsValid())
	{
		Ext = Presence.Get()->Ext;
	}
	SetPresence(EBasicPresence::Away, Ext, OnComplete);
}

TFuture<TPragmaResult<>> UPragmaPresenceApi::SetAsAway()
{
	auto                                                                  Promise = MakeShared<TPromise<TPragmaResult<>>>();
	SetAsAway(FOnCompleteDelegate::CreateLambda([Promise](TPragmaResult<> Result) { Promise->SetValue(MoveTemp(Result)); }));
	return Promise->GetFuture();
}

void UPragmaPresenceApi::SetPresence(const EBasicPresence& BasicPresence, const FPragma_Presence_ExtRichPresence& RichPresence, const FOnCompleteDelegate& OnComplete)
{
	FriendServiceRaw->SetPresenceV1(FPragma_Friend_SetPresenceV1Request{ ParseEnum(BasicPresence), Session()->GameShardId(), RichPresence },
		UPragmaFriendServiceRaw::FSetPresenceV1Delegate::CreateWeakLambda(this,
			[this, OnComplete]
		(const TPragmaResult<FPragma_Friend_SetPresenceV1Response>& Result, const FPragmaMessageMetadata& _) {
				if (Result.IsFailure())
				{
					UE_LOG(LogPragmaPresenceApi, Warning, TEXT("SetPresence - Error setting presence"));
					OnComplete.ExecuteIfBound(TPragmaResult<>::Failure(Result.ErrorCode()));
					return;
				}

				OnComplete.ExecuteIfBound(FFriendResponseHandler::HandleResponse(Result.Payload().Response));
			}
			));
}

TFuture<TPragmaResult<>> UPragmaPresenceApi::SetPresence(const EBasicPresence& BasicPresence, const FPragma_Presence_ExtRichPresence& RichPresence)
{
	auto                                                                                                 Promise = MakeShared<TPromise<TPragmaResult<>>>();
	SetPresence(BasicPresence, RichPresence, FOnCompleteDelegate::CreateLambda([Promise](TPragmaResult<> Result) { Promise->SetValue(MoveTemp(Result)); }));
	return Promise->GetFuture();
}

void UPragmaPresenceApi::SetRichPresence(const FPragma_Presence_ExtRichPresence& RichPresence, const FOnCompleteDelegate& OnComplete)
{
	SetPresence(EBasicPresence::Online, RichPresence, OnComplete);
}

TFuture<TPragmaResult<>> UPragmaPresenceApi::SetRichPresence(const FPragma_Presence_ExtRichPresence& RichPresence)
{
	auto                                                                                      Promise = MakeShared<TPromise<TPragmaResult<>>>();
	SetRichPresence(RichPresence, FOnCompleteDelegate::CreateLambda([Promise](TPragmaResult<> Result) { Promise->SetValue(MoveTemp(Result)); }));
	return Promise->GetFuture();
}

TSharedPtr<FPragma_Friend_Presence> UPragmaPresenceApi::GetPresence()
{
	return Presence;
}

void UPragmaPresenceApi::HandleOnPresenceUpdated(const FPragma_Friend_PresenceUpdateV1Notification Notification, const FPragmaMessageMetadata&)
{
	Presence = MakeShared<FPragma_Friend_Presence>(Notification.NewPresence);
	OnPresenceChanged.Broadcast(*Presence.Get());
}

FString UPragmaPresenceApi::ParseEnum(const EBasicPresence Enum)
{
	switch (Enum)
	{
		case EBasicPresence::Online:
			return TEXT("Online");
		case EBasicPresence::Away:
			return TEXT("Away");
		default:
			return TEXT("Unknown");
	}
}
