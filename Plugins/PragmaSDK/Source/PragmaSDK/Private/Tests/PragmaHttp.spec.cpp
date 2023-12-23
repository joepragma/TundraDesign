#include "CoreMinimal.h"
#include "PragmaHttp.h"
#include "TestDto.h"
#include "Tests/PragmaAutomationSpecBase.h"
#include "Tests/MockHttp.h"

namespace Pragma
{
namespace Tests
{

PRAGMA_BEGIN_DEFINE_SPEC(FPragmaHttpTest, "PragmaSDK.Http")
	FPragmaHttp::IHttpRequestPtr MockRequest;
	FMockHttpResponsePtr MockResponse;
	FPragmaHttp::IHttpRequestRef CreateMockHttpRequest()
	{
		MockRequest = MakeShared<FMockHttpRequest, FPragmaHttp::IHttpRequestSharedPtrEspMode>(MockResponse);
		return MockRequest->AsShared();
	}

	TSharedPtr<FPragmaHttp> HttpApi;
	FString ResponsePayloadStr{ "test123" };

	FString MockResponseJson() const
	{
		FString ResponseStr;
		const FPragmaTestResponsePayload Response{ ResponsePayloadStr };
		if (!PragmaJson::StructToJsonStr(Response, ResponseStr))
		{
			PRAGMA_LOG(Error, "MockResponseJson -- Failed to convert mock object to json.");
		}
		return ResponseStr;
	}

	// Convenience to wrap the HttpGet call and test if it success or not.
	void CallHttpGet_ExpectResult(const bool bExpectSuccess)
	{
        bool bCalled = false;
		HttpApi->HttpGet<FPragmaTestResponsePayload>(TEXT("TestEndpoint"),
            [this, bExpectSuccess, &bCalled](const TPragmaResult<FPragmaTestResponsePayload> Result)
        {
			bCalled = true;
            // Writing the expectations out like this produces much nicer log lines than TestEqual in this case. 
            if (bExpectSuccess)
            {
                TestTrue("Success", Result.IsSuccessful());
            }
            else
            {
                TestFalse("Success", Result.IsSuccessful());
            }
        });
		TestTrue("Called", bCalled);
	}
PRAGMA_END_DEFINE_SPEC(FPragmaHttpTest)

void FPragmaHttpTest::Define()
{
	BeforeEach([this]()
	{
		MockResponse = MakeShared<FMockHttpResponse, ESPMode::ThreadSafe>();
		HttpApi = MakeShared<FPragmaHttp>(FPragmaHttp::FCreateHttpRequest::CreateRaw(this, &FPragmaHttpTest::CreateMockHttpRequest));
	});

	Describe("HttpGet()", [this]()
	{
		It("should result in success when response is OK and it has a valid deserialized response", [this]()
		{
			HttpApi->Init(TEXT("TestUrl"), TEXT("TestUrl"));
			MockResponse->MockResponseCode = EHttpResponseCodes::Ok;
			MockResponse->MockContent = MockResponseJson();
			CallHttpGet_ExpectResult(
			    /* bExpectSuccess */ true
			);
		});
		
		It("should result in failure when no url is set", [this]()
		{
			ExpectErrorOrWarning(*this, TEXT("FPragmaHttp::HttpRequest"));
			// HttpApi->Init(TEXT("TestUrl"), TEXT("TestUrl")); -- Purposefully not initialized for test.
			MockResponse->MockResponseCode = EHttpResponseCodes::Ok;
			MockResponse->MockContent = MockResponseJson();
            CallHttpGet_ExpectResult(
                /* bExpectSuccess */ false
            );
		});
		
		It("should result in failure when response is not OK", [this]()
		{
			ExpectErrorOrWarning(*this, TEXT("FPragmaHttp::HttpRequest"));
			HttpApi->Init(TEXT("TestUrl"), TEXT("TestUrl"));
			MockResponse->MockResponseCode = EHttpResponseCodes::Denied;
			MockResponse->MockContent = MockResponseJson();
			CallHttpGet_ExpectResult(
                /* bExpectSuccess */ false
            );
		});
		
		It("should result in failure when response is OK but it has no response", [this]()
		{
			ExpectErrorOrWarning(*this, TEXT("FPragmaHttp::HttpRequest"));
			HttpApi->Init(TEXT("TestUrl"), TEXT("TestUrl"));
			MockResponse->MockResponseCode = EHttpResponseCodes::Ok;
			MockResponse->MockContent.Empty();
			CallHttpGet_ExpectResult(
                /* bExpectSuccess */ false
            );
		});

		It("should result in failure when response has invalid json", [this]()
		{
			ExpectErrorOrWarning(*this, TEXT("FPragmaHttp::HttpRequest"));
			HttpApi->Init(TEXT("TestUrl"), TEXT("TestUrl"));
			MockResponse->MockResponseCode = EHttpResponseCodes::Ok;
			MockResponse->MockContent = "{NOT VALID JSON}";
            CallHttpGet_ExpectResult(
                /* bExpectSuccess */ false
            );
		});

		It("should have a correctly combined request url when ApiUrl and Endpoint are slashless", [this]()
        {
            HttpApi->Init(TEXT("TestUrl"), TEXT("TestUrl"));
            MockResponse->MockResponseCode = EHttpResponseCodes::Ok;
            MockResponse->MockContent = MockResponseJson();
            HttpApi->HttpGet<FPragmaTestResponsePayload>((("TestEndpoint")), [](const TPragmaResult<FPragmaTestResponsePayload> Result) {});
            TestEqual("Request URL", MockRequest->GetURL(), "TestUrl/TestEndpoint");
        });

        It("should have a correctly combined request url when ApiUrl has a trailing slash", [this]()
        {
            HttpApi->Init(TEXT("TestUrl/"), TEXT("TestUrl/"));
            MockResponse->MockResponseCode = EHttpResponseCodes::Ok;
            MockResponse->MockContent = MockResponseJson();
            HttpApi->HttpGet<FPragmaTestResponsePayload>((("TestEndpoint")), [](const TPragmaResult<FPragmaTestResponsePayload> Result) {});
            TestEqual("Request URL", MockRequest->GetURL(), "TestUrl/TestEndpoint");
        });

        It("should have a correctly combined request url when Endpoint has a leading slash", [this]()
        {
            HttpApi->Init(TEXT("TestUrl"), TEXT("TestUrl"));
            MockResponse->MockResponseCode = EHttpResponseCodes::Ok;
            MockResponse->MockContent = MockResponseJson();
            HttpApi->HttpGet<FPragmaTestResponsePayload>((("/TestEndpoint")), [](const TPragmaResult<FPragmaTestResponsePayload> Result) {});
            TestEqual("Request URL", MockRequest->GetURL(), "TestUrl/TestEndpoint");
        });

        It("should have a correctly combined request url when ApiUrl and Endpoint have slashes", [this]()
        {
            HttpApi->Init(TEXT("TestUrl/"), TEXT("TestUrl/"));
            MockResponse->MockResponseCode = EHttpResponseCodes::Ok;
            MockResponse->MockContent = MockResponseJson();
            HttpApi->HttpGet<FPragmaTestResponsePayload>((("/TestEndpoint")), [](const TPragmaResult<FPragmaTestResponsePayload> Result) {});
            TestEqual("Request URL", MockRequest->GetURL(), "TestUrl/TestEndpoint");
        });
	});
}

}
}
