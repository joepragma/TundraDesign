#pragma once

#include "PragmaPlayerApi.h"
#include "Dto/PragmaFriendServiceRaw.h"
#include "PragmaPresenceApi.generated.h"

UENUM()
enum class EBasicPresence
{
	Online,
	Away,
};

UCLASS(BlueprintType, Transient, Config=Game, Category=Pragma)
class PRAGMASDK_API UPragmaPresenceApi final : public UPragmaPlayerApi
{
	GENERATED_BODY()
		
public:
	
	void SetDependencies(UPragmaFriendServiceRaw* InFriendServiceRaw);
	////////////////////////////////////////////////////////////////
	// Events
	////////////////////////////////////////////////////////////////
	DECLARE_EVENT_OneParam(UPragmaPresenceApi, FPresenceChangedEvent, FPragma_Friend_Presence /* Presence */);
	FPresenceChangedEvent OnPresenceChanged;
	
	////////////////////////////////////////////////////////////////
	// Actions
	////////////////////////////////////////////////////////////////
	DECLARE_DELEGATE_OneParam(FOnCompleteDelegate, TPragmaResult<> /* Result */);

	// Set current presence to Online
	void SetAsOnline(const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> SetAsOnline();

	// Set current presence to Away
	void SetAsAway(const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> SetAsAway();

	// Set presence and rich presence
	void SetPresence(
	const EBasicPresence& BasicPresence,
	const FPragma_Presence_ExtRichPresence& RichPresence,
	const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> SetPresence(
		const EBasicPresence& BasicPresence,
		const FPragma_Presence_ExtRichPresence& RichPresence);

	// Set rich presence -- if basic presence does not yet exist, defaulted to EBasicPresence::Online
	void SetRichPresence(
		const FPragma_Presence_ExtRichPresence& RichPresence,
		const FOnCompleteDelegate& OnComplete);
	TFuture<TPragmaResult<>> SetRichPresence(
		const FPragma_Presence_ExtRichPresence& RichPresence);

	// Gets your current presence, will be a nullptr if you have not set your presence
	TSharedPtr<FPragma_Friend_Presence> GetPresence();
	
private:
	UPROPERTY()
	UPragmaFriendServiceRaw* FriendServiceRaw = nullptr;
	
	TSharedPtr<FPragma_Friend_Presence> Presence = nullptr;
	
	TFuture<TPragmaResult<>> Promisify(const TUniqueFunction<void(FOnCompleteDelegate)>& Func) const;

	////////////////////////////////////////////////////////////////
	// Notification Handlers
	////////////////////////////////////////////////////////////////
	void HandleOnPresenceUpdated(const FPragma_Friend_PresenceUpdateV1Notification Notification, const FPragmaMessageMetadata&);

	////////////////////////////////////////////////////////////////
	// Private Functions
	////////////////////////////////////////////////////////////////

	static FString ParseEnum(EBasicPresence Enum);
};
