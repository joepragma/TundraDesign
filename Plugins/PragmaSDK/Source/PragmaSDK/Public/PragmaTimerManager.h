#pragma once

#include "IPragmaTickable.h"
#include "Delegates/Delegate.h"
#include "Misc/Guid.h"
#include "Containers/Map.h"

DECLARE_DELEGATE(FPragmaTimerDelegate);

class FPragmaTimerHandle
{
	friend class FPragmaTimerManager;

public:
	FPragmaTimerHandle() = default;
	~FPragmaTimerHandle() = default;

	FGuid GetId() const { return Id; }
	bool IsValid() const { return Id.IsValid(); }
	void Invalidate() { Id.Invalidate(); }
	
private:
	FGuid Id;
};

class PRAGMASDK_API IPragmaTimerManager : public IPragmaTickable
{
public:
	virtual void SetTimer(FPragmaTimerHandle& InOutHandle, FPragmaTimerDelegate const& InDelegate, float InRate, bool InbLoop, float InFirstDelay = -1.f) = 0;
	virtual void ClearTimer(FPragmaTimerHandle& InOutHandle) = 0;
	virtual bool IsTimerActive(const FPragmaTimerHandle& Handle) const = 0;
	virtual TOptional<float> RemainingSeconds(const FPragmaTimerHandle& Handle) const = 0;
};

class PRAGMASDK_API FPragmaTimerManager final : public IPragmaTimerManager
{
public:
	virtual void SetTimer(FPragmaTimerHandle& InOutHandle, FPragmaTimerDelegate const& InDelegate, float InRate, bool InbLoop, float InFirstDelay = -1.f) override;
	virtual void ClearTimer(FPragmaTimerHandle& Handle) override;
	virtual bool IsTimerActive(const FPragmaTimerHandle& Handle) const override;
	virtual TOptional<float> RemainingSeconds(const FPragmaTimerHandle& Handle) const override;

	virtual void Tick(float DeltaTime) override;

private:
	struct FTimer {
		bool IsLooping{false};
		FPragmaTimerDelegate Callback;
		float RemainingSeconds{0.0f};
		float Duration = {0.0f};
	};
	TMap<FGuid, FTimer> ActiveTimers;
};
