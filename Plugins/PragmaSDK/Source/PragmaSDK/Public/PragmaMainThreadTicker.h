#pragma once
#include "IPragmaTickable.h"
#include "PragmaPtr.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Delegates/Delegate.h"

#if ENGINE_MAJOR_VERSION > 4
#include "Containers/Ticker.h"
#endif

#if ENGINE_MAJOR_VERSION > 4
	using FPragmaTickHandle = FTSTicker::FDelegateHandle;
#else
	using FPragmaTickHandle = FDelegateHandle;
#endif

namespace Pragma
{
class FMainThreadTicker;
using FMainThreadTickerPtr = TSharedPtr<FMainThreadTicker>;

class PRAGMASDK_API FMainThreadTicker final : public TSharedFromThis<FMainThreadTicker>
{
public:
	static FMainThreadTickerPtr Create()
	{
		FMainThreadTickerPtr Ptr{new FMainThreadTicker()};
		Ptr->Start();
		return Ptr;
	}
	~FMainThreadTicker();
	
	DECLARE_EVENT(FMainThreadTicker, FTickEvent);
	FTickEvent OnTick;

	void Start();
	void Stop();

	void AddTickable(TSharedPtr<IPragmaTickable> Tickable);
	void RemoveTickable(TSharedPtr<IPragmaTickable> Tickable);
	
	bool Tick(float DeltaTime) const;

private:
	// Only instantiate with static Create.
	FMainThreadTicker() = default;
	
	TArray<TSharedPtr<IPragmaTickable>> Tickables;
	FPragmaTickHandle TickHandle;
};

}
