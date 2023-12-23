#include "Misc/AutomationTest.h"

#include "PragmaBackoff.h"
#include "PragmaTimerManager.h"
#include "Tests/PragmaAutomationSpecBase.h"

PRAGMA_BEGIN_DEFINE_SPEC(FPragmaBackoffTest, "PragmaSDK.Backoff")
PRAGMA_END_DEFINE_SPEC(FPragmaBackoffTest)

DECLARE_DELEGATE(FMyFunc)

void FPragmaBackoffTest::Define()
{
	Describe("IsBackoffFuncSet and SetBackoffFunc", [this]()
	{
		It("They work", [this]()
		{
			const auto TimerManager = MakeShared<FPragmaTimerManager>();
			FPragmaBackoff Backoff{TimerManager};

			TestFalse("Should be unset", Backoff.IsBackoffFuncSet());
			Backoff.SetBackoffFunc(FMyFunc::CreateLambda([]()
			{
			}));
			TestTrue("Should be set", Backoff.IsBackoffFuncSet());
		});
	});

	Describe("Continue", [this]()
	{
		It("Returns exponentially increasing delays", [this]()
		{
			const auto InitialRetrySeconds = 0.5f;
			const auto StaggerReconnectRandomSeconds = 2.0f;
			const auto MaxSecondsBetweenRetries = 128.0f;
			const auto TimerManager = MakeShared<FPragmaTimerManager>();
			FPragmaBackoff Backoff{TimerManager};
			Backoff.InitialRetrySeconds = InitialRetrySeconds;
			Backoff.StaggerReconnectRandomSeconds = StaggerReconnectRandomSeconds;
			Backoff.MaxSecondsBetweenRetries = MaxSecondsBetweenRetries;

			int32 counter = 0;
			Backoff.SetTimerDelegate.BindLambda(
				[this,
					&counter,
					&InitialRetrySeconds,
					&StaggerReconnectRandomSeconds,
					&MaxSecondsBetweenRetries]
			(FPragmaTimerHandle&, const FPragmaTimerDelegate&, const float Delay, const bool Loop)
				{
					if (counter == 0)
					{
						TestEqual("InitialRetrySeconds equals delay", Delay, InitialRetrySeconds);
						counter++;
					}
					else
					{
						const float ExpectedBase = FMath::Pow(counter++, 2.0f);
						const float ExpectedMin = FMath::Min(ExpectedBase + 0.0f, MaxSecondsBetweenRetries);
						const float ExpectedMax = FMath::Min(ExpectedBase + StaggerReconnectRandomSeconds, MaxSecondsBetweenRetries);

						TestTrue(FString::Printf(TEXT("Greater than min [%f] vs [%f]"), Delay, ExpectedMin), Delay >= ExpectedMin);
						TestTrue(FString::Printf(TEXT("Less than max [%f] vs [%f]"), Delay, ExpectedMax), Delay <= ExpectedMax);
						TestFalse("Does not loop", Loop);
					}
				}
			);
			for (int i = 0; i < 20; i++)
			{
				Backoff.Continue();
			}
		});
	});
}
