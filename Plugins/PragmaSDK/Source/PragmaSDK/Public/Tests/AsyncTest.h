#pragma once
#include "PragmaHttp.h"
#include "PragmaPtr.h"
#include "Misc/AutomationTest.h"

PRAGMA_FWD(FRuntime);
PRAGMA_FWD(FPlayer);

class FJsonObject;
class FPragmaAutomationSpecBase;

namespace Pragma
{
namespace Tests
{

// Must be forward ref to prevent circular inclusion.
class FTestPlayer;
class FTestServer;

//
// This class has features to simplify testing asynchronous behavior. It follows a 'step' based pattern:
// you add your steps in the order you expect them to execute, then tell it to BeginTest(), and the helper will execute them
// in the order specified, waiting for each to complete.
//
class PRAGMASDK_API FAsyncTest final : public TSharedFromThis<FAsyncTest>
{
public:
	typedef TSharedPtr<TFunction<void()>> FOnStepComplete;

	explicit FAsyncTest(FAutomationSpecBase& Fixture);
	FAsyncTest(const FAsyncTest& Done) = delete;

	void Shutdown();

	// Add a step that executes in order with other steps.
	void AddStep(const FString& Name, TUniqueFunction<void()> CustomStep);
	void AddStep(TUniqueFunction<void()> CustomStep);

	// Add a step that executes at the end of all test steps, regardless of success or failure.
	void AddFinalizeStep(const FString& Name, TUniqueFunction<void()> CustomStep);

	// Add a step that executes in order with other steps that does not return immediately.
	void AddStepLatent(const FString& Name, TUniqueFunction<void(FOnStepComplete)> CustomStep);
	void AddStepLatent(TUniqueFunction<void(FOnStepComplete)> CustomStep);

	// Begin the test, each added step will be executed in order.
	void ExecuteTest(const FString& Name);

	// Fail and immediately end the test.
	void Fail(const FString& Message);

	// Access to the Pragma Runtime with a lifetime equivalent to this object.
	FRuntimePtr Runtime() { return PlayerRuntimePtr; }
	FRuntimePtr ServerRuntime() { return ServerRuntimePtr; }

	// Create a TestPlayer with a lifetime equivalent to this object.
	TSharedRef<FTestPlayer> CreateTestPlayer();

	// Create a TestServer with a lifetime equivalent to this object. There can be only one Server.
	TSharedRef<FTestServer> CreateTestServer();

	// Equivalent to CreateTestPlayer multiple times.
	void CreateTestPlayers(const int32 Amount);

	// Access to Players created via CreateTestPlayer.
	TSharedRef<FTestPlayer> Player(const int32 Index) { check(TestPlayers.IsValidIndex(Index)); return TestPlayers[Index].Pin().ToSharedRef(); }
	const TArray<TWeakPtr<FTestPlayer>>& Players() const { return TestPlayers; }
	FPlayerPtr PlayerSession(const int32 Index);

	// Access to Server created via CreateTestServer.
	TSharedRef<FTestServer> Server() { return TestServer.Pin().ToSharedRef(); }

private:
	bool HasEnded() { return bHasEnded; }
	void EndTest();

	void NextStep(int CallingStepIndex);
	void RunFinalizeSteps();

	FAutomationSpecBase& Fixture;

	typedef TUniqueFunction<void()> FStepFn;
	struct FStep
	{
		FString Name;
		FStepFn Fn;
	};

	bool bHasBegun{false};
	bool bHasEnded{false};
	TArray<FStep> TestSteps;
	TArray<FStep> FinalizeSteps;
	TArray<FStep>::SizeType StepIndex;

	// Currently a single shared config doesn't play nice with both player and server in the same runtime.
	// This only occurs in tests atm, so we just use two runtimes and pretend they're separate, which is
	// how it would be in a dedicated server architecture anyway.
	FRuntimeStrongPtr PlayerRuntimePtr;
	FRuntimeStrongPtr ServerRuntimePtr;

	// Players and Servers keep a strong ptr to us, and they are passed around in the test.
	// If we also keep strong ptr to them, we never die.
	TArray<TWeakPtr<FTestPlayer>> TestPlayers;
	TWeakPtr<FTestServer> TestServer;

	TSharedPtr<FPragmaHttp> Http;
};
}
}
