#include "CoreMinimal.h"
#include "PragmaJson.h"
#include "Tests/PragmaAutomationSpecBase.h"
#include "TestDto.h"

namespace Pragma
{
	namespace Tests
	{
		PRAGMA_BEGIN_DEFINE_SPEC(FJsonTests, "PragmaSDK.Json")
		PRAGMA_END_DEFINE_SPEC(FJsonTests)

		namespace
		{
			const int64 LargeInt64 = MAX_int64 - 100;
			const uint64 LargeUInt64 = MAX_uint64 - 100;
			const int32 LargeInt32 = MAX_int32 - 100;
		}

		void FJsonTests::Define()
		{
			Describe("int64 from json", [this]()
			{
				It("Should successfully deserialize json string without losing precision", [this]()
				{
					FPragmaTestInt64 Data;
					const FString JsonStr = FString::Printf(TEXT("{\"value\":\"%lld\"}"), LargeInt64);
					PragmaJson::JsonStrToStruct(JsonStr, Data);
					TestEqual(TEXT("Deserializing from json string should not lose precision."), Data.Value,
						LargeInt64);
				});

				It("Should successfully deserialize JsonValue without losing precision", [this]()
				{
					const FString JsonStr = FString::Printf(TEXT("{\"value\":\"%lld\"}"), LargeInt64);
					const auto JsonObj = PragmaJson::JsonStrToObj(JsonStr);
					const int64 Value = PragmaJson::JsonValueToInt64(JsonObj->TryGetField("value").ToSharedRef());
					TestEqual(TEXT("Deserializing from json string should not lose precision."), Value, LargeInt64);
				});

				// "Control" test to ensure our number actually loses precision on conversion.
				It("Should successfully deserialize json number but lose precision", [this]()
				{
					ExpectErrorOrWarning(*this,
						TEXT(
							"Value is a int64 integer and we are trying to import a json number. This can result in loss of precision in the deserialized value."));

					FPragmaTestInt64 Data;
					const FString JsonStr = FString::Printf(TEXT("{\"value\":%lld}"), LargeInt64);
					PragmaJson::JsonStrToStruct(JsonStr, Data);
					TestNotEqual(TEXT("Deserializing from json number should lose precision."), Data.Value, LargeInt64);
				});
			});

			Describe("int64 to json", [this]()
			{
				It("Should successfully serialize to json string without losing precision", [this]()
				{
					const FPragmaTestInt64 Data{LargeInt64};
					const FString ExpectedJsonStr = FString::Printf(TEXT("{\"value\":\"%lld\"}"), LargeInt64);
					FString JsonStr;
					PragmaJson::StructToJsonStr(Data, JsonStr);
					TestEqual(TEXT("Serializing int64 to json string should not lose precision."), JsonStr,
						ExpectedJsonStr);
				});
			});

			Describe("uint64 from json", [this]()
			{
				It("Should successfully deserialize json string without losing precision", [this]()
				{
					FPragmaTestUInt64 Data;
					const FString JsonStr = FString::Printf(TEXT("{\"value\":\"%llu\"}"), LargeUInt64);
					PragmaJson::JsonStrToStruct(JsonStr, Data);
					TestEqual(TEXT("Deserializing from json string should not lose precision."), Data.Value,
						LargeUInt64);
				});

				It("Should successfully deserialize JsonValue without losing precision", [this]()
				{
					const FString JsonStr = FString::Printf(TEXT("{\"value\":\"%llu\"}"), LargeUInt64);
					const auto JsonObj = PragmaJson::JsonStrToObj(JsonStr);
					const uint64 Value = PragmaJson::JsonValueToUInt64(JsonObj->TryGetField("value").ToSharedRef());
					TestEqual(TEXT("Deserializing from json string should not lose precision."), Value, LargeUInt64);
				});

				// "Control" test to ensure our number actually loses precision on conversion.
				It("Should successfully deserialize json number but lose precision", [this]()
				{
					ExpectErrorOrWarning(*this,
						TEXT(
							"Value is a uint64 integer and we are trying to import a json number. This can result in loss of precision in the deserialized value."));

					FPragmaTestUInt64 Data;
					const FString JsonStr = FString::Printf(TEXT("{\"value\":%llu}"), LargeUInt64);
					PragmaJson::JsonStrToStruct(JsonStr, Data);
					TestNotEqual(TEXT("Deserializing from json number should lose precision."), Data.Value,
						LargeUInt64);
				});
			});

			Describe("uint64 to json", [this]()
			{
				It("Should successfully serialize to json string without losing precision", [this]()
				{
					const FPragmaTestUInt64 Data{LargeUInt64};
					const FString ExpectedJsonStr = FString::Printf(TEXT("{\"value\":\"%llu\"}"), LargeUInt64);
					FString JsonStr;
					PragmaJson::StructToJsonStr(Data, JsonStr);
					TestEqual(TEXT("Serializing uint64 to json string should not lose precision."), JsonStr,
						ExpectedJsonStr);
				});
			});

			// Ensure our custom numeric handles don't serialize int32s to/from strings.
			Describe("int32 from json", [this]()
			{
				It("Should successfully deserialize from number", [this]()
				{
					FPragmaTestInt32 Data;
					const FString JsonStr = FString::Printf(TEXT("{\"value\":%d}"), LargeInt32);
					PragmaJson::JsonStrToStruct(JsonStr, Data);
					TestEqual(TEXT("Deserializing from json number."), Data.Value, LargeInt32);
				});
			});

			Describe("int32 to json", [this]()
			{
				It("Should successfully serialize to json number", [this]()
				{
					const FPragmaTestInt32 Data{LargeInt32};
					const FString ExpectedJsonStr = FString::Printf(TEXT("{\"value\":%d}"), LargeInt32);
					FString JsonStr;
					PragmaJson::StructToJsonStr(Data, JsonStr);
					TestEqual(TEXT("Serializing int32 to json number."), JsonStr, ExpectedJsonStr);
				});
			});

			Describe("Deserialize to enum", [this]()
			{
				It("Unknown value in string should default to 0 value", [this]()
				{
					ExpectErrorOrWarning(*this,
						TEXT("Unable import enum EPragmaTestEnum from string value TOTALLY UNKNOWN STRING."));

					const EPragmaTestEnum Expected = EPragmaTestEnum::Unknown;
					FPragmaTestEnumStruct Data{EPragmaTestEnum::Two};
					PragmaJson::JsonStrToStruct(TEXT("{\"value\":\"TOTALLY UNKNOWN STRING\"}"), Data);
					TestEqual(TEXT("Deserializing unknown enum"), Data.Value, Expected);
				});

				It("Unknown value in JsonValue should default to 0 value", [this]()
				{
					ExpectErrorOrWarning(*this,
						TEXT("Unable import enum EPragmaTestEnum from string value TOTALLY UNKNOWN STRING."));

					const EPragmaTestEnum Expected = EPragmaTestEnum::Unknown;
					const auto JsonObj = PragmaJson::JsonStrToObj(TEXT("{\"value\":\"TOTALLY UNKNOWN STRING\"}"));
					const EPragmaTestEnum Actual = PragmaJson::JsonValueToEnum<EPragmaTestEnum>(
						JsonObj->TryGetField("value").ToSharedRef());
					TestEqual(TEXT("Deserializing unknown enum"), Actual, Expected);
				});
			});
		}
	}
}
