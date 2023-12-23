#include "CoreMinimal.h"
#include "PragmaJson.h"
#include "TestDto.h"
#include "Tests/PragmaAutomationSpecBase.h"

namespace
{

// Equivalent to CreateTestOptional(1);
FString ExpectedJsonStr(const int i)
{
	FString ExpectedJsonStr;
	ExpectedJsonStr.AppendChar('{');
	ExpectedJsonStr.Append(R"("optionalField":)");
	ExpectedJsonStr.AppendChar('"');
	ExpectedJsonStr.Append(Pragma::Tests::CreateStr(i));
	ExpectedJsonStr.AppendChar('"');
	ExpectedJsonStr.AppendChar('}');
	return ExpectedJsonStr;
}

// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
FPragma_TestOptional CreateTestOptional(const int i)
{
	FPragma_TestOptional TestOptional;
	TestOptional.OptionalField.SetValue(Pragma::Tests::CreateStr(i));
	return TestOptional;
}

FPragma_TestOptional CreateTestOptional()
{
	return CreateTestOptional(FMath::RandRange(0, 100000));
}
}

namespace Pragma
{
namespace Tests
{
PRAGMA_BEGIN_DEFINE_SPEC(FOptionalTests, "PragmaSDK.Optional")
PRAGMA_END_DEFINE_SPEC(FOptionalTests)

void FOptionalTests::Define()
{
	Describe("Optional", [this]()
	{
		It("HasValue on empty is false", [this]()
		{
			const FPragma_TestOptional Opt;
			TestFalse("OptionalField.HasValue()", Opt.OptionalField.HasValue());
		});

		It("HasValue Set/Reset", [this]()
		{
			FPragma_TestOptional Opt;
			Opt.OptionalField.SetValue("hi");
			TestTrue("OptionalField.HasValue()", Opt.OptionalField.HasValue());
			Opt.OptionalField.Reset();
			TestFalse("OptionalField.HasValue()", Opt.OptionalField.HasValue());
		});
		
		It("SetValue", [this]()
		{
			FPragma_TestOptional Opt;
			Opt.OptionalField.SetValue("hi");
			TestEqual("Opt Value", Opt.OptionalField.Value(), "hi");
		});

		It("operator= empty", [this]()
		{
			FPragma_TestOptional Opt0;
			FPragma_TestOptional Opt1;
			
			Opt0.OptionalField.SetValue("hi");
			Opt0 = Opt1;
			TestFalse("Opt Value", Opt0.OptionalField.HasValue());
		});

		It("operator= value", [this]()
		{
			FPragma_TestOptional Opt0;
			FPragma_TestOptional Opt1;
				
			Opt1.OptionalField.SetValue("hi");
			Opt0 = Opt1;
			TestEqual("Opt Value", Opt0.OptionalField.Value(), "hi");
		});

		It("Equals empty", [this]()
		{
			FPragma_TestOptional Opt0;
			FPragma_TestOptional Opt1;
			TestEqual("Opt Value", Opt0, Opt1);
		});

		It("Equals one empty", [this]()
		{
			FPragma_TestOptional Opt0;
			FPragma_TestOptional Opt1;
								
			Opt1.OptionalField.SetValue("hi");
			TestNotEqual("Opt Value", Opt0, Opt1);

			Opt0.OptionalField.SetValue("hi");
			Opt1.OptionalField.Reset();
			TestNotEqual("Opt Value", Opt0, Opt1);
		});

		It("Equals different values", [this]()
		{
			FPragma_TestOptional Opt0;
			FPragma_TestOptional Opt1;
										
			Opt0.OptionalField.SetValue("hello");
			Opt1.OptionalField.SetValue("hi");
			TestNotEqual("Opt Value", Opt0, Opt1);
		});

		It("Equals same value", [this]()
		{
			FPragma_TestOptional Opt0;
			FPragma_TestOptional Opt1;
												
			Opt0.OptionalField.SetValue("hi");
			Opt1.OptionalField.SetValue("hi");
			TestEqual("Opt Value", Opt0, Opt1);
		});
		
		It("serializes to json correctly", [this]()
		{
			const auto Struct = CreateTestOptional(1);
			FString JsonStr;
			TestTrue("Optional StructToJsonStr", PragmaJson::StructToJsonStr(Struct, JsonStr));
			TestEqual("Optional as json str", JsonStr, ExpectedJsonStr(1));
		});

		It("deserializes from json correctly", [this]()
        {
			const FPragma_TestOptional Expected = CreateTestOptional(1);
            FPragma_TestOptional Actual;
            TestTrue("Optional StructToJsonStr", PragmaJson::JsonStrToStruct(ExpectedJsonStr(1), Actual));
			TestTrue("Optional deserialized from json str", Actual == Expected);
        });
	});

}
}
}
