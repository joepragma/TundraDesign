#include "PragmaHttp.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Debug/PragmaDebugServer.h"

#include "PragmaLog.h"

namespace
{
TOptional<FPragmaError> GetResponseError(const FString& LogPrefix, const FHttpRequestPtr Request,
	const FHttpResponsePtr Response, const bool bConnectedSuccessfully)
{
	if (!Request.IsValid())
	{
		PRAGMA_LOG(Error, "%s failed: Request object is no longer valid.", *LogPrefix);
		return FPragmaError{EPragma_SdkError::RequestHandlerExpired};
	}
	if (!bConnectedSuccessfully || !Response.IsValid())
	{
		// Unable to connect, will not have a Response object.
		PRAGMA_LOG(Error, "%s failed: http connection failed: %s.",
			*LogPrefix, EHttpRequestStatus::ToString(Request->GetStatus()));
		return FPragmaError{EPragma_SdkError::PlatformUnavailable};
	}
	if (!EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		// Request went through successfully, but failed for another reason.
		auto Error = FPragmaError{EPragma_SdkError::Unknown};
		switch (Response->GetResponseCode())
		{
		case EHttpResponseCodes::Denied:			// 401
		case EHttpResponseCodes::Forbidden:			// 403
			Error = EPragma_SdkError::Unauthorized;
			break;
			
		case EHttpResponseCodes::NotFound:			// 404
			Error = EPragma_SdkError::NotFound;
			break;
			
		case EHttpResponseCodes::ServerError:		// 500
			Error = EPragma_SdkError::GenericPlatformError;
			break;
			
		case EHttpResponseCodes::BadGateway: 		// 502
		case EHttpResponseCodes::ServiceUnavail: 	// 503
		case EHttpResponseCodes::GatewayTimeout: 	// 504
			Error = EPragma_SdkError::PlatformUnavailable;
			break;
			
		default:
			break;
		}
		PRAGMA_LOG(Error, "%s failed, error: %s. Server returned http status code: %d, payload: %s",
			*LogPrefix, *Error.ToString(), Response->GetResponseCode(), *Response->GetContentAsString());
		return Error;
	}
	return {};
}
}

FPragmaHttp::FPragmaHttp(const FCreateHttpRequest CreateHttpRequestDelegate)
	: FPragmaProtocol(EPragmaBackendType::Game)
{
	CreateHttpRequest = CreateHttpRequestDelegate;
}

void FPragmaHttp::Init(FString InBaseUrl, FString InApiUrl)
{
	if (!InBaseUrl.IsEmpty())
	{
		// Remove trailing slash if any so endpoints are specified as /*.
		BaseUrl = InBaseUrl.EndsWith(TEXT("/"))
			? InBaseUrl.LeftChop(1)
			: MoveTemp(InBaseUrl);
	}
	if (!InApiUrl.IsEmpty())
	{
		ApiUrl = MoveTemp(InApiUrl);
	}
	if (BaseUrl.IsEmpty() && ApiUrl.IsEmpty())
	{
		PRAGMA_LOG(Error, "FPragmaHttp::Init -- At least one of BaseUrl or ApiUrl must be set in Init."
			" This is probably a configuration error.");
		return;
	}
	PRAGMA_LOG(Log, "FPragmaHttp::Init -- Initialized with BaseUrl %s and ApiUrl %s.", *BaseUrl, *ApiUrl);
}

void FPragmaHttp::SetAuthToken(FString AuthToken)
{
	AuthorizationToken = MoveTemp(AuthToken);
}

FString FPragmaHttp::GetAuthToken() const
{
	return FString(AuthorizationToken);
}

void FPragmaHttp::DebugRequestStarted(const FString& RequestTypeName, const FString& ResponseTypeName) const
{
	if (GetDebugServer() != nullptr)
	{
		GetDebugServer()->RequestStarted(GetBackendType(), EPragmaProtocolType::Http, RequestTypeName, ResponseTypeName);
	}
}

void FPragmaHttp::DebugHttpRequestStartedSuccessfully(const RequestId RequestId, TSharedPtr<FJsonObject> Message) const
{
	static FString RequestIdName = TEXT("requestId");
	if (Message == nullptr)
	{
		// Make one to shove the requestId in.
		Message = MakeShared<FJsonObject>();
	}
	if (!Message->HasField(RequestIdName))
	{
		// Http requests outside of the message context don't have requestIds. For the sake of debug, we inject one.
		Message->SetNumberField(RequestIdName, RequestId);
	}
	static FString TypeName = TEXT("type");
	static FString HttpDebugType = TEXT("HTTP");
	if (!Message->HasField(TypeName))
	{
		// Http requests outside of the message context don't have types. For the sake of debug, we inject one.
		Message->SetStringField(TypeName, HttpDebugType);
	}
	DebugRequestStartedSuccessfully(Message);
}

void FPragmaHttp::DebugHttpMessageProcessed(
	const RequestId RequestId,
	const bool bSuccessfullyProcessed,
	TSharedPtr<FJsonObject> MessageJsonObject) const
{
	if (MessageJsonObject != nullptr)
	{
		// We're going to modify the object, so copy the json content to a new object.
		MessageJsonObject = MakeShared<FJsonObject>(*MessageJsonObject);
	}
	else
	{
		// Make one to shove the requestId in.
		MessageJsonObject = MakeShared<FJsonObject>();
	}
	static FString RequestIdName = TEXT("requestId");
	if (!MessageJsonObject->HasField(RequestIdName))
	{
		// Http requests outside of the message context don't have requestIds. For the sake of debug, we inject one.
		MessageJsonObject->SetNumberField(RequestIdName, RequestId);
	}
	// Sequence Number doesn't exist for non-message calls, so give it something related and unique.
	DebugMessageProcessed(RequestId * 100000, bSuccessfullyProcessed, MessageJsonObject);
}

bool FPragmaHttp::CanSendMessage() const
{
	return !ApiUrl.IsEmpty();
}

bool FPragmaHttp::SendJsonMessage(const FString& Message, const FSendFailure& SendFailure)
{
	if (ApiUrl.IsEmpty())
	{
		PRAGMA_LOG(Error, "FPragmaHttp::SendJsonMessage -- ApiUrl is empty, cannot send messages. See Init(). ");
		return false;
	}
	// All API messages use POST.
	static const FString Verb = TEXT("POST");
	static const FString Payload = FString{};
	auto Request = CreateRequest(Verb, ApiUrl, Payload);
	Request->SetContentAsString(Message);
	TWeakPtr<FPragmaHttp> WeakThisTemp = AsShared();
	Request->OnProcessRequestComplete().BindLambda([WeakThisTemp, SendFailure](const FHttpRequestPtr RequestPtr, const FHttpResponsePtr ResponsePtr, const bool bConnectedSuccessfully)
	{
		if (auto This = WeakThisTemp.Pin())
		{
			This->HandleMessageResponse(RequestPtr, ResponsePtr, bConnectedSuccessfully, SendFailure);
		}
	});
	if (!Request->ProcessRequest())
	{
		PRAGMA_LOG(Error, "FPragmaHttp::SendJsonMessage -- Failed to start http request.");
		return false;
	}
	return true;
}

void FPragmaHttp::HandleMessageResponse(const FHttpRequestPtr Request, const FHttpResponsePtr Response, const bool bConnectedSuccessfully, const FSendFailure& SendFailure)
{
	if (GetDebugServer() != nullptr && GetDebugServer()->GetMockEnabled())
	{
		return;
	}
	FString Content;
	const auto Error = GetResponseError(FString::Printf(TEXT("FPragmaHttp::HandleResponse(%s)"), *Request->GetURL()), Request, Response, bConnectedSuccessfully);
	if (Error.IsSet())
	{
		auto _ = SendFailure.ExecuteIfBound(Error.GetValue());
		return;
	}
	PRAGMA_LOG(Verbose, "FPragmaHttp::HandleResponse(%s) success: payload: %s", *Request->GetURL(), *Response->GetContentAsString());
	JsonMessageReceived(Response->GetContentAsString());
}

void FPragmaHttp::HttpRequest(const FString& Verb, const FString& EndPoint, const TSharedPtr<FJsonObject>& PayloadJsonObj, TUniqueFunction<void(const TPragmaResult<TSharedPtr<FJsonObject>>&)> OnComplete)
{
	if (BaseUrl.IsEmpty())
	{
		DebugRequestFailedToStart();
		PRAGMA_LOG(Error, "FPragmaHttp::HttpRequest -- Must set BaseUrl via Init() before attempting an http request.");
		OnComplete(TPragmaResult<TSharedPtr<FJsonObject>>::Failure());
		return;
	}
	const FString Format = EndPoint.StartsWith("/") ? "{0}{1}" : "{0}/{1}";
	const FString Url = FString::Format(*Format, {*BaseUrl, *EndPoint});
	FString PayloadJson;
	if (PayloadJsonObj != nullptr)
	{
		PayloadJson = PragmaJson::JsonObjToStr(PayloadJsonObj.ToSharedRef());
	}
	auto Request = CreateRequest(Verb, Url, PayloadJson);

	// Non-message http requests don't have request-id semantics that messages do. In the default flow they don't need
	// it, but when the debug server is running we want it to be able to track the request-response pair, and also to
	// inject a mocked message into the http process.
	//
	// To do this, we create a request id as we would a normal Pragma message, but we only use it in relation to the
	// debug server.
	const int RequestId = GetNextRequestId();
	PRAGMA_LOG(Verbose, "FPragmaHttp::HttpRequest(%s: %s) Request ID %d: Sending with payload: %s", *Verb, *Url, RequestId, *PayloadJson);

	// Either the real http response or the mock http response will use this info.
	HttpResponseHandlers.Add(RequestId, FResponseHandlerInfo{Url, MoveTemp(OnComplete)});

	// Normal flow will respond to the handler from the HttpRequest complete callback.
	if (GetDebugServer() == nullptr || !GetDebugServer()->GetMockEnabled())
	{
		TWeakPtr<FPragmaHttp> WeakThisTemp = this->AsShared();
		Request->OnProcessRequestComplete().BindLambda([WeakThisTemp, RequestId](const FHttpRequestPtr Request, const FHttpResponsePtr Response, const bool bConnectedSuccessfully)
		{
			const auto SharedThis = WeakThisTemp.Pin();
			if (!SharedThis.IsValid())
			{
				// Protect from this lambda firing during/after session shutdown.
				return;
			}
			const FResponseHandlerInfo* Info = SharedThis->HttpResponseHandlers.Find(RequestId);
			if (Info == nullptr)
			{
				return;
			}
			SharedThis->DebugMessageReceived();
			const auto Error = GetResponseError(FString::Printf(TEXT("FPragmaHttp::HttpRequest(%s)"), *Info->Url), Request, Response, bConnectedSuccessfully);
			if (Error.IsSet())
			{
				SharedThis->DebugHttpMessageProcessed(RequestId, false, nullptr);
				Info->OnComplete(TPragmaResult<TSharedPtr<FJsonObject>>::Failure(Error.GetValue()));
				SharedThis->HttpResponseHandlers.Remove(RequestId);
				return;
			}
			SharedThis->HandleHttpResponse(RequestId, Response->GetContentAsString());
		});
	}
	if (!Request->ProcessRequest())
	{
		DebugRequestFailedToStart();
		PRAGMA_LOG(Error, "FPragmaHttp::HttpRequest(%s) -- Failed to start http request.", *EndPoint);
		const FResponseHandlerInfo* Info = HttpResponseHandlers.Find(RequestId);
		if (Info != nullptr)
		{
			Info->OnComplete(TPragmaResult<TSharedPtr<FJsonObject>>::Failure());
		}
		HttpResponseHandlers.Remove(RequestId);
		return;
	}
	DebugHttpRequestStartedSuccessfully(RequestId, PayloadJsonObj);
}

void FPragmaHttp::HandleHttpResponse(const RequestId RequestId, const FString& Json)
{
	if (!HttpResponseHandlers.Contains(RequestId))
	{
		// Already handled.
		PRAGMA_LOG(VeryVerbose, "FPragmaHttp::HttpRequest -- Already handled request ID: %d", RequestId, *Json);
		return;
	}
	const FResponseHandlerInfo Info = HttpResponseHandlers.FindAndRemoveChecked(RequestId);
	TSharedPtr<FJsonObject> JsonObject = PragmaJson::JsonStrToObj(Json);
	if (JsonObject == nullptr)
	{
		DebugHttpMessageProcessed(RequestId, false, nullptr);
		PRAGMA_LOG(Error, "FPragmaHttp::HttpRequest(%s) failed to parse response as json: %s", *Info.Url, *Json);
		Info.OnComplete(TPragmaResult<TSharedPtr<FJsonObject>>::Failure());
		return;
	}
	DebugHttpMessageProcessed(RequestId, true, JsonObject);
	PRAGMA_LOG(Verbose, "FPragmaHttp::HttpRequest(%s) successful. Response payload: %s", *Info.Url, *Json);
	Info.OnComplete(TPragmaResult<TSharedPtr<FJsonObject>>{MoveTemp(JsonObject)});
}

FPragmaHttp::IHttpRequestRef FPragmaHttp::CreateRequest(const FString& Verb, const FString& Url, const FString& PayloadJson) const
{
	IHttpRequestRef Request = CreateHttpRequest.IsBound()
		? CreateHttpRequest.Execute()
		: FHttpModule::Get().CreateRequest();
	Request->SetVerb(Verb);
	Request->SetURL(Url);
	static TArray<FString> Headers
	{
		TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"),
		TEXT("Content-Type"), TEXT("application/json"),
		TEXT("Accept"), TEXT("application/json")
	};
	checkf(Headers.Num() % 2 == 0, TEXT("Header list must have 2 entries for each header."));
	for (int i = 0; i < Headers.Num(); i += 2)
	{
		Request->SetHeader(Headers[i], Headers[i + 1]);
	}
	if (!AuthorizationToken.IsEmpty())
	{
		const FString BearerHeader = FString::Printf(TEXT("Bearer %s"), *AuthorizationToken);
		Request->SetHeader(TEXT("Authorization"), BearerHeader);
	}
	if (!PayloadJson.IsEmpty())
	{
		Request->SetContentAsString(PayloadJson);
	}
	return Request;
}
