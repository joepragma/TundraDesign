#pragma once
#include "Templates/SharedPointer.h"
#include "PragmaTypes.h"

class FJsonObject;
class UPragmaSdkConfig;

//
// DebugServer is abstract so that we can completely compile out the actual functionality (in ActiveDebugServer) in
// shipping builds without needing ifdef guards all over our code.
//
class FPragmaDebugServer
{
public:
	FPragmaDebugServer() {}
	virtual ~FPragmaDebugServer() {}

	// Returns true if mocks are enabled and we should ignore messages from the real server.
	virtual bool GetMockEnabled() const = 0;

	// Tell the server a response is starting to be processed. Must be paired with RequestProcessed.
	virtual void RequestStarted(EPragmaBackendType BackendType, EPragmaProtocolType ProtocolType, const FString& RequestTypeName, const FString& ResponseTypeName) = 0;
	// Tell the server the started request sent successfully and will expect a response. If a message is included it
	// is assumed the message sent successfully.
	virtual void RequestProcessed(bool bSuccess, const TSharedPtr<FJsonObject>& Message) = 0;

	// Tell the server a message is starting to be processed. Must be paired with MessageFailedToDeserialize or MessageProcessed.
	virtual void MessageReceived() = 0;
	// Tell the server we were unable to deserialize the message as json.
	virtual void MessageFailedToDeserialize(const FString& MessageJsonStr) = 0;
	// Tell the server the received message has finished processing (whether in success or error).
	virtual void MessageProcessed(int32 SequenceNumber, bool bSuccessfullyProcessed, const TSharedPtr<FJsonObject>& MessageJsonObject) = 0;
};
