#pragma once

#include "PragmaHeartbeat.h"
#include "Dto/PragmaPlayerSessionRpcDto.h"
#include "PragmaPtr.h"

struct FPragmaMessageMetadata;

PRAGMA_FWD(FSession);

namespace Pragma
{
class FSessionServiceRawBase;

class PRAGMASDK_API FSessionServiceBase
{
public:
	FSessionServiceBase() = default;
	virtual ~FSessionServiceBase() = default;
	
	void SetDependencies(FSessionPtr InSession, FSessionServiceRawBase* InRawService, const TSharedPtr<IPragmaTimerManager>& InTimerManager, float InHeartbeatPeriodSeconds);

	// Access to this service's Raw API methods. Methods on the Raw API bypass any logic or caching provided by this service.
	// All capabilities should be available on this service, use the Raw API only for custom functionality.
	FSessionServiceRawBase& Raw() const;

	//
	// The pointer parameter is not stable beyond the scope of the event handler. If you need to save the string, copy it.
	//
	DECLARE_EVENT_OneParam(FSessionServiceBase, FAttributeChanged, const FString* /* AttributeValue */);
	DECLARE_EVENT_TwoParams(FSessionServiceBase, FSessionAttributes, const TMap<int32, FString> /* SessionAttributes */);
	FSessionAttributes OnGameSessionChanged;
	FSessionAttributes OnSocialSessionChanged;

	//
	// Get the event delegate used to broadcast for a given Social Session Attribute.
	//
	FAttributeChanged& GetAttributeChangedDelegate(EPragma_SocialSessionAttribute Attribute);
	//
	// Get the event delegate used to broadcast for a given Game Session Attribute.
	//
	FAttributeChanged& GetAttributeChangedDelegate(EPragma_GameSessionAttribute Attribute);

	//
	// Get an existing Social attribute, if set.
	//
	const FString* GetAttribute(EPragma_SocialSessionAttribute Attribute);
	//
	// Get an existing Game attribute, if set.
	//
	virtual const FString* GetAttribute(EPragma_GameSessionAttribute Attribute);

protected:
	static void BroadcastAttributeChanged(const TMap<int32, FAttributeChanged>& Events, int32 Key, const FString* AttributeValue);
	void Initialize();

	// Weird workaround while services are still UObjects.
	virtual UObject* WeakThisBinding() = 0;
	
private:
	struct FSessionData
	{
		int32 Version{MIN_int32};
		TMap<int32, FString> Attributes;
		TMap<int32, FAttributeChanged> Events;
	};

	void CreateHeartbeats();
	void BindHeartbeats();

	void HandleSocialSessionTerminated(FPragma_Session_SessionTerminatedV1Notification Notification, const FPragmaMessageMetadata& Metadata);
	void HandleGameSessionTerminated(FPragma_Session_SessionTerminatedV1Notification Notification, const FPragmaMessageMetadata& Metadata);
	void HandleSocialSessionChanged(FPragma_Session_SessionChangedV1Notification Notification, const FPragmaMessageMetadata& Metadata);
	void HandleGameSessionChanged(FPragma_Session_SessionChangedV1Notification Notification, const FPragmaMessageMetadata& Metadata);

	static void HandleSessionChangedNotification(FSessionData& SessionData, FPragma_Session_SessionChangedV1Notification Notification);
	
	FSessionPtr Session;
	FSessionServiceRawBase* RawService;
	TWeakPtr<IPragmaTimerManager> TimerManager;
	float HeartbeatPeriodSeconds{0};
	
	TSharedPtr<FPragmaHeartbeat> SocialHeartbeat;
	TSharedPtr<FPragmaHeartbeat> GameHeartbeat;

protected:
	FSessionData SocialSessionData;
	FSessionData GameSessionData;
};
}
