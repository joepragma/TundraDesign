#pragma once

#include "PragmaTimerManager.h"

class PRAGMASDK_API FPragmaHeartbeat final : public TSharedFromThis<FPragmaHeartbeat>
{
public:
	static TSharedPtr<FPragmaHeartbeat> Create(TWeakPtr<IPragmaTimerManager> InTimerManager)
	{
		return TSharedPtr<FPragmaHeartbeat>{new FPragmaHeartbeat(MoveTemp(InTimerManager))};
	}

	void Start();
	void Beat() const;
	void Stop();

	float HeartbeatPeriodSeconds{};

	DECLARE_DELEGATE_FiveParams(FSetTimerDelegate, FPragmaTimerHandle&, const FPragmaTimerDelegate&, float, bool, float);
	FSetTimerDelegate SetTimerDelegate;

	DECLARE_DELEGATE(FBeatActionDelegate);
	FBeatActionDelegate BeatActionDelegate;

private:
	explicit FPragmaHeartbeat(TWeakPtr<IPragmaTimerManager> InTimerManager);

	TWeakPtr<IPragmaTimerManager> TimerManager;
	FPragmaTimerHandle HeartbeatTimerHandle;
};
