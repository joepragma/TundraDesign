#include "Tests/TestServer.h"

#include "Tests/AsyncTest.h"
#include "PragmaRuntime.h"
#include "PragmaServer.h"

namespace Pragma
{
namespace Tests
{

FTestServer::FTestServer(TSharedPtr<FAsyncTest> OwningTest)
	: TestPtr(MoveTemp(OwningTest))
{
	checkf(TestPtr.IsValid(), TEXT("FTestServer must be initialized with a valid TSharedPtr<FAsyncTest>"));
	TestPtr->ServerRuntime()->Config().PartnerSessionSocialAuthToken = "eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiJ9.eyJzdWIiOiIwMDAwMDAwMC0wMDAwLTAwMDAtMDAwMC0wMDAwMDAwMDAwMDAiLCJyZWZyZXNoQXRNcyI6IjQ4MTg2OTgyNzY2MzAiLCJiYWNrZW5kVHlwZSI6IlNPQ0lBTCIsImRpc3BsYXlOYW1lIjoicGFydG5lciIsInByYWdtYVNvY2lhbElkIjoiMDAwMDAwMDAtMDAwMC0wMDAwLTAwMDAtMDAwMDAwMDAwMDAwIiwiaXNzIjoicHJhZ21hIiwicmVmcmVzaEluTWlsbGlzIjoiMzE1MzYwMDAwMDAwMCIsInNlc3Npb25UeXBlIjoiUEFSVE5FUiIsImV4cCI6NDgxODY5ODI3NiwiaWF0IjoxNjY1MDk4Mjc2LCJqdGkiOiI1NjNlZjI3ZS0wNGVlLTQwMjUtYmQ1Ni1kM2E0ZDgwNTkxNTkiLCJkaXNjcmltaW5hdG9yIjoiIn0.NnqAPh0H_8PQNuHud7Wyulo6ElDo0X4NXmZ4xH-APwaUxmvaG8V2oda4XrjdM7NVHpW8NiLfziqIHEp5luBCvjcZBEd4ZqPRRyFlfFoanxWZFFPw1PJWr1IRquh8S_fRtzuWR5bsm-CSGGA9C4IfLadMGNhzDFIQTa8irVrZrivhJw_tP6ow7OLAi0SeBwxfM53TJ1mTXhNPQCwEv4N3n4j-0FPBLzqyMbgaDnEcDuXQXy2G86dUS0GAByPc9s3wMty691k9Q6Ud2PTd3GBrRJ6JVuWXzFBIOxqtvH4P9uzZImT2XsZjXkW1zSm7s2NCHRwYFT9jplnjZe_g55jwbQ";
	TestPtr->ServerRuntime()->Config().PartnerSessionGameAuthToken = "eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiJ9.eyJzdWIiOiIwMDAwMDAwMC0wMDAwLTAwMDAtMDAwMC0wMDAwMDAwMDAwMDAiLCJiYWNrZW5kVHlwZSI6IkdBTUUiLCJkaXNwbGF5TmFtZSI6InBhcnRuZXIiLCJwcmFnbWFTb2NpYWxJZCI6IjAwMDAwMDAwLTAwMDAtMDAwMC0wMDAwLTAwMDAwMDAwMDAwMCIsImlzcyI6InByYWdtYSIsInJlZnJlc2hJbk1pbGxpcyI6IjMxNTM2MDAwMDAwMDAiLCJkaXNjcmltaW5hdG9yIjoiIiwicmVmcmVzaEF0TXMiOiI0ODE4Njk4Mjc2NjI0Iiwic2Vzc2lvblR5cGUiOiJQQVJUTkVSIiwiZXhwIjo0ODE4Njk4Mjc2LCJnYW1lU2hhcmRJZCI6IjAwMDAwMDAwLTAwMDAtMDAwMC0wMDAwLTAwMDAwMDAwMDAwMSIsImlhdCI6MTY2NTA5ODI3NiwicHJhZ21hUGxheWVySWQiOiIwMDAwMDAwMC0wMDAwLTAwMDAtMDAwMC0wMDAwMDAwMDAwMDAiLCJqdGkiOiIyMTBhODlkMC0zMzYzLTRlMWEtYTY1Zi1jZWY4YTNlOGJkZTEifQ.qrcnkhBfSm9lxKn8bQypC6A84bGVi4KDtkxuZhBkGBgrzEzqNxAbQWCMz0gadaqVwt3MOcYQOIXaTE3qLZRMdacnVCztQuXmT7pBcTVQiZ3H8T0RHEa2l_7N9EE2YY7RBoeo07igjLaxoQ5yUl4YGsQ1RwZ0Q4J5XYgMoGM7KjJu5YViVHJBIC0iiOOJHlNoBhEiTW6IMWfx_BmwwvtTIuWB7QFYlUBevtqQwt5k_X9u3_slZpX9xDcsf6GDTkV55t7ie3Vtj1ORQeV1d_x6yxKF1AH5ezPLcRXnYwbDjXGfY0PwoKpGmR_4QnrWceIxMVTlMLVl-EC6msO-ue0x1g";
	Server = TestPtr->ServerRuntime()->GetOrCreateServer();
}

FTestServer::~FTestServer()
{
	if (TestPtr.IsValid() && TestPtr->ServerRuntime() != nullptr)
	{
		OwningTest()->ServerRuntime()->DestroyServer();
	}
}

void FTestServer::ConnectOrFail() const
{
	OwningTest()->AddStepLatent([this](const FAsyncTest::FOnStepComplete OnStepComplete)
	{
		Session()->Connect(FServer::FConnectedDelegate::CreateLambda([this, OnStepComplete](const TPragmaResult<>& Result)
		{
			if (Result.IsFailure())
			{
				OwningTest()->Fail("PragmaSession -- Failed to initialize server");
			}
			(*OnStepComplete)();
		}));
	});
}
}
}
