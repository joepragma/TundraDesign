#include "PragmaBackoff.h"
#include "Math/UnrealMathUtility.h"
#include "PragmaLog.h"
#include "PragmaTimerManager.h"

FPragmaBackoff::FPragmaBackoff(TWeakPtr<IPragmaTimerManager> InTimerManager)
	: TimerManager(MoveTemp(InTimerManager))
{
	checkf(TimerManager.IsValid(), TEXT("FPragmaBackoff must be created with a valid PragmaSession"));
}

void FPragmaBackoff::SetBackoffFunc(const FPragmaTimerDelegate InFunc)
{
	BackoffFunc = InFunc;
}

bool FPragmaBackoff::IsBackoffFuncSet()
{
	return BackoffFunc.IsBound();
}

void FPragmaBackoff::Continue()
{
	const float Delay = GetDelay();
	const bool Loop = false;

	if (SetTimerDelegate.IsBound())
	{
		SetTimerDelegate.Execute(TimerHandle, BackoffFunc, Delay, Loop);
		return;
	}
	if (auto PinnedTimerManager = TimerManager.Pin())
	{
		PRAGMA_LOG(Display, "%s -- Attempting [%s] in [%f] seconds.", ANSI_TO_TCHAR(__FUNCTION__), *FuncName, Delay);
		PinnedTimerManager->SetTimer(TimerHandle, BackoffFunc, Delay, Loop);
		return;
	}
}

void FPragmaBackoff::Cancel()
{
	if (auto PinnedTimerManager = TimerManager.Pin())
	{
		PinnedTimerManager->ClearTimer(TimerHandle);
	}
	ExponentCounter = 0;
}

TOptional<float> FPragmaBackoff::SecondsUntilNextRetry() const
{
	if (const auto PinnedTimerManager = TimerManager.Pin())
	{
		return PinnedTimerManager->RemainingSeconds(TimerHandle);
	}
	return {};
}

float FPragmaBackoff::GetDelay()
{
	if (ExponentCounter == 0)
	{
		ExponentCounter++;
		return InitialRetrySeconds;
	}

	const float BaseDelay = FMath::Pow(ExponentCounter, 2.0f);
	const float Stagger = FMath::RandRange(0.0f, StaggerReconnectRandomSeconds);
	if (BaseDelay + Stagger >= MaxSecondsBetweenRetries)
	{
		return MaxSecondsBetweenRetries;
	}
	ExponentCounter++;
	return BaseDelay + Stagger;
}
