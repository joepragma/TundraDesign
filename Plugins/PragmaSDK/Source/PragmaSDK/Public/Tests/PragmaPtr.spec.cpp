#include "CoreMinimal.h"
#include "PragmaRuntime.h"
#include "PragmaServer.h"
#include "PragmaPtr.h"
#include "Tests/PragmaAutomationSpecBase.h"

namespace
{
constexpr TCHAR TestBackendAddress[] = TEXT("TestBackendAddress");
}

namespace Pragma
{
namespace Tests
{
PRAGMA_BEGIN_DEFINE_SPEC(FPtrTests, "PragmaSDK.Ptr")
PRAGMA_END_DEFINE_SPEC(FPtrTests)

struct TestA {};
struct TestB : public TestA {};

void FPtrTests::Define()
{
	Describe("Ptr keeps correct raw ptr", [this]()
	{
		It("copy ctor", [this]()
		{
			const auto Data = MakeShared<FString, ESPMode::NotThreadSafe>("hi");
			const TPtr<FString> Ptr0{Data};
			const TPtr<FString> Ptr1{Ptr0};
			TestEqual("Ptrs equal", Ptr0, Ptr1);
			TestEqual("Contents equal", *Ptr0, *Data);
			TestEqual("Contents equal", *Ptr1, *Data);
		});
		
		It("move ctor", [this]()
		{
			const auto Data = MakeShared<FString, ESPMode::NotThreadSafe>("hi");
			TPtr<FString> Ptr0{Data};
			const TPtr<FString> Ptr1{MoveTemp(Ptr0)};
			TestEqual("Contents equal", *Ptr1, *Data);
		});
		
		It("copy assign", [this]()
		{
			const auto Data = MakeShared<FString, ESPMode::NotThreadSafe>("hi");
			const TPtr<FString> Ptr0 = Data;
			const TPtr<FString> Ptr1 = Ptr0;
			TestEqual("Ptrs equal", Ptr0, Ptr1);
			TestEqual("Contents equal", *Ptr0, *Data);
			TestEqual("Contents equal", *Ptr1, *Data);
		});
		
		It("move assign", [this]()
		{
			const auto Data = MakeShared<FString, ESPMode::NotThreadSafe>("hi");
			TPtr<FString> Ptr0 = Data;
			const TPtr<FString> Ptr1 = MoveTemp(Ptr0);
			TestEqual("Contents equal", *Ptr1, *Data);
		});
		
		It("upcast", [this]()
		{
			const TSharedPtr<TestB, SpMode> Data = MakeShared<TestB, ESPMode::NotThreadSafe>();
			const TPtr<TestB> Ptr0 = Data;
			const TPtr<TestA> Ptr1 = Ptr0;
			TestEqual("ptrs equal", (void*)Ptr0.Pin().Get(), (void*)Data.Get());
			TestEqual("ptrs equal", (void*)Ptr1.Pin().Get(), (void*)Data.Get());
		});
	});

	Describe("Ptrs equal", [this]()
	{
		It("true if same object", [this]()
		{
			const auto Data = MakeShared<FString, ESPMode::NotThreadSafe>("hi");
			const TPtr<FString> Ptr0{Data};
			const TPtr<FString> Ptr1{Ptr0};
			TestEqual("Ptrs equal", Ptr0, Ptr1);
		});
		
		It("false if different object, same contents", [this]()
		{
			const auto Data0 = MakeShared<FString, ESPMode::NotThreadSafe>("hi");
			const auto Data1 = MakeShared<FString, ESPMode::NotThreadSafe>("hi");
			const TPtr<FString> Ptr0{Data0};
			const TPtr<FString> Ptr1{Data1};
			TestNotEqual("Ptrs equal", Ptr0, Ptr1);
		});
		
		It("true if both invalid", [this]()
		{
			const TPtr<FString> Ptr0;
			const TPtr<FString> Ptr1;
			TestFalse("Ptr0 valid", Ptr0.IsValid());
			TestFalse("Ptr1 valid", Ptr1.IsValid());
			TestEqual("Ptrs equal", Ptr0, Ptr1);
		});
		
		It("false if only one invalid", [this]()
		{
			const auto Data = MakeShared<FString, ESPMode::NotThreadSafe>("hi");
			const TPtr<FString> Ptr0{Data};
			const TPtr<FString> Ptr1;
			TestTrue("Ptr0 valid", Ptr0.IsValid());
			TestFalse("Ptr1 valid", Ptr1.IsValid());
			TestNotEqual("Ptrs equal", Ptr0, Ptr1);
		});
	});
	
	Describe("Ptrs bool conversion", [this]()
	{
		It("true if same object", [this]()
		{
			const auto Data = MakeShared<FString, ESPMode::NotThreadSafe>("hi");
			const TPtr<FString> Ptr0{Data};
			if (Ptr0)
			{
				TestTrue("Ptr0 valid", static_cast<bool>(Ptr0));
			}
			Data->Reset();
			if (!Ptr0)
			{
				TestFalse("Ptr0 valid", static_cast<bool>(Ptr0));
			}
		});
	});
}
}
}
