#include "PragmaHeartbeat.h"
#include "PragmaTimerManager.h"
#include "Tests/PragmaAutomationSpecBase.h"

PRAGMA_BEGIN_DEFINE_SPEC(FPragmaHeartbeatTest, "PragmaSDK.Heartbeat")
PRAGMA_END_DEFINE_SPEC(FPragmaHeartbeatTest)

void FPragmaHeartbeatTest::Define()
{
	Describe("Start", [this]()
	{
		It("Starts beat", [this]()
		{
			const auto TimerManager = MakeShared<FPragmaTimerManager>();
			auto Heartbeat = FPragmaHeartbeat::Create(TimerManager);
			int32 Triggered = 0;
			Heartbeat->BeatActionDelegate.BindLambda([&Triggered]()
			{
				Triggered += 1;
			});
			Heartbeat->SetTimerDelegate.BindLambda([this, Heartbeat](FPragmaTimerHandle& , FPragmaTimerDelegate const& InDelegate, const float InRate, const bool InbLoop, const float InFirstDelay)
			{
				// For the purpose of this test, Start() also Beats()
				InDelegate.Execute();

				TestEqual("Expected same period seconds", InRate, Heartbeat->HeartbeatPeriodSeconds, 0.0f);
				TestEqual("Expected loop", InbLoop, true);
				TestEqual("Expected same delay seconds", InFirstDelay, 0.0f, 0.0f);
			});

			auto count = 5;
			for (int i = 0; i < count; i++)
			{
				Heartbeat->Start();
			}

			TestEqual("Expected equal beat count", Triggered, count);
		});
	});
}
