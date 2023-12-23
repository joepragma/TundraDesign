#pragma once
#include "PragmaBackoffBase.h"

class FMockBackoff final : public IPragmaBackoff
{
public:
	virtual void SetBackoffFunc(const FPragmaTimerDelegate InFunc) override
	{
		SetBackoffFuncCalled = true;
		InFunc.Execute();
	}

	virtual bool IsBackoffFuncSet() override
	{
		return IsBackoffFuncSetResult;
	}

	virtual void Continue() override
	{
		ContinueCount++;
	}

	virtual void Cancel() override
	{
		CancelCalled = true;
	}

	virtual TOptional<float> SecondsUntilNextRetry() const override
	{
		return {};
	}

	bool SetBackoffFuncCalled = false;
	int32 ContinueCount = 0;
	bool CancelCalled = false;
	bool IsBackoffFuncSetResult = true;
};
