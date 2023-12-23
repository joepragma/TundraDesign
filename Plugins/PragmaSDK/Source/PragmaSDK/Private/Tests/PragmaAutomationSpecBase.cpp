#include "Tests/PragmaAutomationSpecBase.h"

FPragmaAutomationSpecBase::FPragmaAutomationSpecBase(const FString& InName, const bool bInComplexTask)
	: FAutomationSpecBase(InName, bInComplexTask)
{
	AfterEach([this] {
		// Shutdown Runtimes in a different context than the rest of the test so we don't end up in a deeply
		// nested tick/event callback tree when we destroy stuff. 
		TSharedPtr<Pragma::Tests::FAsyncTest> TestPtr;
		while (Tests.Dequeue(TestPtr))
		{
			TestPtr->Shutdown();
		}
	});
}

void FPragmaAutomationSpecBase::AsyncIt(const FString& Name, TFunction<void(const TSharedRef<Pragma::Tests::FAsyncTest>& Test)> DefineSteps, const FTimespan Timeout)
{
	// Always run on the main thread as if we were running from game code. Otherwise (e.g.) Runtime main thread ticker can cause issues. 
	LatentIt(Name, EAsyncExecution::TaskGraphMainThread, Timeout, [this, Name, DefineSteps](const FDoneDelegate& Done) mutable {
		const auto Test = NewTest();
		auto       TestRef = Test.ToSharedRef();
		DefineSteps(TestRef);
		Test->AddFinalizeStep("Finish LatentIt", [Done] {
			UE_LOG(LogTemp, Log, TEXT("All test steps completed."));
			auto _ = Done.ExecuteIfBound();
		});
		Test->ExecuteTest(Name);
	});
}

void FPragmaAutomationSpecBase::xAsyncIt(const FString&, TFunction<void(const TSharedRef<Pragma::Tests::FAsyncTest>& Test)>, const FTimespan)
{
	// do nothing
}

void FPragmaAutomationSpecBase::ExpectErrorOrWarning(FAutomationSpecBase& Spec, const FString& SearchString)
{
	// Note that this just uses log matching on the _message_ of a log not including the log
	// category (e.g. LogPragma) or log type (e.g. Error, Warning).
	//
	// Note that passing 'Occurrences: 0' means "1 or more".
	Spec.AddExpectedError(SearchString, EAutomationExpectedErrorFlags::MatchType::Contains, 0);
}

void FPragmaAutomationSpecBase::ExpectErrorOrWarning(FAutomationSpecBase& Spec)
{
	// PragmaTest: is only added when the commandline -PragmaTest is specified. See PragmaLog.h
	ExpectErrorOrWarning(Spec, TEXT("PragmaTest:"));
}
