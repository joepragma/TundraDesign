#pragma once
#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "PragmaProtocol.h"
#include "PragmaJson.h"
#include "PragmaLog.h"
#include "Runtime/Launch/Resources/Version.h"

//
// Handles communication with the Pragma backend through http.
//
class PRAGMASDK_API FPragmaHttp : public FPragmaProtocol, public TSharedFromThis<FPragmaHttp>
{
public:
#if ENGINE_MAJOR_VERSION > 4 || ENGINE_MINOR_VERSION >= 26
	static const ESPMode IHttpRequestSharedPtrEspMode = ESPMode::ThreadSafe;
#else
	static const ESPMode IHttpRequestSharedPtrEspMode = ESPMode::Fast;
#endif
	typedef TSharedPtr<IHttpRequest, IHttpRequestSharedPtrEspMode> IHttpRequestPtr;
	typedef TSharedRef<IHttpRequest, IHttpRequestSharedPtrEspMode> IHttpRequestRef;

public:
    DECLARE_DELEGATE_RetVal(IHttpRequestRef, FCreateHttpRequest);

	explicit FPragmaHttp(const EPragmaBackendType InBackendType) : FPragmaProtocol(InBackendType) {}
    // Inject a custom http request creation function. Defaults to FHttpModule::CreateRequest().
    explicit FPragmaHttp(const FCreateHttpRequest CreateHttpRequestDelegate);

	// TODO (jcole//lilith): clean this
	explicit FPragmaHttp() : FPragmaProtocol(EPragmaBackendType::Social) {}

	virtual ~FPragmaHttp() override {}
	
	virtual TWeakPtr<FPragmaProtocol> WeakThis() { return AsShared(); }

	// Initialize the protocol.
	//
    // InBaseUrl: The base URL for http traffic to the backend in the form: https://{hostname}:{port}. Must be set before using
	// Http* methods. Can be used for any http calls once set.
    //
    // InApiUrl: The URL for RPC-style http traffic to the backend in the form: https://{hostname}:{port}/{rpc_endpoint}.
	// Must be set before using SendMessage method.
	void Init(FString InBaseUrl, FString InApiUrl);

	// This token will be added as an "Authorization: Bearer %s" header for all requests made.
	void SetAuthToken(FString AuthToken);
    // Gets the auth token
    FString GetAuthToken() const;

	// Http GET a backend resource served as a normal HTTP resource, not as part of the typical Pragma RPC-style flow.
	// EndPoint will be used as: https://{BackendAddress}/{EndPoint}
	//
	// This is only necessary for a handful of endpoints.
	template <typename ResponseType>
    void HttpGet(const FString& EndPoint, TUniqueFunction<void(TPragmaResult<ResponseType>)> OnComplete)
	{
		DebugRequestStarted(TEXT(""), ResponseType::StaticStruct()->GetName());
		static const FString Verb = TEXT("GET");
		static const FString Payload{};
		HttpRequest(Verb, EndPoint, nullptr, [this, OnComplete = MoveTemp(OnComplete)](const TPragmaResult<TSharedPtr<FJsonObject>>& Result)
		{
			OnComplete(PragmaJson::JsonResultToTypedResult<ResponseType>(TEXT("FPragmaHttp::HttpGet"), Result));
		});
	}

	// Http GET a backend resource served as a normal HTTP resource, not as part of the typical Pragma RPC-style flow.
	// EndPoint will be used as: https://{BackendAddress}/{EndPoint}
	//
	// This is only necessary for a handful of endpoints.
	//
	// ResponseType is only used for debugging.
	template <typename ResponseType>
	void HttpGet(const FString& EndPoint, TUniqueFunction<void(TPragmaResult<TSharedPtr<FJsonObject>>)> OnComplete)
	{
		DebugRequestStarted(TEXT(""), ResponseType::StaticStruct()->GetName());
		static const FString Verb = TEXT("GET");
		HttpRequest(Verb, EndPoint, nullptr, [this, OnComplete = MoveTemp(OnComplete)](const TPragmaResult<TSharedPtr<FJsonObject>>& Result)
        {
            OnComplete(Result);
        });
	}

	// Http POST to a backend method not part of the typical Pragma RPC-style flow.
	// EndPoint will be used as: https://{BackendAddress}/{EndPoint}
	//
	// This is only necessary for a handful of endpoints.
	template <typename RequestType, typename ResponseType>
    void HttpPost(const FString& EndPoint, const RequestType& Payload, TUniqueFunction<void(TPragmaResult<ResponseType>)> OnComplete)
	{
		DebugRequestStarted(RequestType::StaticStruct()->GetName(), ResponseType::StaticStruct()->GetName());
		static const FString Verb = TEXT("POST");
		auto PayloadJsonObj = PragmaJson::StructToJsonObj(Payload);
		if (PayloadJsonObj == nullptr)
		{
            PRAGMA_LOG(Error, "FPragmaHttp::HttpPost -- Failed to serialize request payload. Target endpoint: %s.", *EndPoint);
			OnComplete(TPragmaResult<ResponseType>::Failure());
			return;
		}
		HttpRequest(Verb, EndPoint, PayloadJsonObj, [this, OnComplete = MoveTemp(OnComplete)](const TPragmaResult<TSharedPtr<FJsonObject>>& Result)
        {
			OnComplete(PragmaJson::JsonResultToTypedResult<ResponseType>(TEXT("FPragmaHttp::HttpPost"), Result));
        });
	}

protected:
	void DebugHttpRequestStartedSuccessfully(const RequestId RequestId, TSharedPtr<FJsonObject> Message) const;
	void DebugHttpMessageProcessed(const RequestId RequestId, const bool bSuccessfullyProcessed, TSharedPtr<FJsonObject> MessageJsonObject) const;

	// FPragmaProtocol impl.
	virtual void DebugRequestStarted(const FString& RequestTypeName, const FString& ResponseTypeName) const override;
	virtual bool CanSendMessage() const override;
	virtual bool SendJsonMessage(const FString& Message, const FSendFailure& SendFailure) override;

	typedef TUniqueFunction<void(const TPragmaResult<TSharedPtr<FJsonObject>>&)> FOnHttpRequestComplete;

private:
	void HandleMessageResponse(const FHttpRequestPtr Request, const FHttpResponsePtr Response, const bool bConnectedSuccessfully, const FSendFailure& SendFailure);
	virtual void HttpRequest(const FString& Verb, const FString& EndPoint, const TSharedPtr<FJsonObject>& PayloadJsonObj, FOnHttpRequestComplete OnComplete);
	void HandleHttpResponse(const RequestId RequestId, const FString& Json);

	IHttpRequestRef CreateRequest(const FString& Verb, const FString& Url, const FString& PayloadJson) const;

    // Injected method for creating HTTP requests.
    FCreateHttpRequest CreateHttpRequest;

    // URL at which we can access HTTP endpoints.
    FString BaseUrl;

	// URL at which we access the backend api (also available via websocket).
    FString ApiUrl;

	// Auth token added to all requests as an "Authorization: Bearer %s" header.
	FString AuthorizationToken;

	// Handlers specifically for non-pragma-message http responses.
	struct FResponseHandlerInfo
	{
		FString Url;
		FOnHttpRequestComplete OnComplete;
	};
	TMap<RequestId, FResponseHandlerInfo> HttpResponseHandlers;

#if !UE_BUILD_SHIPPING
	// For access to HandleHttpResponse to mock responses.
	friend class FPragmaActiveDebugServer;
#endif
};
