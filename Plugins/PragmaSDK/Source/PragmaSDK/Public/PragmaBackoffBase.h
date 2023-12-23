#pragma once
#include "PragmaTimerManager.h"

class IPragmaBackoff
{
public:
	virtual ~IPragmaBackoff() {}
	virtual void SetBackoffFunc(FPragmaTimerDelegate InFunc) = 0;
	virtual bool IsBackoffFuncSet() = 0;
	virtual void Continue() = 0;
	virtual void Cancel() = 0;
	virtual TOptional<float> SecondsUntilNextRetry() const = 0;
};
