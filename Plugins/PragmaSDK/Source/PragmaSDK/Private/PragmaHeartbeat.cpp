#include "PragmaHeartbeat.h"
#include "PragmaLog.h"
#include "PragmaTimerManager.h"

FPragmaHeartbeat::FPragmaHeartbeat(TWeakPtr<IPragmaTimerManager> InTimerManager)
	: TimerManager(MoveTemp(InTimerManager))
{
}

void FPragmaHeartbeat::Start()
{
	const FPragmaTimerDelegate InDelegate = FPragmaTimerDelegate::CreateSP(this, &FPragmaHeartbeat::Beat);
	const bool Loop = true;
	const float InFirstDelay = 0.0f;

	if(SetTimerDelegate.IsBound())
	{
		SetTimerDelegate.Execute(HeartbeatTimerHandle, InDelegate, HeartbeatPeriodSeconds, Loop, InFirstDelay);
		return;
	}
	if (const auto PinnedTimerManager = TimerManager.Pin())
	{
		if (PinnedTimerManager->IsTimerActive(HeartbeatTimerHandle))
		{
			return;
		}
		PRAGMA_LOG(Log, "FPragmaHeartbeat::Start -- Starting heartbeat timer [%f] seconds.", HeartbeatPeriodSeconds);
		PinnedTimerManager->SetTimer(HeartbeatTimerHandle, InDelegate,  HeartbeatPeriodSeconds, Loop, InFirstDelay);
		return;
	}
}

void FPragmaHeartbeat::Beat() const
{
	auto _ = BeatActionDelegate.ExecuteIfBound();
}

void FPragmaHeartbeat::Stop()
{
	if (auto PinnedTimerManager = TimerManager.Pin())
	{
		if (!PinnedTimerManager->IsTimerActive(HeartbeatTimerHandle))
		{
			return;
		}
		PRAGMA_LOG(Log, "FPragmaHeartbeat::Stop -- Stopping heartbeat.", HeartbeatPeriodSeconds);
		PinnedTimerManager->ClearTimer(HeartbeatTimerHandle);
	}
}
