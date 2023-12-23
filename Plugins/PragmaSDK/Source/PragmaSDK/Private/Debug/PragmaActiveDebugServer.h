#pragma once

#include "CoreMinimal.h"
#if !UE_BUILD_SHIPPING
#include "PragmaDebugServer.h"
#include "PragmaDebugServerDto.h"
#include "PragmaJson.h"
#include "IHttpRouter.h"

class UPragmaSdkConfig;
class UPragmaConnection;

//
// NOTE: Debug server is currently experimental, not final. It is disabled by default, and is compiled out in shipping builds.
//
class FPragmaActiveDebugServer final : public FPragmaDebugServer, public TSharedFromThis<FPragmaActiveDebugServer>
{
public:
	FPragmaActiveDebugServer();
	virtual ~FPragmaActiveDebugServer();

	void Init(const UPragmaSdkConfig* SdkConfig, UPragmaConnection* InConnection);

	// Returns true if mocks are enabled and we should ignore messages from the real server.
	virtual bool GetMockEnabled() const override { return bMockEnabled; }

	// Tell the server a request is starting to be processed. Must be paired with RequestProcessed.
	virtual void RequestStarted(EPragmaBackendType BackendType, EPragmaProtocolType ProtocolType, const FString& RequestTypeName, const FString& ResponseTypeName) override;
	// Tell the server the started request sent successfully and will expect a response.
	virtual void RequestProcessed(bool bSuccess, const TSharedPtr<FJsonObject>& Message) override;

	// Tell the server a message is starting to be processed. Must be paired with MessageFailedToDeserialize or MessageProcessed.
	virtual void MessageReceived() override;
	// Tell the server we were unable to deserialize the message as json.
	virtual void MessageFailedToDeserialize(const FString& MessageJsonStr) override;
	// Tell the server the received message has finished processing (whether in success or error).
	virtual void MessageProcessed(int32 SequenceNumber, bool bSuccessfullyProcessed, const TSharedPtr<FJsonObject>& MessageJsonObject) override;

private:
	bool HandleV1Types(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete) const;
	bool HandleV1Requests(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete) const;
	bool HandleV1Messages(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete) const;
	bool HandleV1GetMockEnabled(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete) const;
	bool HandleV1SetMockEnabled(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete);
	bool HandleV1MockMessage(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete) const;

	void InitStructJsonMap();
	template <typename T>
	void InitStructJson(const T& Data)
	{
		FString Json;
		PragmaJson::StructToJsonStr(Data, Json);
		StructJsonMap.Add(Data.StaticStruct()->GetName(), Json);
	}

	UPragmaConnection* Connection;

	// Map of struct type name (via UScriptStruct::GetName()) -> defaulted struct as a json string.
	TMap<FString, FString> StructJsonMap;

	bool bMockEnabled{false};
	TUniquePtr<FPragmaDebugRequestInfo> CurrentRequest;
	TUniquePtr<FPragmaDebugMessageInfo> CurrentMessage;
	TArray<FPragmaDebugRequestInfo> Requests;
	TArray<FPragmaDebugMessageInfo> Messages;

	TFunction<void()> UnbindAllEndpoints;

	// TODO remove once sequence numbers are on everything again.
	int32 NextFakeSeqNumber = 10000;
};
#endif // if !UE_BUILD_SHIPPING
