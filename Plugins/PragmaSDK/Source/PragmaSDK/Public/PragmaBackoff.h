#pragma once
#include "PragmaBackoffBase.h"
#include "PragmaTimerManager.h"

class FPragmaBackoff : public IPragmaBackoff
{
public:
	DECLARE_DELEGATE_FourParams(FSetTimer, FPragmaTimerHandle&, const FPragmaTimerDelegate&, float, bool);

	explicit FPragmaBackoff(TWeakPtr<IPragmaTimerManager> InTimerManager);

	virtual void SetBackoffFunc(FPragmaTimerDelegate InFunc) override;
	virtual bool IsBackoffFuncSet() override;
	virtual void Continue() override;
	virtual void Cancel() override;
	virtual TOptional<float> SecondsUntilNextRetry() const override;

private:
	float GetDelay();

public:
	FSetTimer SetTimerDelegate;
	FPragmaTimerHandle TimerHandle;

	FString FuncName;
	FPragmaTimerDelegate BackoffFunc;
	int8 ExponentCounter{0};

	float InitialRetrySeconds{0.f};
	float StaggerReconnectRandomSeconds{0.f};
	float MaxSecondsBetweenRetries{0.f};

	TWeakPtr<IPragmaTimerManager> TimerManager;
};
