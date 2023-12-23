#include "PragmaProtocol.h"
#include "Dto/PragmaCoreDto.h"
#include "Debug/PragmaDebugServer.h"

#include "PragmaSdkConfig.h"
#include "PragmaLog.h"
#include "PragmaMessageMetadata.h"

FPragmaProtocol::RequestId FPragmaProtocol::NextRequestId = 1;

//
// List of backend messages to exclude from logging when the above bLogPragmaDetailedMessages is false [default]
//
const TArray<FString> PragmaDetailedMessageList{
	"PlayerSessionRpc.HeartbeatV1Request",
	"PlayerSessionRpc.HeartbeatV1Response",
	"MatchmakingRpc.GetMatchmakingInfoV2Request",
	"MatchmakingRpc.GetMatchmakingInfoV2Response",
	"GameInstanceRpc.GameKeepAliveV1Request",
	"GameInstanceRpc.GameKeepAliveV1Response",
	"MatchCapacityRpc.ReportCapacityV2Request",
	"MatchCapacityRpc.ReportCapacityV2Response",
	"FleetRpc.ReportCapacityV1Request",
	"FleetRpc.ReportCapacityV1Response",
	"PartnerSessionRpc.HeartbeatPartnerV1Response",
	"PartnerSessionRpc.HeartbeatPartnerV1Request"
};

void FPragmaProtocol::CleanupBeforeDestruction()
{
	// These callbacks must be destroyed inside this method called from the root PragmaSession while everything is still alive so that cached handlers
	// for responses haven't been destroyed when we try to call them.
	//
	// This can still be a problem in user code, but can be avoided by binding delegates properly using WeakLambda, UObject, or SP style delegate bindings.
	for (auto& Pair : JsonResponseCallbacks)
	{
		PRAGMA_LOG(Verbose, "FPragmaProtocol::~FPragmaProtocol -- Request ID '%d' has not yet returned, but we are being destroyed. Calling handler with error.", Pair.Key);
		Pair.Value(TPragmaResult<TSharedPtr<FJsonObject>>::Failure(EPragma_SdkError::ProtocolShuttingDown), FPragmaMessageMetadata{});
	}
	JsonResponseCallbacks.Empty();
}

void FPragmaProtocol::JsonMessageReceived(const FString& MessageString)
{
	DebugMessageReceived();
	// Messages come through as one big json blob with no separation between envelope and payload. For this reason
	// we deserialize it as a JsonObject and extract the Payload so that we can deserialize it as a more specific type.
	const TSharedPtr<FJsonObject> MessageJsonObject = PragmaJson::JsonStrToObj(MessageString);
	if (MessageJsonObject == nullptr)
	{
		DebugMessageFailedToDeserialize(MessageString);
		PRAGMA_LOG(Error, "FPragmaProtocol::JsonMessageReceived -- Unable to parse backend message as json."
			" Message response handler will not be called. Message: %s.", *MessageString);
		return;
	}

	const TSharedPtr<FJsonObject> PayloadJsonObj = FPragmaMessageJson::GetPayload(MessageJsonObject.ToSharedRef());
	FPragmaMessageJson Message;
	if (!PragmaJson::JsonObjToStruct(MessageJsonObject.ToSharedRef(), Message))
	{
		DebugMessageFailedToDeserialize(MessageString);
		PRAGMA_LOG(Error, "FPragmaProtocol::JsonMessageReceived -- Unable to parse backend message as a"
			" json-serialized FPragmaMessageJson. Message response handler will not be called. Message: %s.", *MessageString);
		return;
	}

	const FPragmaMessageMetadata Metadata{ Message.SequenceNumber };

	const FString& NotificationType = Message.Notification.Type;
	if (!NotificationType.IsEmpty())
	{
		const FString MessageHeading = FString::Printf(TEXT("NOTIFICATION %s (seq: %d): Ok, message body:"), *NotificationType, Message.SequenceNumber);

		PragmaLogMessage(MessageHeading, NotificationType, MessageString.ReplaceEscapedCharWithChar());
		const bool bFoundNotificationHandler = NotifyNotificationHandlers(NotificationType, Metadata, PayloadJsonObj.ToSharedRef());
		if (!bFoundNotificationHandler)
		{
			// This can happen if the response timed out but we eventually got a response,
			// or if a notification type has no handlers registered when we got a notification of that type. Either way,
			// the message was unable to be handled properly and may be a symptom of a larger problem.
			PRAGMA_LOG(Warning, "FPragmaProtocol::HandleWebSocketTextMessage -- Did not find a response callback or"
                " notification handler for NotificationType %s.", *NotificationType);
		}
		DebugMessageProcessed(Message.SequenceNumber, /* bSuccessfullyProcessed */ bFoundNotificationHandler, MessageJsonObject);
		return;
	}

	int32 RequestId;
	TPragmaResult<TSharedPtr<FJsonObject>> Result{ PayloadJsonObj };
	if (!Message.Response.Type.IsEmpty())
	{
		RequestId = Message.Response.RequestId;
		const FString MessageHeading = FString::Printf(TEXT("RESPONSE %d %s (seq: %d): Ok, message body:"), RequestId, *Message.Response.Type, Message.SequenceNumber);
		PragmaLogMessage(MessageHeading, Message.Response.Type, MessageString.ReplaceEscapedCharWithChar());
	}
	else
	{
		RequestId = Message.ServiceError.RequestId;
		Result = TPragmaResult<TSharedPtr<FJsonObject>>::Failure(MoveTemp(Message.ServiceError));
		PRAGMA_LOG(Error, "RESPONSE ERROR %d (seq: %d): Error code: %s, full message: %s", Message.ServiceError.RequestId,
			Message.SequenceNumber, *Result.ErrorCode(), *MessageString);
	}

	if (!JsonResponseCallbacks.Contains(RequestId))
	{
		PRAGMA_LOG(Error, "FPragmaProtocol::JsonMessageReceived -- Unable to find callback for RequestId %d.", RequestId);
		DebugMessageProcessed(Message.SequenceNumber, /* bSuccessfullyProcessed */ false, MessageJsonObject);
		return;
	}

	DebugMessageProcessed(Message.SequenceNumber, /* bSuccessfullyProcessed */ true, MessageJsonObject);
	const FJsonCallback ResponseCallback = JsonResponseCallbacks.FindAndRemoveChecked(RequestId);
	ResponseCallback(Result, Metadata);
}

void FPragmaProtocol::SendJsonMessage(FString RequestTypeName, const TSharedRef<FJsonObject> PayloadJsonObj, FJsonCallback OnComplete)
{
	if (!CanSendMessage())
	{
		DebugRequestFailedToStart();
		PRAGMA_LOG(Error, "FPragmaProtocol::SendJsonMessage -- Unable to send message because the protocol is not fully initialized (e.g. not logged in). Payload: %s",
            *PragmaJson::JsonObjToStr(PayloadJsonObj));
		OnComplete(TPragmaResult<TSharedPtr<FJsonObject>>::Failure(), FPragmaMessageMetadata::None());
		return;
	}
	const FPragmaRequestJson Request{GetNextRequestId(), MoveTemp(RequestTypeName)};
	const TSharedPtr<FJsonObject> RequestJsonObj = PragmaJson::StructToJsonObj(Request);
	if (RequestJsonObj == nullptr)
	{
    	DebugRequestFailedToStart();
		PRAGMA_LOG(Error, "FPragmaProtocol::SendJsonMessage(%s) -- Could not serialize Request object to json. Payload: %s",
			*Request.Type, *PragmaJson::JsonObjToStr(PayloadJsonObj));
		OnComplete(TPragmaResult<TSharedPtr<FJsonObject>>::Failure(), FPragmaMessageMetadata::None());
		return;
	}
	FPragmaRequestJson::InjectPayload(PayloadJsonObj, RequestJsonObj.ToSharedRef());

	const FString RequestJson = PragmaJson::JsonObjToStr(RequestJsonObj.ToSharedRef());
	const FString RequestType = Request.Type;

	const FString MessageHeading = FString::Printf(TEXT("REQUEST %d:"), Request.RequestId);
	PragmaLogMessage(MessageHeading, RequestType, RequestJson);

	JsonResponseCallbacks.Emplace(Request.RequestId, MoveTemp(OnComplete));
	const FSendFailure SendFailure = FSendFailure::CreateLambda([WeakThis = WeakThis(), RequestId = Request.RequestId](const FPragmaError Error)
	{
		const auto This = WeakThis.Pin();
		if (!This.IsValid())
		{
			return;
		}
		if (!This->JsonResponseCallbacks.Contains(RequestId))
		{
			return;
		}
		const auto JsonCallback = MoveTemp(This->JsonResponseCallbacks[RequestId]);
		This->JsonResponseCallbacks.Remove(RequestId);
		JsonCallback(TPragmaResult<TSharedPtr<FJsonObject>>::Failure(Error), FPragmaMessageMetadata::None());
	});
	if (!SendJsonMessage(RequestJson, SendFailure))
	{
		DebugRequestFailedToStart();
		auto _ = SendFailure.ExecuteIfBound(EPragma_SdkError::RequestFailedToSend);
		return;
	}
	DebugRequestStartedSuccessfully(RequestJsonObj);
}

void FPragmaProtocol::PragmaLogMessage(const FString& MessageHeading, const FString& MessageType, const FString& MessageString) const
{
	if (!bLogPragmaDetailedMessages && PragmaDetailedMessageList.Contains(MessageType))
	{
		return;
	}

	UE_SUPPRESS(LogPragma, Verbose,
	{
		if (UE_LOG_ACTIVE(LogPragma, VeryVerbose))
		{
			PRAGMA_LOG(Verbose, "%s %s.", *MessageHeading, *MessageString);
		}
		else
		{
			PRAGMA_LOG(Verbose, "%s %.2048s.", *MessageHeading, *MessageString);
		}
	});

}

bool FPragmaProtocol::NotifyNotificationHandlers(const FString& NotificationType, const FPragmaMessageMetadata& Metadata, const TSharedRef<FJsonObject> JsonPayload) const
{
	const auto* HandlerMap = JsonNotificationHandlers.Find(NotificationType);
	if (HandlerMap == nullptr || HandlerMap->Num() == 0)
	{
		// No handlers registered.
		return false;
	}
	for (const auto& Handler : *HandlerMap)
	{
		Handler.Value(JsonPayload, Metadata);
	}
	return true;
}

void FPragmaProtocol::SetDebugServer(TSharedPtr<FPragmaDebugServer> InDebugServer)
{
	DebugServer = MoveTemp(InDebugServer);
}
void FPragmaProtocol::DebugRequestFailedToStart() const
{
	if (DebugServer != nullptr)
	{
		DebugServer->RequestProcessed(false, nullptr);
	}
}
void FPragmaProtocol::DebugRequestStartedSuccessfully(const TSharedPtr<FJsonObject>& Message) const
{
	if (DebugServer != nullptr)
	{
		DebugServer->RequestProcessed(true, Message);
	}
}
void FPragmaProtocol::DebugMessageReceived() const
{
	if (DebugServer != nullptr)
	{
		DebugServer->MessageReceived();
	}
}
void FPragmaProtocol::DebugMessageFailedToDeserialize(const FString& MessageJsonStr) const
{
	if (DebugServer != nullptr)
	{
		DebugServer->MessageFailedToDeserialize(MessageJsonStr);
	}
}
void FPragmaProtocol::DebugMessageProcessed(
	const int32 SequenceNumber,
	const bool bSuccessfullyProcessed,
	const TSharedPtr<FJsonObject>& MessageJsonObject) const
{
	if (DebugServer != nullptr)
	{
		DebugServer->MessageProcessed(SequenceNumber, bSuccessfullyProcessed, MessageJsonObject);
	}
}
