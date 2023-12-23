#include "CoreMinimal.h"

#include "PragmaResult.h"
#include "Tests/PragmaAutomationSpecBase.h"

namespace Pragma
{
namespace Tests
{
PRAGMA_BEGIN_DEFINE_SPEC(TPragmaResultTest, "PragmaSDK.UnitTests.PragmaResult")

	EPragma_PlatformError GivenPragmaError;

PRAGMA_END_DEFINE_SPEC(TPragmaResultTest)

void TPragmaResultTest::Define()
{
	BeforeEach([this]()
	{
		GivenPragmaError = EPragma_PlatformError::AccountService_Error;
	});

	Describe("Templatized TPragmaResult", [this]()
	{
		It("Failure given an Error should have correct underlying error", [this]()
		{
			const FPragmaError Error = GivenPragmaError;
			const TPragmaResult<FJsonObject> Result = TPragmaResult<FJsonObject>::Failure(Error);

			TestEqual("Expect Pragma Error Enum", GivenPragmaError, Result.Error().Platform());
			TestEqual("Expect string of Pragma Error", "EPragma_PlatformError::AccountService_Error",
			          Result.ErrorCode());
		});

		It("Failure given an ServiceError should have correct underlying error", [this]()
		{
			FPragmaServiceError ServiceError = {};
			ServiceError.Error = GivenPragmaError;
			const TPragmaResult<FJsonObject> Result = TPragmaResult<FJsonObject>::Failure(ServiceError);

			TestEqual("Expect Pragma Error Enum", GivenPragmaError, Result.Error().Platform());
			TestEqual("Expect string of Pragma Error", "EPragma_PlatformError::AccountService_Error",
			          Result.ErrorCode());
		});

		It("Failure given a string should have correct underlying error", [this]()
		{
			const TPragmaResult<FJsonObject> Result = TPragmaResult<FJsonObject>::Failure("SOME_STRING_ERROR");

			TestEqual("Expect string error code", "SOME_STRING_ERROR", Result.ErrorCode());
		});
	});

	Describe("Void Spec", [this]()
	{
		It("Failure given ServiceError should have correct underlying error", [this]()
		{
			FPragmaServiceError ServiceError = {};
			ServiceError.Error = GivenPragmaError;

			const TPragmaResult<void> Result = TPragmaResult<void>::Failure(ServiceError);

			TestEqual("Expect Account Service Error", GivenPragmaError, Result.Error().Platform());
		});

		It("Failure given Error should have correct underlying error", [this]()
		{
			const FPragmaError Error = GivenPragmaError;
			const TPragmaResult<void> Result = TPragmaResult<void>::Failure(Error);

			TestEqual("Expect Account Service Error", GivenPragmaError, Result.Error().Platform());
		});

		It("Success should work", [this]()
		{
			const TPragmaResult<void> Result = TPragmaResult<void>::Success();

			TestTrue("IsSuccessful()", Result.IsSuccessful());
		});

		It("Should return empty string if IsUnknown", [this]()
		{
			const FPragmaError Error = FPragmaError{};
			const TPragmaResult<void> Result = TPragmaResult<void>::Failure(Error);

			TestEqual("Should be equal", Result.ErrorCode(), "");
		});

		It("Should return the right error code when set with an Error", [this]()
		{
			const FPragmaError Error = EPragma_PlatformError::AccountService_Error;
			const TPragmaResult<void> Result = TPragmaResult<void>::Failure(Error);

			TestEqual("Should be equal", Result.ErrorCode(), "EPragma_PlatformError::AccountService_Error");
		});
	});
}
}
}
