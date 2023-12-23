#pragma once
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Runtime/Launch/Resources/Version.h"
#include "PragmaLog.h"

// Simple way to declare a method not implemented in the mock and return some default.
// Just to warn when writing tests against it that it isn't going be useful.
#define NOT_IMPLEMENTED(MethodName) \
	PRAGMA_LOG(Error, "Mocked method " #MethodName " is not implemented.");
#define NOT_IMPLEMENTED_RETVAL(MethodName, RetVal) \
	PRAGMA_LOG(Error, "Mocked method " #MethodName " is not implemented."); \
	return RetVal;

namespace Pragma
{
namespace Tests
{

// Mock an HttpResponse by giving it a response code and string content directly.
class FMockHttpResponse final : public IHttpResponse
{
	public:
	FMockHttpResponse() {}
	FMockHttpResponse(const int32 ResponseCode, FString Content)
        : MockResponseCode(ResponseCode)
        , MockContent(MoveTemp(Content)) {}
	virtual ~FMockHttpResponse() {}

	virtual int32 GetResponseCode() const override { return MockResponseCode; }
	virtual FString GetContentAsString() const override { return MockContent; }

	// IHttpBase impl.
	virtual FString GetURL() const override { NOT_IMPLEMENTED_RETVAL(FMockHttpResponse::GetURL, FString{}); }
	virtual FString GetURLParameter(const FString& ParameterName) const override { NOT_IMPLEMENTED_RETVAL(FMockHttpResponse::GetURLParameter, FString{}); }
	virtual FString GetHeader(const FString& HeaderName) const override { NOT_IMPLEMENTED_RETVAL(FMockHttpResponse::GetHeader, FString{}); }
	virtual TArray<FString> GetAllHeaders() const override { NOT_IMPLEMENTED_RETVAL(FMockHttpResponse::GetAllHeaders, TArray<FString>{}); }
	virtual FString GetContentType() const override { return "application/json"; }
#if ENGINE_MAJOR_VERSION < 5 || ENGINE_MINOR_VERSION < 3
    virtual int32 GetContentLength() const override
#else
    virtual uint64 GetContentLength() const override
#endif
    { return MockContent.Len(); }
	virtual const TArray<uint8>& GetContent() const override { NOT_IMPLEMENTED_RETVAL(FMockHttpResponse::GetContent, FakeBytes); }

	int32 MockResponseCode{ EHttpResponseCodes::Ok };
	FString MockContent;
	private:
	TArray<uint8> FakeBytes;
};

typedef TSharedPtr<FMockHttpResponse, ESPMode::ThreadSafe> FMockHttpResponsePtr;

// Mock an HttpRequest by telling it what to result in.
//
// Modify MockRequestStatus and MockResponse member variables, then trigger the completion delegate with MockComplete().
class FMockHttpRequest final : public IHttpRequest
{
public:
	explicit FMockHttpRequest(FMockHttpResponsePtr MockResponse)
        : MockResponse(MoveTemp(MockResponse))
	{}
	virtual ~FMockHttpRequest() {}

	// IHttpRequest impl.
	virtual FString GetVerb() const override { return Verb; }
	virtual void SetVerb(const FString& InVerb) override { Verb = InVerb; }
	virtual void SetURL(const FString& InUrl) override { Url = InUrl; }
	virtual void SetContent(const TArray<uint8>& ContentPayload) override { NOT_IMPLEMENTED(FMockHttpRequest::SetContent); }
	virtual void SetContentAsString(const FString& ContentString) override { /* Ok to call but not Get. */ }
	virtual bool SetContentAsStreamedFile(const FString& Filename) override { NOT_IMPLEMENTED_RETVAL(FMockHttpRequest::SetContentAsStreamedFile, true); }
	virtual bool SetContentFromStream(TSharedRef<FArchive, ESPMode::ThreadSafe> Stream) override { NOT_IMPLEMENTED_RETVAL(FMockHttpRequest::SetContentFromStream, true); }
	virtual void SetHeader(const FString& HeaderName, const FString& HeaderValue) override { /* Ok to call but not Get. */ }
	virtual void AppendToHeader(const FString& HeaderName, const FString& AdditionalHeaderValue) override { NOT_IMPLEMENTED(FMockHttpRequest::AppendToHeader); }
	virtual bool ProcessRequest() override
	{
		MockProcessRequestCalled = true;
		const bool bSuccess = (MockRequestStatus == EHttpRequestStatus::Type::Succeeded);
		HttpRequestCompleteDelegate.ExecuteIfBound(AsShared(), MockResponse, bSuccess);
		return bSuccess;
	}

	virtual FHttpRequestCompleteDelegate& OnProcessRequestComplete() override { return HttpRequestCompleteDelegate; }
	virtual FHttpRequestProgressDelegate& OnRequestProgress() override { NOT_IMPLEMENTED_RETVAL(FMockHttpRequest::GetURLParameter, HttpRequestProgressDelegate); }
	virtual FHttpRequestHeaderReceivedDelegate& OnHeaderReceived() override { NOT_IMPLEMENTED_RETVAL(FMockHttpRequest::GetURLParameter, HttpRequestHeaderReceivedDelegate); }
	virtual void CancelRequest() override { NOT_IMPLEMENTED(FMockHttpRequest::CancelRequest); }
	virtual EHttpRequestStatus::Type GetStatus() const override { return MockRequestStatus; }
	virtual const FHttpResponsePtr GetResponse() const override { return MockResponse; }
	virtual void Tick(float DeltaSeconds) override {}
	virtual float GetElapsedTime() const override { NOT_IMPLEMENTED_RETVAL(FMockHttpRequest::GetElapsedTime, 0.f); }
#if ENGINE_MAJOR_VERSION > 4 || ENGINE_MINOR_VERSION >= 26
	virtual void SetContent(TArray<uint8>&& ContentPayload) override { NOT_IMPLEMENTED(FMockHttpRequest::SetContent); }
	virtual void SetTimeout(float InTimeoutSecs) override { NOT_IMPLEMENTED(FMockHttpRequest::SetTimeout) }
	virtual void ClearTimeout() override { NOT_IMPLEMENTED(FMockHttpRequest::ClearTimeout) }
	virtual TOptional<float> GetTimeout() const override { NOT_IMPLEMENTED_RETVAL(FMockHttpRequest::GetTimeout, TOptional<float>()) }
#endif
#if ENGINE_MAJOR_VERSION > 4 || ENGINE_MINOR_VERSION >= 25
	virtual FHttpRequestWillRetryDelegate& OnRequestWillRetry() override { return HttpRequestWillRetryDelegate; }
#endif

	// IHttpBase impl.
	virtual FString GetURL() const override { return Url; }
	virtual FString GetURLParameter(const FString& ParameterName) const override { NOT_IMPLEMENTED_RETVAL(FMockHttpResponse::GetURLParameter, FString{}); }
	virtual FString GetHeader(const FString& HeaderName) const override { NOT_IMPLEMENTED_RETVAL(FMockHttpResponse::GetHeader, FString{}); }
	virtual TArray<FString> GetAllHeaders() const override { NOT_IMPLEMENTED_RETVAL(FMockHttpResponse::GetAllHeaders, TArray<FString>{}); }
	virtual FString GetContentType() const override { return "application/json"; }
#if ENGINE_MAJOR_VERSION < 5 || ENGINE_MINOR_VERSION < 3
    virtual int32 GetContentLength() const override
#else
    virtual uint64 GetContentLength() const override
#endif
	{ NOT_IMPLEMENTED_RETVAL(IHttpRequest::GetContentLength, 0); }
	virtual const TArray<uint8>& GetContent() const override { NOT_IMPLEMENTED_RETVAL(IHttpRequest::GetContent, FakeBytes); }

#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION > 2
    virtual bool SetResponseBodyReceiveStream(TSharedRef<FArchive> Stream) override { NOT_IMPLEMENTED_RETVAL(FMockHttpResponse::SetResponseBodyReceiveStream, bool{}); }
    virtual void SetDelegateThreadPolicy(EHttpRequestDelegateThreadPolicy InThreadPolicy) override { NOT_IMPLEMENTED(FMockHttpResponse::SetDelegateThreadPolicy); }
    virtual EHttpRequestDelegateThreadPolicy GetDelegateThreadPolicy() const override { NOT_IMPLEMENTED_RETVAL(FMockHttpResponse::GetDelegateThreadPolicy, EHttpRequestDelegateThreadPolicy{}); }
#endif

	// Mocks.
	bool WasProcessRequestCalled() const { return MockProcessRequestCalled; }
	EHttpRequestStatus::Type MockRequestStatus{ EHttpRequestStatus::Type::Succeeded };
	FMockHttpResponsePtr MockResponse;

private:
	FHttpRequestCompleteDelegate HttpRequestCompleteDelegate;
	FHttpRequestProgressDelegate HttpRequestProgressDelegate;
	FHttpRequestHeaderReceivedDelegate HttpRequestHeaderReceivedDelegate;
#if ENGINE_MAJOR_VERSION > 4 || ENGINE_MINOR_VERSION >= 25
	FHttpRequestWillRetryDelegate HttpRequestWillRetryDelegate;
#endif
	TArray<uint8> FakeBytes;

	bool MockProcessRequestCalled{ false };
	FString Verb;
	FString Url;

	template<typename T>
	void asdf() {}
};


template<>
inline void FMockHttpRequest::asdf<int>() {}

}
}

#undef NOT_IMPLEMENTED
#undef NOT_IMPLEMENTED_RETVAL
