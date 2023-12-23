
#include "PragmaActiveDebugServer.h"

#include "CoreMinimal.h"
#if !UE_BUILD_SHIPPING

#include "Misc/CommandLine.h"
#include "HttpServerModule.h"
#include "IHttpRouter.h"
#include "HttpServerRequest.h"
#include "HttpServerResponse.h"
#include "HttpRequestHandler.h"
#include "HttpServerConstants.h"
#include "Dto/PragmaAccountRpcDto.h"
#include "Dto/PragmaConfigDto.h"
#include "PragmaSdkConfig.h"
#include "PragmaLog.h"
#include "PragmaConnection.h"
#include "PragmaProtocol.h"
#include "PragmaHttp.h"
#include "HttpDto/PragmaAuthenticateDto.h"

namespace
{
// Keep this many request-response pairs and notifications in memory.
const int MessageHistorySize = 50;

FString SerializeRequests(const FPragmaDebugRequests& Payload, const TArray<FPragmaDebugRequestInfo>& Requests)
{
	TArray<TSharedPtr<FJsonValue>> RequestJsonValues;
	for (const auto& Request : Requests)
	{
		TSharedPtr<FJsonObject> RequestJsonObj = PragmaJson::StructToJsonObj(Request);
		if (RequestJsonObj == nullptr)
		{
			PRAGMA_LOG(Error, "PragmaActiveDebugServer.cpp::SerializeRequests -- Unable to convert Request struct to json object.");
			continue;
		}
		// Manually inject the already-serialized message into the serialized request json object.
		TSharedPtr<FJsonValueObject> RequestJsonValue = MakeShared<FJsonValueObject>(MoveTemp(RequestJsonObj));
		if (Request.Message != nullptr)
		{
			TSharedPtr<FJsonValueObject> MessageJsonValue = MakeShared<FJsonValueObject>(Request.Message);
			RequestJsonValue->AsObject()->SetField(TEXT("message"), MoveTemp(MessageJsonValue));
		}
		if (Request.ProtocolType == EPragmaProtocolType::Http)
		{
			RequestJsonValue->AsObject()->SetStringField(TEXT("httpVerb"), Request.Message != nullptr ? TEXT("POST") : TEXT("GET"));
		}
		RequestJsonValues.Emplace(MoveTemp(RequestJsonValue));
	}
	TSharedPtr<FJsonObject> PayloadJsonObj = PragmaJson::StructToJsonObj(Payload);
	PayloadJsonObj->SetArrayField(TEXT("requests"), MoveTemp(RequestJsonValues));
	return PragmaJson::JsonObjToStr(PayloadJsonObj.ToSharedRef());
}

FString SerializeMessages(const FPragmaDebugServerMessages& Payload, const TArray<FPragmaDebugMessageInfo>& MessageInfos)
{
	TArray<TSharedPtr<FJsonValue>> ResponseJsonValues;
	for (const auto& MessageInfo : MessageInfos)
	{
		TSharedPtr<FJsonObject> MessageInfoJsonObj = PragmaJson::StructToJsonObj(MessageInfo);
		if (MessageInfoJsonObj == nullptr)
		{
			PRAGMA_LOG(Error, "PragmaActiveDebugServer.cpp::SerializeResponses -- Unable to convert Response struct to json object.");
			continue;
		}
		// Manually inject the already-serialized payload into the serialized response json object.
		TSharedPtr<FJsonValueObject> MessageInfoJsonValue = MakeShared<FJsonValueObject>(MoveTemp(MessageInfoJsonObj));
		if (MessageInfo.Message != nullptr)
		{
			TSharedPtr<FJsonValueObject> MessageJsonValue = MakeShared<FJsonValueObject>(MessageInfo.Message);
			MessageInfoJsonValue->AsObject()->SetField(TEXT("message"), MoveTemp(MessageJsonValue));
		}
		ResponseJsonValues.Emplace(MoveTemp(MessageInfoJsonValue));
	}
	TSharedPtr<FJsonObject> PayloadJsonObj = PragmaJson::StructToJsonObj(Payload);
	PayloadJsonObj->SetArrayField(TEXT("messages"), MoveTemp(ResponseJsonValues));
	return PragmaJson::JsonObjToStr(PayloadJsonObj.ToSharedRef());
}

void InjectCorsHeaders(FHttpServerResponse* Response)
{
	check(Response != nullptr);
	Response->Headers.Add(TEXT("Access-Control-Allow-Origin"), { TEXT("*") });
	Response->Headers.Add(TEXT("Access-Control-Allow-Methods"), { TEXT("PUT, POST, GET, OPTIONS") });
	Response->Headers.Add(TEXT("Access-Control-Allow-Headers"), { TEXT("Origin, X-Requested-With, Content-Type, Accept") });
}

TUniquePtr<FHttpServerResponse> CreateHttpResponse(const FString& Content)
{
	auto Response = FHttpServerResponse::Create(
            *Content,
            TEXT("application/json")
        );
	InjectCorsHeaders(Response.Get());
	return Response;
}
TUniquePtr<FHttpServerResponse> CreateError(const EHttpServerResponseCodes Status, const FString& Error)
{
	auto Response = FHttpServerResponse::Error(Status, Error);
	InjectCorsHeaders(Response.Get());
	return Response;
}

int64 GetTimeNow() { return static_cast<int64>(FPlatformTime::Seconds() * 1000.f); }

FString GetBodyAsString(const FHttpServerRequest& Request)
{
	TArray<uint8> Data;
	Data.Reserve(Request.Body.Num() + 1);
	Data = Request.Body;
	// Null terminate.
	Data.AddZeroed(1);
	return FString{FUTF8ToTCHAR((ANSICHAR*)Data.GetData()).Get()};
}
}

FPragmaActiveDebugServer::FPragmaActiveDebugServer()
{
	InitStructJsonMap();
}

FPragmaActiveDebugServer::~FPragmaActiveDebugServer()
{
	UnbindAllEndpoints();
}

void FPragmaActiveDebugServer::Init(const UPragmaSdkConfig* SdkConfig, UPragmaConnection* InConnection)
{
	Connection = InConnection;

	const int32 Port = (SdkConfig != nullptr ? SdkConfig->GetDebugServerPort() : 9999);
	PRAGMA_LOG(Log, "FPragmaActiveDebugServer -- Starting debug server at port: %d", Port);

	TWeakPtr<FPragmaActiveDebugServer> WeakThisPtr{AsShared()};
	auto Router = FHttpServerModule::Get().GetHttpRouter(Port);
	TArray<FHttpRouteHandle> RouteHandles;

	//
	// OPTIONS requests for all endpoints.
	//
	RouteHandles.Add(Router->BindRoute(FHttpPath(TEXT("/v1")), EHttpServerRequestVerbs::VERB_OPTIONS,
    [this](const FHttpServerRequest&, const FHttpResultCallback& OnComplete)
	{
	    TUniquePtr<FHttpServerResponse> Response = MakeUnique<FHttpServerResponse>();
	    Response->Code = EHttpServerResponseCodes::Ok;
	    InjectCorsHeaders(Response.Get());
	    OnComplete(MoveTemp(Response));
	    return true;
	}));

	//
	// GET /v1/types/{<typename>}
	// Returns a default-initialized struct serialized to json of the type {<typename>}.
	//
	RouteHandles.Add(Router->BindRoute(FHttpPath{TEXT("/v1/types")}, EHttpServerRequestVerbs::VERB_GET,
		[WeakThisPtr](const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
		{
			const auto SharedThis = WeakThisPtr.Pin();
			if (!SharedThis.IsValid()) { return false; }
			return SharedThis->HandleV1Types(Request, OnComplete);
		}
	));

	//
	// GET /v1/requests
	// Returns a list of recent requests.
	//
	RouteHandles.Add(Router->BindRoute(FHttpPath{TEXT("/v1/requests")}, EHttpServerRequestVerbs::VERB_GET,
        [WeakThisPtr](const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
        {
            const auto SharedThis = WeakThisPtr.Pin();
            if (!SharedThis.IsValid()) { return false; }
            return SharedThis->HandleV1Requests(Request, OnComplete);
        }
    ));

	//
	// GET /v1/messages
	// Returns a list of recent messages.
	//
	RouteHandles.Add(Router->BindRoute(FHttpPath{TEXT("/v1/messages")}, EHttpServerRequestVerbs::VERB_GET,
        [WeakThisPtr](const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
        {
            const auto SharedThis = WeakThisPtr.Pin();
            if (!SharedThis.IsValid()) { return false; }
            return SharedThis->HandleV1Messages(Request, OnComplete);
        }
    ));

	//
	// GET /v1/mock-enabled
	// Returns if the server is in mocked mode.
	//
	RouteHandles.Add(Router->BindRoute(FHttpPath{TEXT("/v1/mock-enabled")}, EHttpServerRequestVerbs::VERB_GET,
        [WeakThisPtr](const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
        {
            const auto SharedThis = WeakThisPtr.Pin();
            if (!SharedThis.IsValid()) { return false; }
            return SharedThis->HandleV1GetMockEnabled(Request, OnComplete);
        }
    ));

	//
	// PUT /v1/mock-enabled
	// Set the mocked mode of the server. True will tell it to ignore any server messages.
	//
	RouteHandles.Add(Router->BindRoute(FHttpPath{TEXT("/v1/set-mock-enabled")}, EHttpServerRequestVerbs::VERB_PUT,
        [WeakThisPtr](const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
        {
            const auto SharedThis = WeakThisPtr.Pin();
            if (!SharedThis.IsValid()) { return false; }
            return SharedThis->HandleV1SetMockEnabled(Request, OnComplete);
        }
    ));

	//
	// POST /v1/mock/{social,game}/{http,websocket,http-response}/{message}
	// Send a mocked message to the server.
	//
	RouteHandles.Add(Router->BindRoute(FHttpPath{TEXT("/v1/mock")}, EHttpServerRequestVerbs::VERB_POST,
        [WeakThisPtr](const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
        {
            const auto SharedThis = WeakThisPtr.Pin();
            if (!SharedThis.IsValid()) { return false; }
            return SharedThis->HandleV1MockMessage(Request, OnComplete);
        }
    ));

	UnbindAllEndpoints = [Router, RouteHandles]()
	{
		if (!FModuleManager::Get().IsModuleLoaded("HTTPServer") || !Router.IsValid())
		{
			return;
		}
		for (auto& Handle : RouteHandles)
		{
			if (Handle.IsValid())
			{
				Router->UnbindRoute(Handle);
			}
		}
	};

	FHttpServerModule::Get().StartAllListeners();
}

void FPragmaActiveDebugServer::RequestStarted(const EPragmaBackendType BackendType, const EPragmaProtocolType ProtocolType, const FString& RequestTypeName, const FString& ResponseTypeName)
{
	CurrentRequest = MakeUnique<FPragmaDebugRequestInfo>(
		FPragmaDebugRequestInfo{GetTimeNow(), BackendType, ProtocolType, RequestTypeName, ResponseTypeName}
	);
}
void FPragmaActiveDebugServer::RequestProcessed(const bool bSuccess, const TSharedPtr<FJsonObject>& Message)
{
	if (CurrentRequest == nullptr)
	{
		return;
	}
	CurrentRequest->State = bSuccess ? EPragmaDebugMessageInfoState::ProcessedSuccess : EPragmaDebugMessageInfoState::ProcessedFailure;
	CurrentRequest->Message = Message;
	Requests.Add(*CurrentRequest);
	if (Requests.Num() > MessageHistorySize)
	{
		// TODO not efficient.
		Requests.RemoveAt(0);
	}
	CurrentRequest.Reset();
}

void FPragmaActiveDebugServer::MessageReceived()
{
	CurrentMessage = MakeUnique<FPragmaDebugMessageInfo>(
        FPragmaDebugMessageInfo{GetTimeNow()}
    );
}
void FPragmaActiveDebugServer::MessageFailedToDeserialize(const FString& MessageJsonStr)
{
	if (CurrentMessage == nullptr)
	{
		return;
	}
	CurrentMessage->State = EPragmaDebugMessageInfoState::ProcessedFailure;
	CurrentMessage->ProcessingError = MessageJsonStr;
	Messages.Add(*CurrentMessage);
	if (Messages.Num() > MessageHistorySize)
	{
		// TODO not efficient.
		Messages.RemoveAt(0);
	}
	CurrentMessage.Reset();
}
void FPragmaActiveDebugServer::MessageProcessed(const int32 SequenceNumber, const bool bSuccessfullyProcessed, const TSharedPtr<FJsonObject>& MessageJsonObject)
{
	if (CurrentMessage == nullptr)
	{
		return;
	}
	// TODO remove once sequence number issue on BE is fixed. These are just used for list keys on debug console.
	CurrentMessage->SequenceNumber = SequenceNumber == 0 ? NextFakeSeqNumber++ : SequenceNumber;
	CurrentMessage->State = bSuccessfullyProcessed ? EPragmaDebugMessageInfoState::ProcessedSuccess : EPragmaDebugMessageInfoState::ProcessedFailure;
	CurrentMessage->Message = MessageJsonObject;
	Messages.Add(*CurrentMessage);
	if (Messages.Num() > MessageHistorySize)
	{
		// TODO not efficient.
		Messages.RemoveAt(0);
	}
	CurrentMessage.Reset();
}

bool FPragmaActiveDebugServer::HandleV1Types(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete) const
{
	TArray<FString> PathTokens;
	Request.RelativePath.ParsePathTokens(PathTokens);
	if (PathTokens.Num() != 1)
	{
		OnComplete(CreateError(
			EHttpServerResponseCodes::NotFound,
			TEXT("Invalid path. Expected: /v1/types/{<typename>}.")
        ));
		return true;
	}

	const FString& StructType = PathTokens[0];
	const FString* Json = StructJsonMap.Find(StructType);
	if (Json == nullptr)
	{
		OnComplete(CreateError(
            EHttpServerResponseCodes::NotFound,
            FString::Printf(TEXT("Could not find data for the type %s. The type may be invalid, or it"
            	" may need to be added to InitStructJsonMap() in the Pragma Unreak SDK Debug Server code."), *StructType)
        ));
		return true;
	}
	OnComplete(CreateHttpResponse(*Json));
	return true;
}

bool FPragmaActiveDebugServer::HandleV1Requests(const FHttpServerRequest&, const FHttpResultCallback& OnComplete) const
{
	OnComplete(CreateHttpResponse(SerializeRequests(FPragmaDebugRequests{}, Requests)));
	return true;
}

bool FPragmaActiveDebugServer::HandleV1Messages(const FHttpServerRequest&, const FHttpResultCallback& OnComplete) const
{
	OnComplete(CreateHttpResponse(SerializeMessages(FPragmaDebugServerMessages{}, Messages)));
	return true;
}

bool FPragmaActiveDebugServer::HandleV1GetMockEnabled(const FHttpServerRequest&, const FHttpResultCallback& OnComplete) const
{
	OnComplete(CreateHttpResponse(bMockEnabled ? TEXT("true") : TEXT("false")));
	return true;
}

bool FPragmaActiveDebugServer::HandleV1SetMockEnabled(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
{
	const bool bPreviousMockEnabled = bMockEnabled;
	bMockEnabled = GetBodyAsString(Request) == "true";
	if (bMockEnabled != bPreviousMockEnabled)
	{
		PRAGMA_LOG(Warning, "DEBUG SERVER -- Backend Mocking is now %s", bMockEnabled ? TEXT("enabled") : TEXT("disabled"));
	}
	auto Response = FHttpServerResponse::Create(FString{}, TEXT("text/plain"));
	InjectCorsHeaders(Response.Get());
	OnComplete(MoveTemp(Response));
	return true;
}

bool FPragmaActiveDebugServer::HandleV1MockMessage(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete) const
{
	if (!bMockEnabled)
	{
		OnComplete(CreateError(EHttpServerResponseCodes::Forbidden, TEXT("Mock Backend must be enabled before sending mocked messages.")));
		return true;
	}
	TArray<FString> PathTokens;
	Request.RelativePath.ParsePathTokens(PathTokens);
	if (PathTokens.Num() != 3
		|| (PathTokens[0] != TEXT("social") && PathTokens[0] != TEXT("game"))
		|| (PathTokens[1] != TEXT("http") && PathTokens[1] != TEXT("websocket") && PathTokens[1] != TEXT("http-response")))
	{
		OnComplete(CreateError(
            EHttpServerResponseCodes::NotFound,
            TEXT("Invalid path. Expected: /v1/mock/{social,game}/{http,websocket,http-response}/{message}.")
        ));
		return true;
	}
	const EPragmaBackendType BackendType = PathTokens[0] == TEXT("social")
        ? EPragmaBackendType::Social
        : EPragmaBackendType::Game;
	if (PathTokens[1] == TEXT("http-response"))
	{
		// Generic http response, NOT a message.
		const int32 RequestId = FCString::Atoi(*PathTokens[2]);
		const FString MessageJsonStr = GetBodyAsString(Request);
		FPragmaHttp& Http = (Connection->GetInitializerHttp() != nullptr)
			? *Connection->GetInitializerHttp()
			: Connection->Http(BackendType);
		Http.HandleHttpResponse(RequestId, MessageJsonStr);
		OnComplete(CreateHttpResponse(TEXT("")));
		return true;
	}
	const EPragmaProtocolType Protocol = PathTokens[0] == TEXT("http")
        ? EPragmaProtocolType::Http
        : EPragmaProtocolType::WebSocket;
	const FString MessageJsonStr = GetBodyAsString(Request);
	Connection->Protocol(Protocol, BackendType).JsonMessageReceived(MessageJsonStr);
	OnComplete(CreateHttpResponse(TEXT("")));
	return true;
}

void FPragmaActiveDebugServer::InitStructJsonMap()
{
	// TODO This is a temp solution to make it easy to get defaulted structs for the debug server.
	InitStructJson(FPragmaInfoV1Response{});
	InitStructJson(FPragmaTypesV1Response{});
	InitStructJson(FPragma_Account_AuthenticateOrCreateV2Request{});
	InitStructJson(FPragma_Account_AuthenticateOrCreateV2Response{});
	InitStructJson(FPragma_Account_GetPragmaPlayerIdForDisplayNameV1Request{});
	InitStructJson(FPragma_Account_GetPragmaPlayerIdForDisplayNameV1Response{});
	InitStructJson(FPragma_Account_GetDisplayNameForPragmaPlayerIdV1Request{});
	InitStructJson(FPragma_Account_GetDisplayNameForPragmaPlayerIdV1Response{});
}
#endif // if !UE_BUILD_SHIPPING
