#include "CoreMinimal.h"

#include "Tests/PragmaAutomationSpecBase.h"
#include "PragmaTimerManager.h"

namespace Pragma
{
namespace Tests
{
PRAGMA_BEGIN_DEFINE_SPEC(TPragmaTimerManagerTest, "PragmaSDK.UnitTests.TimerManager")

	TUniquePtr<FPragmaTimerManager> TimerManager;

	FPragmaTimerDelegate IncrementDelegate(int& CallCount)
	{
		return FPragmaTimerDelegate::CreateLambda([&]()
		{
			CallCount++;
		});
	}

PRAGMA_END_DEFINE_SPEC(TPragmaTimerManagerTest)

void TPragmaTimerManagerTest::Define()
{
	BeforeEach([this]()
	{
		TimerManager = MakeUnique<FPragmaTimerManager>();
	});

	Describe("SetTimer", [this]()
	{
		It("basic", [this]()
		{
			const float Duration = 1.f;
			const bool IsLooping = false;
			FPragmaTimerHandle Handle;
			int CallCount = 0;
			TimerManager->SetTimer(Handle, IncrementDelegate(CallCount), Duration, IsLooping);
			
			TimerManager->Tick(Duration * 0.5f);
			TestEqual("Call count", CallCount, 0);
			
			TimerManager->Tick(Duration);
			TestEqual("Call count", CallCount, 1);
			
			TimerManager->Tick(Duration);
			TestEqual("Call count", CallCount, 1);
		});
		
		It("looping", [this]()
		{
			const float Duration = 1.f;
			const bool IsLooping = true;
			FPragmaTimerHandle Handle;
			int CallCount = 0;
			TimerManager->SetTimer(Handle, IncrementDelegate(CallCount), Duration, IsLooping);
			
			TimerManager->Tick(Duration * 0.5f); // .5
			TestEqual("Call count", CallCount, 0);
			
			TimerManager->Tick(Duration); // 1.5
			TestEqual("Call count", CallCount, 1);

			TimerManager->Tick(Duration * 2.f); // 3.5
			TestEqual("Call count", CallCount, 3);
		});

		It("looping account for spillover", [this]()
		{
			const float Duration = 1.f;
			const bool IsLooping = true;
			FPragmaTimerHandle Handle;
			int CallCount = 0;
			TimerManager->SetTimer(Handle, IncrementDelegate(CallCount), Duration, IsLooping);
				
			TimerManager->Tick(Duration * 1.3f); // 1.3
			TestEqual("Call count", CallCount, 1);
				
			TimerManager->Tick(Duration * 1.3f); // 2.6
			TestEqual("Call count", CallCount, 2);

			TimerManager->Tick(Duration * 1.3f); // 3.9
			TestEqual("Call count", CallCount, 3);
			
			TimerManager->Tick(Duration * 1.3f); // 5.2
			TestEqual("Call count", CallCount, 5); // <- 3 to 5 due to spillover.
		});
		
		It("With initial delay", [this]()
		{
			const float Duration = 1.f;
			const bool IsLooping = false;
			FPragmaTimerHandle Handle;
			int CallCount = 0;
			TimerManager->SetTimer(Handle, IncrementDelegate(CallCount), Duration, IsLooping, Duration * 2.f);

			TimerManager->Tick(Duration * 1.5f); // -.5
			TestEqual("Call count", CallCount, 0);
			
			TimerManager->Tick(Duration); // .5
			TestEqual("Call count", CallCount, 0);
			
			TimerManager->Tick(Duration); // 1.5
			TestEqual("Call count", CallCount, 1);
		});
		
		It("looping with initial delay", [this]()
		{
			const float Duration = 1.f;
			const bool IsLooping = true;
			FPragmaTimerHandle Handle;
			int CallCount = 0;
			TimerManager->SetTimer(Handle, IncrementDelegate(CallCount), Duration, IsLooping, Duration * 2.f);
			
			TimerManager->Tick(Duration * 1.5f);
			TestEqual("Call count", CallCount, 0);
			
			TimerManager->Tick(Duration);
			TestEqual("Call count", CallCount, 0);
			
			TimerManager->Tick(Duration);
			TestEqual("Call count", CallCount, 1);
			
			TimerManager->Tick(Duration * 2.f);
			TestEqual("Call count", CallCount, 3);
		});
	});

	Describe("ClearTimer", [this]()
	{
		It("clear before full duration doesn't fire", [this]()
		{
			const float Duration = 1.f;
			const bool IsLooping = false;
			FPragmaTimerHandle Handle;
			int CallCount = 0;
			TimerManager->SetTimer(Handle, IncrementDelegate(CallCount), Duration, IsLooping);
			
			TimerManager->Tick(Duration * 0.5f);
			TestEqual("Call count", CallCount, 0);
			
			TimerManager->ClearTimer(Handle);
			TimerManager->Tick(Duration);
			TestEqual("Call count", CallCount, 0);
		});
		
		It("clear looping", [this]()
		{
			const float Duration = 1.f;
			const bool IsLooping = true;
			FPragmaTimerHandle Handle;
			int CallCount = 0;
			TimerManager->SetTimer(Handle, IncrementDelegate(CallCount), Duration, IsLooping);
			
			TimerManager->Tick(Duration * 1.5f);
			TestEqual("Call count", CallCount, 1);
			
			TimerManager->ClearTimer(Handle);
			TimerManager->Tick(Duration * 5.f);
			TestEqual("Call count", CallCount, 1);
		});
	});

	Describe("IsTimerActive", [this]()
	{
		It("true after timer is set", [this]()
		{
			const float Duration = 1.f;
			const bool IsLooping = false;
			FPragmaTimerHandle Handle;
			int CallCount = 0;
			TimerManager->SetTimer(Handle, IncrementDelegate(CallCount), Duration, IsLooping);
			TestTrue("IsTimerActive", TimerManager->IsTimerActive(Handle));
		});
		
		It("false after timer ends", [this]()
		{
			const float Duration = 1.f;
			const bool IsLooping = false;
			FPragmaTimerHandle Handle;
			int CallCount = 0;
			TimerManager->SetTimer(Handle, IncrementDelegate(CallCount), Duration, IsLooping);
			TimerManager->Tick(Duration * 5.0f);
			TestFalse("IsTimerActive", TimerManager->IsTimerActive(Handle));
		});
		
		It("false for invalid timer", [this]()
		{
			const float Duration = 1.f;
			const bool IsLooping = false;
			FPragmaTimerHandle Handle;
			int CallCount = 0;
			TestFalse("IsTimerActive", TimerManager->IsTimerActive(Handle));
			TimerManager->SetTimer(Handle, IncrementDelegate(CallCount), Duration, IsLooping);
			TestTrue("IsTimerActive", TimerManager->IsTimerActive(Handle));
		});
	});

	Describe("Tick", [this]()
	{
		It("modifies all timers based on remaining time", [this]()
		{
			const float Duration = 1.f;
			const bool IsLooping = false;
			FPragmaTimerHandle Handle0;
			FPragmaTimerHandle Handle1;
			FPragmaTimerHandle Handle2;
			int CallCount = 0;
			TimerManager->SetTimer(Handle0, IncrementDelegate(CallCount), Duration * 1.f, IsLooping);
			TimerManager->SetTimer(Handle1, IncrementDelegate(CallCount), Duration * 2.f, IsLooping);
			TimerManager->SetTimer(Handle2, IncrementDelegate(CallCount), Duration * 4.f, IsLooping);
			
			TimerManager->Tick(Duration * 0.5f);
			TestTrue("IsTimerActive 0", TimerManager->IsTimerActive(Handle0));
			TestTrue("IsTimerActive 1", TimerManager->IsTimerActive(Handle1));
			TestTrue("IsTimerActive 2", TimerManager->IsTimerActive(Handle2));
			
			TimerManager->Tick(Duration);
			TestFalse("IsTimerActive 0", TimerManager->IsTimerActive(Handle0));
			TestTrue("IsTimerActive 1", TimerManager->IsTimerActive(Handle1));
			TestTrue("IsTimerActive 2", TimerManager->IsTimerActive(Handle2));
			
			TimerManager->Tick(Duration);
			TestFalse("IsTimerActive 0", TimerManager->IsTimerActive(Handle0));
			TestFalse("IsTimerActive 1", TimerManager->IsTimerActive(Handle1));
			TestTrue("IsTimerActive 2", TimerManager->IsTimerActive(Handle2));
			
			TimerManager->Tick(Duration);
			TestFalse("IsTimerActive 0", TimerManager->IsTimerActive(Handle0));
			TestFalse("IsTimerActive 1", TimerManager->IsTimerActive(Handle1));
			TestTrue("IsTimerActive 2", TimerManager->IsTimerActive(Handle2));
			
			TimerManager->Tick(Duration);
			TestFalse("IsTimerActive 0", TimerManager->IsTimerActive(Handle0));
			TestFalse("IsTimerActive 1", TimerManager->IsTimerActive(Handle1));
			TestFalse("IsTimerActive 2", TimerManager->IsTimerActive(Handle2));
		});
	});
}
}
}
