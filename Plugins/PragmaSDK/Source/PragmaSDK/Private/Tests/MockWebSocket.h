#pragma once
#include "IWebSocket.h"
#include "PragmaJson.h"
#include "PragmaProtocol.h"
#include "Dto/PragmaCoreDto.h"
#include "Runtime/Launch/Resources/Version.h"

namespace Pragma
{
namespace Tests
{
class FMockWebSocket final : public IWebSocket
{
public:
	void MockConnect()
	{
		bIsConnected = true;
		ConnectedEvent.Broadcast();
	}
	void MockConnectionError(const FString& Error)
	{
		bIsConnected = false;
		ConnectionErrorEvent.Broadcast(Error);
	}
	void MockDisconnect(const int32 StatusCode, const FString& Reason, const bool bWasClean)
	{
		bIsConnected = false;
		ClosedEvent.Broadcast(StatusCode, Reason, bWasClean);
	}

	bool MockServerMessageError(const int32 RequestId, const FString& ErrorCode) const
	{
		FPragmaMessageJson Message{0};
		const auto& Index = StaticEnum<EPragma_PlatformError>()->GetValueByNameString(ErrorCode);

		Message.ServiceError.RequestId = RequestId;
		Message.ServiceError.Status = EPragma_ResponseStatusCode::SERVICE_ERROR;
		Message.ServiceError.Error = static_cast<EPragma_PlatformError>(Index);

		FString MessageJson;
		if (!PragmaJson::StructToJsonStr(Message, MessageJson))
		{
			return false;
		}
		MockServerMessageFullJson(MessageJson);
		return true;
	}
	bool MockServerMessageExtError(const int32 RequestId, const FString& ErrorCode) const
	{
		FPragmaMessageJson Message{0};
		const auto& Index = StaticEnum<EPragma_ExtError>()->GetValueByNameString(ErrorCode);

		Message.ServiceError.RequestId = RequestId;
		Message.ServiceError.Status = EPragma_ResponseStatusCode::SERVICE_ERROR;
		Message.ServiceError.ExtError = static_cast<EPragma_ExtError>(Index);

		FString MessageJson;
		if (!PragmaJson::StructToJsonStr(Message, MessageJson))
		{
			return false;
		}
		MockServerMessageFullJson(MessageJson);
		return true;
	}
	template <typename T>
	bool MockServerNotification(const FString& NotificationId, const T& Payload) const
	{
		FPragmaMessageJson Message{0};
		Message.Notification = FPragmaNotificationJson{NotificationId};
		Message.ServiceError.Status = EPragma_ResponseStatusCode::OK;
		const auto MessageObj = PragmaJson::StructToJsonObj(Message);
		auto PayloadObj = PragmaJson::StructToJsonObj(Payload);
		if (MessageObj == nullptr || PayloadObj == nullptr)
		{
			return false;
		}
		FPragmaNotificationJson::InjectPayload(PayloadObj.ToSharedRef(), MessageObj->GetObjectField(FPragmaMessageJson::ResponseFieldName()).ToSharedRef());
		MockServerMessageFullJson(PragmaJson::JsonObjToStr(MessageObj.ToSharedRef()));
		return true;
	}
	template <typename T>
	bool MockServerResponse(const int32 RequestId, const FString& BackendType, const T& Payload) const
	{
		FPragmaMessageJson Message{0};
		Message.Response = FPragmaResponseJson{RequestId, BackendType};
		Message.ServiceError.Status = EPragma_ResponseStatusCode::OK;
		const auto MessageObj = PragmaJson::StructToJsonObj(Message);
		auto PayloadObj = PragmaJson::StructToJsonObj(Payload);
		if (MessageObj == nullptr || PayloadObj == nullptr)
		{
			return false;
		}
		FPragmaResponseJson::InjectPayload(PayloadObj.ToSharedRef(), MessageObj->GetObjectField(FPragmaMessageJson::ResponseFieldName()).ToSharedRef());
		MockServerMessageFullJson(PragmaJson::JsonObjToStr(MessageObj.ToSharedRef()));
		return true;
	}
	void MockServerMessageFullJson(const FString& Message) const
	{
		MessageEvent.Broadcast(Message);
	}

	FString GetMockLastSentMessage() const { return LastSentMessage; }
	TSharedRef<FJsonObject> GetMockLastSentMessageObj() const { return PragmaJson::JsonStrToObj(LastSentMessage).ToSharedRef(); }
	int32 GetMockLastSendRequestId() const
	{
		const FString RequestMessage = GetMockLastSentMessage();
		const auto RequestObj = PragmaJson::JsonStrToObj(RequestMessage);
		checkf(RequestObj != nullptr, TEXT("MockWebSocket::GetMockLastSendRequestId -- Don't ask for GetMockLastSendRequestId if you haven't sent a message."));
		FPragmaRequestJson::GetPayload(RequestObj.ToSharedRef());
		FPragmaRequestJson Request;
		if (!PragmaJson::JsonObjToStruct(RequestObj.ToSharedRef(), Request))
		{
			PRAGMA_LOG(Error, "MockWebSocket::GetMockLastSendRequestId -- Failed to deserialie LastSentMessage as FPragmaRequestJson.");
			return -1;
		}
		return Request.RequestId;
	}

	// IWebSocket impl.
	virtual void Connect() override { /* Specifically does not connect so we can test error states. See MockConnect() etc. */ }
	virtual void Close(int32 Code, const FString& Reason = FString()) override {}
	virtual bool IsConnected() override { return bIsConnected; }
	virtual void Send(const FString& Data) override { LastSentMessage = Data; }
	virtual void Send(const void* Data, SIZE_T Size, bool bIsBinary = false) override {}

	virtual FWebSocketConnectedEvent& OnConnected() override { return ConnectedEvent; }
	virtual FWebSocketConnectionErrorEvent& OnConnectionError() override { return ConnectionErrorEvent; }
	virtual FWebSocketClosedEvent& OnClosed() override { return ClosedEvent; }
	virtual FWebSocketMessageEvent& OnMessage() override { return MessageEvent; }
	virtual FWebSocketRawMessageEvent& OnRawMessage() override { return RawMessageEvent; }
#if ENGINE_MAJOR_VERSION > 4 || ENGINE_MINOR_VERSION >= 25
	virtual FWebSocketMessageSentEvent& OnMessageSent() override { return MessageSentEvent; }
#endif
#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 2
	virtual FWebSocketBinaryMessageEvent& OnBinaryMessage() override { return BinaryMessageEvent; }
	virtual void SetTextMessageMemoryLimit(uint64 TextMessageMemoryLimit) override {}
#endif

private:
	FString LastSentMessage;

	FWebSocketConnectedEvent ConnectedEvent;
	FWebSocketConnectionErrorEvent ConnectionErrorEvent;
	FWebSocketClosedEvent ClosedEvent;
	FWebSocketMessageEvent MessageEvent;
	FWebSocketRawMessageEvent RawMessageEvent;
#if ENGINE_MAJOR_VERSION > 4 || ENGINE_MINOR_VERSION >= 25
	FWebSocketMessageSentEvent MessageSentEvent;
#endif
#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 2
	FWebSocketBinaryMessageEvent BinaryMessageEvent;
#endif

	bool bIsConnected{ false };
};
}
}
