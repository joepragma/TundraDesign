#pragma once

#include "Async/Future.h"
#include "PragmaLog.h"

namespace Pragma
{
template <typename T>
class TPromiseAll final
{
private:
	enum class EState
	{
		Initializing,	// Adding futures.
		Initialized,    // Can no longer add futures. Once all futures are completed will move to Completed.
		Completed,		// All futures complete.
	};

public:
	TPromiseAll() {}

	TPromiseAll& Add(TFuture<T>&& Future)
	{
		if (State != EState::Initializing)
		{
			PRAGMA_LOG(Error, "%s -- All add futures before Initialize().", ANSI_TO_TCHAR(__FUNCTION__));
			return *this;
		}
		Futures.Emplace(MoveTemp(Future));
		return *this;
	}

	TPromiseAll& Initialize()
	{
		if (State != EState::Initializing)
		{
			PRAGMA_LOG(Error, "%s -- Already initialized.", ANSI_TO_TCHAR(__FUNCTION__));
			return *this;
		}
		State = EState::Initialized;
		if (Futures.Num() == 0)
		{
			// No futures.
			TryComplete();
			return *this;
		}
		// The continuation of a future invalidates the future by moving it into the lambda.
		// We create a zeroed array to store results as the futures finish, _in the order the futures are added_,
		// not the order they complete.
		Results.AddZeroed(Futures.Num());
		for (int i = 0; i < Futures.Num(); ++i)
		{
			Futures[i].Next([this, i](T Result)
			{
				Results[i] = MoveTempIfPossible(Result);
				++ResultsCollected;
				TryComplete();
			});
		}
		return *this;
	}

	TFuture<TArray<T>> GetFuture() { return AllCompletePromise.GetFuture(); }

private:
	EState State{EState::Initializing};
	TPromise<TArray<T>> AllCompletePromise;
	TArray<TFuture<T>> Futures;
	TArray<T> Results;
	int32 ResultsCollected{0};
	
	 void TryComplete()
	 {
		if (State != EState::Initialized)
		{
			PRAGMA_LOG(Error, "%s -- Must be initialized before completion. Cannot complete more than once.", ANSI_TO_TCHAR(__FUNCTION__));
			return;
		}
	 	if (ResultsCollected < Results.Num())
	 	{
	 		return;
	 	}
	 	State = EState::Completed;
	 	AllCompletePromise.EmplaceValue(MoveTemp(Results));
	 }
};
}
