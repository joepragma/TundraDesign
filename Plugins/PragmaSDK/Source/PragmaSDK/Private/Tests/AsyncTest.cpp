#include "Tests/AsyncTest.h"
#include "PragmaRuntime.h"
#include "PragmaSdkConfig.h"
#include "Dto/PragmaConfigDto.h"
#include "Tests/TestPlayer.h"
#include "Tests/TestServer.h"

namespace Pragma
{
namespace Tests
{

FAsyncTest::FAsyncTest(FAutomationSpecBase& Fixture)
	: Fixture{Fixture}
	, StepIndex{0}
	, PlayerRuntimePtr{FRuntime::Create()}
	, ServerRuntimePtr{FRuntime::Create()}
{
	PlayerRuntimePtr->Config().BackendAddress = "http://127.0.0.1:10000";
	PlayerRuntimePtr->Config().ProtocolType = EPragmaProtocolType::WebSocket;
	PlayerRuntimePtr->Config().bDebugServer = false;

	ServerRuntimePtr->Config().BackendAddress = "http://127.0.0.1:10000";
	ServerRuntimePtr->Config().bDebugServer = false;
}

void FAsyncTest::Shutdown()
{
	UE_LOG(LogTemp, Log, TEXT("FAsyncTest::Shutdown -- destroying runtimes."));
	PlayerRuntimePtr.Reset();
	ServerRuntimePtr.Reset();
}

void FAsyncTest::AddStep(TUniqueFunction<void()> CustomStep)
{
	static FString Name = "<no name>";
	AddStep(Name, MoveTemp(CustomStep));
}

void FAsyncTest::AddStep(const FString& Name, TUniqueFunction<void()> CustomStep)
{
	int LocalStepIndex = TestSteps.Num();
	TestSteps.Add(FStep{Name, [this, LocalStepIndex, CustomStep = MoveTemp(CustomStep)]()
    {
        CustomStep();
		NextStep(LocalStepIndex);
    }});
}

void FAsyncTest::AddFinalizeStep(const FString& Name, TUniqueFunction<void()> CustomStep)
{
	FinalizeSteps.Add(FStep{Name, MoveTemp(CustomStep)});
}

void FAsyncTest::AddStepLatent(TUniqueFunction<void(FOnStepComplete)> CustomStep)
{
	static FString Name = "<no name>";
	AddStepLatent(Name, MoveTemp(CustomStep));
}

void FAsyncTest::AddStepLatent(const FString& Name, TUniqueFunction<void(FOnStepComplete)> CustomStep)
{
	int LocalStepIndex = TestSteps.Num();
	TestSteps.Add(FStep{Name, [this, LocalStepIndex, CustomStep = MoveTemp(CustomStep)]()
    {
        CustomStep(MakeShared<TFunction<void()>>([this, LocalStepIndex]()
        {
			NextStep(LocalStepIndex);
        }));
    }});
}

void FAsyncTest::ExecuteTest(const FString& Name)
{
	if (bHasBegun)
	{
		UE_LOG(LogTemp, Warning, TEXT("FAsyncTest::BeginTest called more than once. Ignoring duplicate call."));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("------------ BEGINNING ASYNC TEST '%s' (TestSteps? %d) ------------"), *Name, TestSteps.Num());
	bHasBegun = true;
	StepIndex = 0;
	NextStep(-1);
}

void FAsyncTest::Fail(const FString& Message)
{
	Fixture.AddError(Message, 1);
	UE_LOG(LogTemp, Error, TEXT("Fail() called with message: %s"), *Message);
	EndTest();
}

TSharedRef<FTestPlayer> FAsyncTest::CreateTestPlayer()
{
	const auto Ptr = MakeShared<FTestPlayer>(this->AsShared());
	TestPlayers.Add(Ptr);
	return Ptr;
}

TSharedRef<FTestServer> FAsyncTest::CreateTestServer()
{
	const auto Ptr = MakeShared<FTestServer>(this->AsShared());
	TestServer = Ptr;
	return Ptr;
}

void FAsyncTest::CreateTestPlayers(const int32 Amount)
{
	for (int i = 0; i < Amount; ++i)
	{
		CreateTestPlayer();
	}
}

FPlayerPtr FAsyncTest::PlayerSession(const int32 Index)
{
	return Player(Index)->Session();
}

void FAsyncTest::EndTest()
{
	UE_LOG(LogTemp, Log, TEXT("END OF TEST"));
	StepIndex = TestSteps.Num();
	RunFinalizeSteps();
}

void FAsyncTest::NextStep(const int CallingStepIndex)
{
	if (StepIndex != CallingStepIndex + 1)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Step %d attempting to call NextStep, but we are already on step %d"), CallingStepIndex, this->StepIndex);
		return;
	}

	if (HasEnded())
	{
		return;
	}

	if (StepIndex == TestSteps.Num())
	{
		EndTest();
		return;
	}

	const int CurrentStepIndex = StepIndex;
	StepIndex++;
	if (CurrentStepIndex < TestSteps.Num())
	{
		const auto& Step = TestSteps[CurrentStepIndex];
		UE_LOG(LogTemp, Log, TEXT("%s - starting Step '%s' [%d]"), ANSI_TO_TCHAR(__FUNCTION__), *Step.Name, CurrentStepIndex);
		Step.Fn();
	}
}

void FAsyncTest::RunFinalizeSteps()
{
	for (const auto& Step : FinalizeSteps)
	{
		Step.Fn();
	}
}

} // namespace Tests
} // namespace Pragma
