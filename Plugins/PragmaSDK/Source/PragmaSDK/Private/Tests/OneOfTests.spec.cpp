#include "CoreMinimal.h"
#include "PragmaJson.h"
#include "TestDto.h"
#include "Tests/PragmaAutomationSpecBase.h"

namespace
{

// Equivalent to CreateTestOneOf(1);
const FString& ExpectedJsonStr()
{
	static const FString ExpectedJsonStr = TEXT(R"({"outsideStr":"1","outsideInt32":1,"outsideInt64":"1","outsideUint32":1,"outsideUint64":"1","outsideFloat":1,"outsideDouble":1,"outsideEnum":"One","outsideFixed128":"fixed128_1","outsideStruct":{"outsideStr":"1","outsideInt32":1,"outsideInt64":"1","outsideUint32":1,"outsideUint64":"1","outsideFloat":1,"outsideDouble":1,"outsideFixed128":"fixed128_1","outsideEnum":"One","outsideStruct":{"innerStr":"1","innerInt32":1,"innerInt64":"1","innerUint32":1,"innerUint64":"1","innerFloat":1,"innerDouble":1,"innerEnum":"One","innerFixed128":"fixed128_1"},"outsideArray":[{"innerStr":"1","innerInt32":1,"innerInt64":"1","innerUint32":1,"innerUint64":"1","innerFloat":1,"innerDouble":1,"innerEnum":"One","innerFixed128":"fixed128_1"},{"innerStr":"2","innerInt32":2,"innerInt64":"2","innerUint32":2,"innerUint64":"2","innerFloat":2,"innerDouble":2,"innerEnum":"Two","innerFixed128":"fixed128_2"}],"outsideMap":{"1":{"innerStr":"1","innerInt32":1,"innerInt64":"1","innerUint32":1,"innerUint64":"1","innerFloat":1,"innerDouble":1,"innerEnum":"One","innerFixed128":"fixed128_1"},"2":{"innerStr":"2","innerInt32":2,"innerInt64":"2","innerUint32":2,"innerUint64":"2","innerFloat":2,"innerDouble":2,"innerEnum":"Two","innerFixed128":"fixed128_2"}},"insideInt32":1},"outsideArray":[{"outsideStr":"1","outsideInt32":1,"outsideInt64":"1","outsideUint32":1,"outsideUint64":"1","outsideFloat":1,"outsideDouble":1,"outsideFixed128":"fixed128_1","outsideEnum":"One","outsideStruct":{"innerStr":"1","innerInt32":1,"innerInt64":"1","innerUint32":1,"innerUint64":"1","innerFloat":1,"innerDouble":1,"innerEnum":"One","innerFixed128":"fixed128_1"},"outsideArray":[{"innerStr":"1","innerInt32":1,"innerInt64":"1","innerUint32":1,"innerUint64":"1","innerFloat":1,"innerDouble":1,"innerEnum":"One","innerFixed128":"fixed128_1"},{"innerStr":"2","innerInt32":2,"innerInt64":"2","innerUint32":2,"innerUint64":"2","innerFloat":2,"innerDouble":2,"innerEnum":"Two","innerFixed128":"fixed128_2"}],"outsideMap":{"1":{"innerStr":"1","innerInt32":1,"innerInt64":"1","innerUint32":1,"innerUint64":"1","innerFloat":1,"innerDouble":1,"innerEnum":"One","innerFixed128":"fixed128_1"},"2":{"innerStr":"2","innerInt32":2,"innerInt64":"2","innerUint32":2,"innerUint64":"2","innerFloat":2,"innerDouble":2,"innerEnum":"Two","innerFixed128":"fixed128_2"}},"insideInt32":1},{"outsideStr":"2","outsideInt32":2,"outsideInt64":"2","outsideUint32":2,"outsideUint64":"2","outsideFloat":2,"outsideDouble":2,"outsideFixed128":"fixed128_2","outsideEnum":"Two","outsideStruct":{"innerStr":"2","innerInt32":2,"innerInt64":"2","innerUint32":2,"innerUint64":"2","innerFloat":2,"innerDouble":2,"innerEnum":"Two","innerFixed128":"fixed128_2"},"outsideArray":[{"innerStr":"2","innerInt32":2,"innerInt64":"2","innerUint32":2,"innerUint64":"2","innerFloat":2,"innerDouble":2,"innerEnum":"Two","innerFixed128":"fixed128_2"},{"innerStr":"3","innerInt32":3,"innerInt64":"3","innerUint32":3,"innerUint64":"3","innerFloat":3,"innerDouble":3,"innerEnum":"Zero","innerFixed128":"fixed128_3"}],"outsideMap":{"2":{"innerStr":"2","innerInt32":2,"innerInt64":"2","innerUint32":2,"innerUint64":"2","innerFloat":2,"innerDouble":2,"innerEnum":"Two","innerFixed128":"fixed128_2"},"3":{"innerStr":"3","innerInt32":3,"innerInt64":"3","innerUint32":3,"innerUint64":"3","innerFloat":3,"innerDouble":3,"innerEnum":"Zero","innerFixed128":"fixed128_3"}},"insideInt32":2}],"outsideMap":{"1":{"outsideStr":"1","outsideInt32":1,"outsideInt64":"1","outsideUint32":1,"outsideUint64":"1","outsideFloat":1,"outsideDouble":1,"outsideFixed128":"fixed128_1","outsideEnum":"One","outsideStruct":{"innerStr":"1","innerInt32":1,"innerInt64":"1","innerUint32":1,"innerUint64":"1","innerFloat":1,"innerDouble":1,"innerEnum":"One","innerFixed128":"fixed128_1"},"outsideArray":[{"innerStr":"1","innerInt32":1,"innerInt64":"1","innerUint32":1,"innerUint64":"1","innerFloat":1,"innerDouble":1,"innerEnum":"One","innerFixed128":"fixed128_1"},{"innerStr":"2","innerInt32":2,"innerInt64":"2","innerUint32":2,"innerUint64":"2","innerFloat":2,"innerDouble":2,"innerEnum":"Two","innerFixed128":"fixed128_2"}],"outsideMap":{"1":{"innerStr":"1","innerInt32":1,"innerInt64":"1","innerUint32":1,"innerUint64":"1","innerFloat":1,"innerDouble":1,"innerEnum":"One","innerFixed128":"fixed128_1"},"2":{"innerStr":"2","innerInt32":2,"innerInt64":"2","innerUint32":2,"innerUint64":"2","innerFloat":2,"innerDouble":2,"innerEnum":"Two","innerFixed128":"fixed128_2"}},"insideInt32":1},"2":{"outsideStr":"2","outsideInt32":2,"outsideInt64":"2","outsideUint32":2,"outsideUint64":"2","outsideFloat":2,"outsideDouble":2,"outsideFixed128":"fixed128_2","outsideEnum":"Two","outsideStruct":{"innerStr":"2","innerInt32":2,"innerInt64":"2","innerUint32":2,"innerUint64":"2","innerFloat":2,"innerDouble":2,"innerEnum":"Two","innerFixed128":"fixed128_2"},"outsideArray":[{"innerStr":"2","innerInt32":2,"innerInt64":"2","innerUint32":2,"innerUint64":"2","innerFloat":2,"innerDouble":2,"innerEnum":"Two","innerFixed128":"fixed128_2"},{"innerStr":"3","innerInt32":3,"innerInt64":"3","innerUint32":3,"innerUint64":"3","innerFloat":3,"innerDouble":3,"innerEnum":"Zero","innerFixed128":"fixed128_3"}],"outsideMap":{"2":{"innerStr":"2","innerInt32":2,"innerInt64":"2","innerUint32":2,"innerUint64":"2","innerFloat":2,"innerDouble":2,"innerEnum":"Two","innerFixed128":"fixed128_2"},"3":{"innerStr":"3","innerInt32":3,"innerInt64":"3","innerUint32":3,"innerUint64":"3","innerFloat":3,"innerDouble":3,"innerEnum":"Zero","innerFixed128":"fixed128_3"}},"insideInt32":2}},"insideInt32":1})");
	return ExpectedJsonStr;
}

// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
FString CreateFixed128(const int i)
{
	return FString::Printf(TEXT("fixed128_%d"), i);
}

// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
EPragma_TestOneOfEnum CreateEnum(const int i)
{
	return static_cast<EPragma_TestOneOfEnum>(static_cast<uint8>(i) % static_cast<uint8>(EPragma_TestOneOfEnum::Last));
}

// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
FPragma_TestOneOfInnerStruct CreateTestOneOfInnerStruct(const int i)
{
	FPragma_TestOneOfInnerStruct InnerStruct;
	InnerStruct.InnerStr = Pragma::Tests::CreateStr(i);
	InnerStruct.InnerInt32 = i;
	InnerStruct.InnerInt64 = i;
	InnerStruct.InnerUint32 = i;
	InnerStruct.InnerUint64 = i;
	InnerStruct.InnerFloat = static_cast<float>(i);
	InnerStruct.InnerDouble = i;
	InnerStruct.InnerEnum = CreateEnum(i);
	InnerStruct.InnerFixed128 = CreateFixed128(i);
	return InnerStruct;
}

// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
FPragma_TestOneOfStruct CreateTestOneOfStruct(const int i)
{
	FPragma_TestOneOfStruct Struct;
	Struct.OutsideStr = Pragma::Tests::CreateStr(i);
	Struct.OutsideInt32 = i;
	Struct.OutsideInt64 = i;
	Struct.OutsideUint32 = i;
	Struct.OutsideUint64 = i;
	Struct.OutsideFloat = static_cast<float>(i);
	Struct.OutsideDouble = i;
	Struct.OutsideEnum = CreateEnum(i);
	Struct.OutsideFixed128 = CreateFixed128(i);
	Struct.OutsideStruct = CreateTestOneOfInnerStruct(i);
	Struct.OutsideArray = {CreateTestOneOfInnerStruct(i), CreateTestOneOfInnerStruct(i + 1)};
	Struct.OutsideMap = {
		{Pragma::Tests::CreateStr(i), CreateTestOneOfInnerStruct(i)},
		{Pragma::Tests::CreateStr(i + 1), CreateTestOneOfInnerStruct(i + 1)},
	};
	Struct.Inside.SetInsideInt32(i);
	return Struct;
}

// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
FPragma_TestOneOf CreateTestOneOf(const int i)
{
	FPragma_TestOneOf TestOneOf;
	TestOneOf.OutsideStr = Pragma::Tests::CreateStr(i);
	TestOneOf.OutsideInt32 = i;
	TestOneOf.OutsideInt64 = i;
	TestOneOf.OutsideUint32 = i;
	TestOneOf.OutsideUint64 = i;
	TestOneOf.OutsideFloat = static_cast<float>(i);
	TestOneOf.OutsideDouble = i;
	TestOneOf.OutsideEnum = CreateEnum(i);
	TestOneOf.OutsideFixed128 = CreateFixed128(i);
	TestOneOf.OutsideStruct = CreateTestOneOfStruct(i);
	TestOneOf.OutsideStruct = CreateTestOneOfStruct(i);
	TestOneOf.OutsideArray = {CreateTestOneOfStruct(i), CreateTestOneOfStruct(i + 1)};
	TestOneOf.OutsideMap = {
		{Pragma::Tests::CreateStr(i), CreateTestOneOfStruct(i)},
		{Pragma::Tests::CreateStr(i + 1), CreateTestOneOfStruct(i + 1)},
	};
	TestOneOf.Inside.SetInsideInt32(i);
	return TestOneOf;
}

FPragma_TestOneOf CreateTestOneOf()
{
	return CreateTestOneOf(FMath::RandRange(0, 100000));
}
}

namespace Pragma
{
namespace Tests
{
PRAGMA_BEGIN_DEFINE_SPEC(FOneOfTests, "PragmaSDK.OneOf")

void VerifyToFromJson(const FPragma_TestOneOf& Struct)
{
	FString JsonStr;
	TestTrue("OneOf StructToJsonStr", PragmaJson::StructToJsonStr(Struct, JsonStr));
	FPragma_TestOneOf Actual;
	TestTrue("OneOf serialized to json successfully", PragmaJson::JsonStrToStruct(JsonStr, Actual));
	TestTrue("OneOf deserialized from json str", Actual == Struct);
}

PRAGMA_END_DEFINE_SPEC(FOneOfTests)

void FOneOfTests::Define()
{
	//
	// The host of different nested types in the struct is important to verify that at whatever nesting layer, we are properly going through our own de/serialization
	// flow that hits the necessary generated oneof de/serializer functions.
	//

	// Test the overall struct-with-oneof in a complex scenario.
	Describe("Complex nested oneof struct", [this]()
	{
		It("serializes to json correctly", [this]()
		{
			const auto Struct = CreateTestOneOf(1);
			FString JsonStr;
			TestTrue("OneOf StructToJsonStr", PragmaJson::StructToJsonStr(Struct, JsonStr));
			TestEqual("OneOf as json str", JsonStr, ExpectedJsonStr());
		});

		It("deserializes from json correctly", [this]()
        {
			const FPragma_TestOneOf Expected = CreateTestOneOf(1);
            FPragma_TestOneOf Actual;
            TestTrue("OneOf StructToJsonStr", PragmaJson::JsonStrToStruct(ExpectedJsonStr(), Actual));
			TestTrue("OneOf deserialized from json str", Actual == Expected);
        });
	});

	// Test individual properties of the oneof.
	Describe("OneOf", [this]()
	{
        It("de/serializes int32 correctly", [this]()
        {
        	auto Struct = CreateTestOneOf();
        	Struct.Inside.SetInsideInt32(TNumericLimits<int32>::Max() - 100);
        	VerifyToFromJson(Struct);
        });

		It("de/serializes int64 correctly", [this]()
        {
        	auto Struct = CreateTestOneOf();
        	Struct.Inside.SetInsideInt64(TNumericLimits<int64>::Max() - 100);
        	VerifyToFromJson(Struct);
        });

		It("de/serializes uint32 correctly", [this]()
        {
        	auto Struct = CreateTestOneOf();
        	Struct.Inside.SetInsideUint32(TNumericLimits<uint32>::Max() - 100);
        	VerifyToFromJson(Struct);
        });

		It("de/serializes uint64 correctly", [this]()
        {
        	auto Struct = CreateTestOneOf();
        	Struct.Inside.SetInsideUint64(TNumericLimits<uint64>::Max() - 100);
        	VerifyToFromJson(Struct);
        });

		It("de/serializes float correctly", [this]()
        {
        	auto Struct = CreateTestOneOf();
        	Struct.Inside.SetInsideFloat(TNumericLimits<float>::Max() - 100);
        	VerifyToFromJson(Struct);
        });

		It("de/serializes double correctly", [this]()
        {
        	auto Struct = CreateTestOneOf();
        	Struct.Inside.SetInsideDouble(TNumericLimits<double>::Max() - 100);
        	VerifyToFromJson(Struct);
        });

		It("de/serializes string correctly", [this]()
        {
        	auto Struct = CreateTestOneOf();
        	Struct.Inside.SetInsideStr("Hello 你好!");
        	VerifyToFromJson(Struct);
        });

		It("de/serializes struct correctly", [this]()
        {
        	auto Struct = CreateTestOneOf();
        	Struct.Inside.SetInsideStruct(CreateTestOneOfStruct(12345));
        	VerifyToFromJson(Struct);
        });

		It("de/serializes enum correctly", [this]()
        {
        	auto Struct = CreateTestOneOf();
        	Struct.Inside.SetInsideEnum(EPragma_TestOneOfEnum::Two);
        	VerifyToFromJson(Struct);
        });
	});
}
}
}
