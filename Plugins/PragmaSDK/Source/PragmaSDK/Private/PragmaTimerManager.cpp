#include "PragmaTimerManager.h"

void FPragmaTimerManager::SetTimer(FPragmaTimerHandle& InOutHandle, const FPragmaTimerDelegate& InDelegate, const float InRate, const bool InbLoop, const float InFirstDelay)
{
	if (InRate <= 0.0f)
	{
		return;
	}
	
	FGuid& Handle = InOutHandle.Id;
	if (!InOutHandle.IsValid())
	{
		Handle = FGuid::NewGuid();
	}

	FTimer& Timer = ActiveTimers.FindOrAdd(Handle);

	Timer.Callback = InDelegate;
	Timer.Duration = InRate;
	Timer.RemainingSeconds = Timer.Duration;
	Timer.IsLooping = InbLoop;

	if (InFirstDelay > 0.0f)
	{
		Timer.RemainingSeconds += InFirstDelay;
	}
}

void FPragmaTimerManager::ClearTimer(FPragmaTimerHandle& Handle)
{
	ActiveTimers.Remove(Handle.Id);
	Handle.Invalidate();
}

bool FPragmaTimerManager::IsTimerActive(const FPragmaTimerHandle& Handle) const
{
	return ActiveTimers.Contains(Handle.Id);
}

TOptional<float> FPragmaTimerManager::RemainingSeconds(const FPragmaTimerHandle& Handle) const
{
	const auto Timer = ActiveTimers.Find(Handle.Id);
	if (Timer == nullptr)
	{
		return {};
	}
	return Timer->RemainingSeconds;
}

void FPragmaTimerManager::Tick(const float DeltaTime)
{
	for (auto It = ActiveTimers.CreateIterator(); It; ++It)
	{
		FTimer& Timer = It.Value();
		Timer.RemainingSeconds -= DeltaTime;
		if (Timer.RemainingSeconds > 0.0f)
		{
			continue;
		}

		if (Timer.IsLooping)
		{
			// Drain remaining in case we had a long tick.
			while (Timer.RemainingSeconds <= 0.0f)
			{
				auto _2 = Timer.Callback.ExecuteIfBound();
				Timer.RemainingSeconds += Timer.Duration;
			}
		}
		else
		{
			auto _ = Timer.Callback.ExecuteIfBound();
			It.RemoveCurrent();
		}
	}
}
