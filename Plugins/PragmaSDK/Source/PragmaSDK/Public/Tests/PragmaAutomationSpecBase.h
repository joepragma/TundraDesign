#pragma once

#include "AsyncTest.h"
#include "Misc/AutomationTest.h"

class PRAGMASDK_API FPragmaAutomationSpecBase : public FAutomationSpecBase
{
public:
	FPragmaAutomationSpecBase(const FString& InName, const bool bInComplexTask);

	TSharedPtr<Pragma::Tests::FAsyncTest> NewTest()
	{
		auto Test = MakeShared<Pragma::Tests::FAsyncTest>(*this);
		Tests.Enqueue(Test);
		return Test;
	}

	void AsyncIt(const FString& Name, TFunction<void(const TSharedRef<Pragma::Tests::FAsyncTest>& Test)> DefineSteps, const FTimespan Timeout = FTimespan::FromSeconds(10));
	void xAsyncIt(const FString& Name, TFunction<void(const TSharedRef<Pragma::Tests::FAsyncTest>& Test)> DefineSteps, const FTimespan Timeout = FTimespan::FromSeconds(10));

	static void ExpectErrorOrWarning(FAutomationSpecBase& Spec, const FString& SearchString);

	// By default, automation tests fail on _any_ error. Sometimes we actually expect an error.
	static void ExpectErrorOrWarning(FAutomationSpecBase& Spec);

private:
	TQueue<TSharedPtr<Pragma::Tests::FAsyncTest>> Tests;
};

#define PRAGMA_BEGIN_DEFINE_SPEC( TClass, PrettyName ) \
	PRAGMA_BEGIN_DEFINE_SPEC_PRIVATE(TClass, PrettyName, EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask, __FILE__, __LINE__)

#define PRAGMA_BEGIN_DEFINE_SPEC_PRIVATE( TClass, PrettyName, TFlags, FileName, LineNumber ) \
	class TClass : public FPragmaAutomationSpecBase \
	{ \
	public: \
		TClass( const FString& InName ) \
		: FPragmaAutomationSpecBase( InName, false ) {\
			static_assert((TFlags)&EAutomationTestFlags::ApplicationContextMask, "AutomationTest has no application flag.  It shouldn't run.  See AutomationTest.h."); \
			static_assert(	(((TFlags)&EAutomationTestFlags::FilterMask) == EAutomationTestFlags::SmokeFilter) || \
							(((TFlags)&EAutomationTestFlags::FilterMask) == EAutomationTestFlags::EngineFilter) || \
							(((TFlags)&EAutomationTestFlags::FilterMask) == EAutomationTestFlags::ProductFilter) || \
							(((TFlags)&EAutomationTestFlags::FilterMask) == EAutomationTestFlags::PerfFilter) || \
							(((TFlags)&EAutomationTestFlags::FilterMask) == EAutomationTestFlags::StressFilter) || \
							(((TFlags)&EAutomationTestFlags::FilterMask) == EAutomationTestFlags::NegativeFilter), \
							"All AutomationTests must have exactly 1 filter type specified.  See AutomationTest.h."); \
		} \
		virtual uint32 GetTestFlags() const override { return TFlags; } \
		using FAutomationSpecBase::GetTestSourceFileName; \
		virtual FString GetTestSourceFileName() const override { return FileName; } \
		using FAutomationSpecBase::GetTestSourceFileLine; \
		virtual int32 GetTestSourceFileLine() const override { return LineNumber; } \
	protected: \
		virtual FString GetBeautifiedTestName() const override { return PrettyName; } \
		virtual void Define() override;

#define PRAGMA_END_DEFINE_SPEC( TClass ) END_DEFINE_SPEC( TClass )

#define PRAGMA_TEST_TRUE_OR_RETURN(Expression) do { TestTrue(TEXT(#Expression), (Expression)); if (!(Expression)) return; } while (0) 
#define PRAGMA_TEST_FALSE_OR_RETURN(Expression) do { TestFalse(TEXT(#Expression), (Expression)); if ((Expression)) return; } while (0) 
#define PRAGMA_TEST_EQUAL_OR_RETURN(Expression, Expected) do { TestEqual(TEXT(#Expression), (Expression)); if ((Expression != Expected)) return; } while (0) 
#define PRAGMA_TEST_TRUE_EXPR(Expression) TestTrue(TEXT(#Expression), Expression)
#define PRAGMA_TEST_FALSE_EXPR(Expression) TestFalse(TEXT(#Expression), Expression)
#define PRAGMA_TEST_EQUAL_EXPR(Expression, Expected) TestEqual(TEXT(#Expression), Expression, Expected)

namespace Pragma
{
	namespace Tests
	{
		// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
		inline FString CreateStr(const int i)
		{
			return FString::Printf(TEXT("%d"), i);
		}
	}
}
