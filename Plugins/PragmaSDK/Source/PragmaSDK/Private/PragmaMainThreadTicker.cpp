#include "PragmaMainThreadTicker.h"

#include "Containers/BackgroundableTicker.h"
#include "PragmaLog.h"

#if ENGINE_MAJOR_VERSION > 4
using FPragmaBackgroundTicker = FTSBackgroundableTicker;
#else
using FPragmaBackgroundTicker = FBackgroundableTicker;
#endif

namespace Pragma
{
FMainThreadTicker::~FMainThreadTicker()
{
	Stop();
}

void FMainThreadTicker::Start()
{
	PRAGMA_LOG(Verbose, "FMainThreadTicker -- Started ticking");
	TickHandle = FPragmaBackgroundTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateSP(this, &FMainThreadTicker::Tick), 0.0f);
}

void FMainThreadTicker::Stop()
{
	PRAGMA_LOG(Verbose, "FMainThreadTicker -- Stopped ticking");
	FPragmaBackgroundTicker::GetCoreTicker().RemoveTicker(TickHandle);
	TickHandle.Reset();
}

bool FMainThreadTicker::Tick(const float DeltaTime) const
{
	OnTick.Broadcast();
	for (auto& Tickable : Tickables)
	{
		Tickable->Tick(DeltaTime);
	}
	return true;
}

void FMainThreadTicker::AddTickable(TSharedPtr<IPragmaTickable> Tickable)
{
	Tickables.Add(MoveTemp(Tickable));
}

void FMainThreadTicker::RemoveTickable(TSharedPtr<IPragmaTickable> Tickable)
{
	Tickables.Remove(MoveTemp(Tickable));
}

}
