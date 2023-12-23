#pragma once
#include "CoreMinimal.h"

#include "PragmaBackendTypeConverter.h"
#include "PragmaTypes.h"
#include "PragmaJson.h"
#include "PragmaResult.h"
#include "PragmaLog.h"
#include "PragmaMessageMetadata.h"

class FPragmaDebugServer;

//
// Base class for sending and receiving Pragma messages.
//
// Implement SendJsonMessage and handle TextMessageReceived when inheriting.
//
class PRAGMASDK_API FPragmaProtocol
{
public:
	DECLARE_DELEGATE_OneParam(FSendFailure, FPragmaError);

    explicit FPragmaProtocol(const EPragmaBackendType InBackendType, const bool InLogPragmaDetailedMessages = false) : BackendType(InBackendType), bLogPragmaDetailedMessages(InLogPragmaDetailedMessages) {}
	virtual  ~FPragmaProtocol() = default;
	void     CleanupBeforeDestruction();

	virtual TWeakPtr<FPragmaProtocol> WeakThis() = 0;

	// No copy. This allows us to store TUniqueFunctions.
	FPragmaProtocol(const FPragmaProtocol& Other) = delete;
	FPragmaProtocol& operator=(const FPragmaProtocol& Other) = delete;

	void SetBackendTypeConverter(const TSharedRef<FPragmaBackendTypeConverter> InBackendTypeConverter) { BackendTypeConverter = InBackendTypeConverter; }

	// Send a message to the backend.
	//
	// OnComplete will be called when a response comes, with the deserialized payload if successful, or an ErrorCode
	// if the service returns an error.
    template <typename RequestType, typename ResponseType>
    void SendMessage(const RequestType& RequestPayload, TUniqueFunction<void(TPragmaResult<ResponseType>, const FPragmaMessageMetadata&)> OnComplete)
    {
		DebugRequestStarted(RequestType::StaticStruct()->GetName(), ResponseType::StaticStruct()->GetName());
		if (BackendTypeConverter == nullptr)
		{
			DebugRequestFailedToStart();
			PRAGMA_LOG(Error, "FPragmaProtocol::SendMessage -- BackendTypeConverter is not set. Do not try to send messages when Connection is not fully initialized.");
			OnComplete(TPragmaResult<ResponseType>::Failure(), FPragmaMessageMetadata::None());
			return;
		}
		FString RequestTypeName;
    	if (!BackendTypeConverter->GetBackendName(RequestType::StaticStruct(), &RequestTypeName))
    	{
    		DebugRequestFailedToStart();
    		PRAGMA_LOG(Error, "FPragmaProtocol::SendMessage -- The payload type %s does not convert to a valid"
    			" backend payload type. Make sure you're using a payload type that the Pragma backend expects by"
    			" consulting the Types map.", *RequestType::StaticStruct()->GetName());
    		OnComplete(TPragmaResult<ResponseType>::Failure(), FPragmaMessageMetadata::None());
    		return;
    	}

        // In order to keep the templating at this layer, we do json conversion here, and pass json strings internally.
        const TSharedPtr<FJsonObject> PayloadJsonObj = PragmaJson::StructToJsonObj(RequestPayload);
        if (PayloadJsonObj == nullptr)
        {
    		DebugRequestFailedToStart();
            PRAGMA_LOG(Error, "FPragmaProtocol::SendMessage(%s) -- Failed to serialize request payload as json.", *RequestTypeName);
        	OnComplete(TPragmaResult<ResponseType>::Failure(), FPragmaMessageMetadata::None());
            return;
        }
    	// Setup a callback that wraps the external OnComplete callback. This way we can handle templated json conversion
    	// here and pass back a nicely typed TPragmaResult.
    	auto OnCompleteInternal = [OnComplete = MoveTemp(OnComplete)](const TPragmaResult<TSharedPtr<FJsonObject>>& JsonResult, const FPragmaMessageMetadata& Metadata)
    	{
    		OnComplete(PragmaJson::JsonResultToTypedResult<ResponseType>(JsonResult), Metadata);
    	};
    	SendJsonMessage(RequestTypeName, PayloadJsonObj.ToSharedRef(), MoveTemp(OnCompleteInternal));
    }

    // Register to receive a callback when a notification of the specified type is comes over the socket.
    //
    // Returns a unique value to reference the handler in case it needs to be removed via RemoveNotificationHandler();
	typedef int32 FNotificationHandlerId;
    template <typename PayloadType>
    FNotificationHandlerId AddNotificationHandler(TUniqueFunction<void(PayloadType, const FPragmaMessageMetadata&)> Handler)
    {
    	if (BackendTypeConverter == nullptr)
    	{
    		PRAGMA_LOG(Error, "FPragmaProtocol::AddNotificationHandler -- BackendTypeConverter is not set. Do not try to add notification handlers when Connection is not fully initialized.");
    		return -1;
    	}
    	FString NotificationType;
    	if (!BackendTypeConverter->GetBackendName(PayloadType::StaticStruct(), &NotificationType))
    	{
    		PRAGMA_LOG(Error, "FPragmaProtocol::AddNotificationHandler -- Failed to add notification handler:"
                " The payload type %s does not convert to a valid backend payload type. Make sure you're using a"
                " payload type that the Pragma backend expects by consulting the Types map.",
                *PayloadType::StaticStruct()->GetName());
    		return -1;
    	}

        const auto HandlerId = NextNotificationHandlerId++;
        auto& HandlerMap = JsonNotificationHandlers.FindOrAdd(NotificationType);
        // Wrap the external handler with a lambda to deserialize the json result into the external type.
        HandlerMap.Add(HandlerId, [Handler = MoveTemp(Handler)](TSharedRef<FJsonObject> JsonObject, const FPragmaMessageMetadata& Metadata)
        {
        	PayloadType Payload;
        	if (!PragmaJson::JsonObjToStruct(JsonObject, Payload))
        	{
        		return;
        	}
            Handler(MoveTemp(Payload), Metadata);
        });
        return HandlerId;
    }
    template <typename PayloadType>
    void RemoveNotificationHandler(const FNotificationHandlerId NotificationHandlerId)
    {
    	if (BackendTypeConverter == nullptr)
    	{
    		PRAGMA_LOG(Error, "FPragmaProtocol::RemoveNotificationHandler -- BackendTypeConverter is not set. Do not try to remove notification handlers when the Connection is not fully initialized.");
    		return;
    	}
    	FString NotificationType;
    	if (!BackendTypeConverter->GetBackendName(PayloadType::StaticStruct(), &NotificationType))
    	{
    		PRAGMA_LOG(Error, "FPragmaProtocol::RemoveNotificationHandler -- Failed to remove notification handler:"
    			" The payload type %s does not convert to a valid backend payload type. Make sure you're using a"
    			" payload type that the Pragma backend expects by consulting the Types map.",
    			*PayloadType::StaticStruct()->GetName());
    		return;
    	}

    	auto* HandlerMap = JsonNotificationHandlers.Find(NotificationType);
    	if (HandlerMap == nullptr)
    	{
    		// Nothing has ever been registered for this notification.
    		return;
    	}
    	HandlerMap->Remove(NotificationHandlerId);
    }

	void SetDebugServer(TSharedPtr<FPragmaDebugServer> InDebugServer);

	EPragmaBackendType GetBackendType() const { return BackendType; }

protected:
	typedef int32 RequestId;
	static RequestId GetNextRequestId() { return NextRequestId++; }

	// Implement to call DebugServer->RequestStarted with the correct protocol.
	virtual void DebugRequestStarted(const FString& RequestTypeName, const FString& ResponseTypeName) const = 0;
	virtual void DebugRequestFailedToStart() const;
    virtual void DebugRequestStartedSuccessfully(const TSharedPtr<FJsonObject>& Message) const;
    virtual void DebugMessageReceived() const;
    virtual void DebugMessageFailedToDeserialize(const FString& MessageJsonStr) const;
	virtual void DebugMessageProcessed(int32 SequenceNumber, bool bSuccessfullyProcessed, const TSharedPtr<FJsonObject>& MessageJsonObject) const;

	FPragmaDebugServer* GetDebugServer() const { return DebugServer.Get(); }

	typedef TUniqueFunction<void(const TPragmaResult<TSharedPtr<FJsonObject>>& /* Result */, const FPragmaMessageMetadata& /* Metadata*/)> FJsonCallback;

	// Implement to allow short-circuiting message serialization, e.g. when socket is not connected or we're offline.
	virtual bool CanSendMessage() const = 0;

	// Implement to send the json Message string over the specific protocol.
	virtual bool SendJsonMessage(const FString& Message, const FSendFailure& SendFailure) = 0;

	// Call this when a json message is received from the backend.
	void JsonMessageReceived(const FString& MessageString);

private:
	void SendJsonMessage(FString RequestTypeName, const TSharedRef<FJsonObject> PayloadJsonObj, FJsonCallback OnComplete);
	void PragmaLogMessage(const FString& MessageHeading, const FString& MessageType, const FString& MessageString) const;

	// Notify any registered notification handlers that a json notification has been received.
	bool NotifyNotificationHandlers(const FString& NotificationType, const FPragmaMessageMetadata& Metadata, TSharedRef<FJsonObject> JsonPayload) const;

	// Static id across all protocols.
	static RequestId NextRequestId;

	// Incrementing ID to track notification handlers for later removal.
	FNotificationHandlerId NextNotificationHandlerId{ 1 };

	// Quickly convert between int/string backend types IDs.
	TSharedPtr<FPragmaBackendTypeConverter> BackendTypeConverter;

	// MessageId -> OnComplete callback.
    TMap<RequestId, FJsonCallback> JsonResponseCallbacks;

    // NotificationType -> NotificationHandlerId -> Notification (json) handler.
    TMap<FString, TMap<FNotificationHandlerId, TUniqueFunction<void(TSharedRef<FJsonObject>, const FPragmaMessageMetadata&)>>> JsonNotificationHandlers;

	// So we can post what's going on to the debug server.
	TSharedPtr<FPragmaDebugServer> DebugServer;

	// Backend type this protocol handles. This is just for debug info.
	EPragmaBackendType BackendType;

	bool bLogPragmaDetailedMessages{false};

#if !UE_BUILD_SHIPPING
	// For access to JsonMessageReceived to mock messages.
	friend class FPragmaActiveDebugServer;
#endif
};
