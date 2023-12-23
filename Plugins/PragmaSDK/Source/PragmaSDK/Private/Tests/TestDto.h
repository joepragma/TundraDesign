#pragma once

#include "CoreMinimal.h"

#include "Dom/JsonObject.h"


#include "TestDto.generated.h"

// Arbitrary data structs for tests.
USTRUCT()
struct FPragmaTestRequestPayload
{
	GENERATED_BODY()

	UPROPERTY()
	FString ReqStr;
};
USTRUCT()
struct FPragmaTestResponsePayload
{
	GENERATED_BODY()

	UPROPERTY()
	FString ResStr;
};
USTRUCT()
struct FPragmaTestNotificationPayload0
{
	GENERATED_BODY()

	UPROPERTY()
	FString ResStr;
};
USTRUCT()
struct FPragmaTestNotificationPayload1
{
	GENERATED_BODY()

	UPROPERTY()
	FString ResStr;
};

USTRUCT()
struct FPragmaTestInt64
{
	GENERATED_BODY()

	UPROPERTY()
	int64 Value{0};
};

USTRUCT()
struct FPragmaTestUInt64
{
	GENERATED_BODY()

	UPROPERTY()
	uint64 Value{0};
};

USTRUCT()
struct FPragmaTestInt32
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Value{0};
};

UENUM()
enum class EPragmaTestEnum
{
	Unknown,
	One,
	Two,
};

USTRUCT()
struct FPragmaTestEnumStruct
{
	GENERATED_BODY()

	UPROPERTY()
	EPragmaTestEnum Value{EPragmaTestEnum::Unknown};
};

UENUM(BlueprintType, Category=Pragma)
enum class EPragma_TestOneOfEnum : uint8
{
	Zero = 0,
	One = 1,
	Two = 2,
	Last = 3,
};

//
// OneOf test structs/enum below were generated via code gen.
// The operator== were manually added.
//

USTRUCT(BlueprintType)
struct FPragma_TestOneOfInnerStruct
{
	GENERATED_BODY()

	UPROPERTY()
	FString InnerStr;

	UPROPERTY()
	int32 InnerInt32{0};

	UPROPERTY()
	int64 InnerInt64{0};

	UPROPERTY()
	uint32 InnerUint32{0};

	UPROPERTY()
	uint64 InnerUint64{0};

	UPROPERTY()
	float InnerFloat{0.0f};

	UPROPERTY()
	double InnerDouble{0.0};

	UPROPERTY()
	EPragma_TestOneOfEnum InnerEnum{EPragma_TestOneOfEnum::Zero};

	UPROPERTY()
	FString InnerFixed128;

	bool operator==(const FPragma_TestOneOfInnerStruct& Other) const;
	bool operator!=(const FPragma_TestOneOfInnerStruct& Other) const { return !operator==(Other); }
};

UENUM(BlueprintType, Category=Pragma)
enum class EPragma_TestOneOfStruct_InsideType : uint8
{
	Invalid,
	InsideStr,
	InsideInt32,
	InsideInt64,
	InsideUint32,
	InsideUint64,
	InsideFloat,
	InsideDouble,
	InsideFixed128,
	InsideEnum,
	InsideStruct,
};

USTRUCT(BlueprintType)
struct PRAGMASDK_API FPragma_TestOneOfStruct_Inside
{
	GENERATED_BODY()

	FPragma_TestOneOfStruct_Inside();
	~FPragma_TestOneOfStruct_Inside();

	FPragma_TestOneOfStruct_Inside(const FPragma_TestOneOfStruct_Inside& Other);
	FPragma_TestOneOfStruct_Inside& operator=(const FPragma_TestOneOfStruct_Inside& Other);

	void Reset();

	EPragma_TestOneOfStruct_InsideType OneOfType() const { return Type; }

	const FString& InsideStr() const;
	const int32& InsideInt32() const;
	const int64& InsideInt64() const;
	const uint32& InsideUint32() const;
	const uint64& InsideUint64() const;
	const float& InsideFloat() const;
	const double& InsideDouble() const;
	const FString& InsideFixed128() const;
	const EPragma_TestOneOfEnum& InsideEnum() const;
	const FPragma_TestOneOfInnerStruct& InsideStruct() const;

	void SetInsideStr(const FString& Value);
	void SetInsideInt32(const int32& Value);
	void SetInsideInt64(const int64& Value);
	void SetInsideUint32(const uint32& Value);
	void SetInsideUint64(const uint64& Value);
	void SetInsideFloat(const float& Value);
	void SetInsideDouble(const double& Value);
	void SetInsideFixed128(const FString& Value);
	void SetInsideEnum(const EPragma_TestOneOfEnum& Value);
	void SetInsideStruct(const FPragma_TestOneOfInnerStruct& Value);

	static bool SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
	static bool DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);

private:
	UPROPERTY()
	EPragma_TestOneOfStruct_InsideType Type { EPragma_TestOneOfStruct_InsideType::Invalid };

	union
	{
		TTypeCompatibleBytes<FString> InsideStr;
		TTypeCompatibleBytes<int32> InsideInt32;
		TTypeCompatibleBytes<int64> InsideInt64;
		TTypeCompatibleBytes<uint32> InsideUint32;
		TTypeCompatibleBytes<uint64> InsideUint64;
		TTypeCompatibleBytes<float> InsideFloat;
		TTypeCompatibleBytes<double> InsideDouble;
		TTypeCompatibleBytes<FString> InsideFixed128;
		TTypeCompatibleBytes<EPragma_TestOneOfEnum> InsideEnum;
		TTypeCompatibleBytes<FPragma_TestOneOfInnerStruct> InsideStruct;
	} Union{};

public:
	bool operator==(const FPragma_TestOneOfStruct_Inside& Other) const;
	bool operator!=(const FPragma_TestOneOfStruct_Inside& Other) const { return !operator==(Other); }
};

USTRUCT(BlueprintType)
struct FPragma_TestOneOfStruct
{
	GENERATED_BODY()

	UPROPERTY()
	FString OutsideStr;

	UPROPERTY()
	int32 OutsideInt32{0};

	UPROPERTY()
	int64 OutsideInt64{0};

	UPROPERTY()
	uint32 OutsideUint32{0};

	UPROPERTY()
	uint64 OutsideUint64{0};

	UPROPERTY()
	float OutsideFloat{0.0f};

	UPROPERTY()
	double OutsideDouble{0.0};

	UPROPERTY()
	FString OutsideFixed128;

	UPROPERTY()
	EPragma_TestOneOfEnum OutsideEnum{EPragma_TestOneOfEnum::Zero};

	UPROPERTY()
	FPragma_TestOneOfInnerStruct OutsideStruct;

	UPROPERTY()
	TArray<FPragma_TestOneOfInnerStruct> OutsideArray;

	UPROPERTY()
	TMap<FString, FPragma_TestOneOfInnerStruct> OutsideMap;

	UPROPERTY(Transient)
	FPragma_TestOneOfStruct_Inside Inside;

	PRAGMASDK_API static bool AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
	PRAGMASDK_API static bool AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);

	bool operator==(const FPragma_TestOneOfStruct& Other) const;
	bool operator!=(const FPragma_TestOneOfStruct& Other) const { return !operator==(Other); }
};

UENUM(BlueprintType, Category=Pragma)
enum class EPragma_TestOneOf_InsideType : uint8
{
	Invalid,
	InsideStr,
	InsideInt32,
	InsideInt64,
	InsideUint32,
	InsideUint64,
	InsideFloat,
	InsideDouble,
	InsideFixed128,
	InsideEnum,
	InsideStruct,
};

USTRUCT(BlueprintType)
struct PRAGMASDK_API FPragma_TestOneOf_Inside
{
	GENERATED_BODY()

	FPragma_TestOneOf_Inside();
	~FPragma_TestOneOf_Inside();

	FPragma_TestOneOf_Inside(const FPragma_TestOneOf_Inside& Other);
	FPragma_TestOneOf_Inside& operator=(const FPragma_TestOneOf_Inside& Other);

	void Reset();

	EPragma_TestOneOf_InsideType OneOfType() const { return Type; }

	const FString& InsideStr() const;
	const int32& InsideInt32() const;
	const int64& InsideInt64() const;
	const uint32& InsideUint32() const;
	const uint64& InsideUint64() const;
	const float& InsideFloat() const;
	const double& InsideDouble() const;
	const FString& InsideFixed128() const;
	const EPragma_TestOneOfEnum& InsideEnum() const;
	const FPragma_TestOneOfStruct& InsideStruct() const;

	void SetInsideStr(const FString& Value);
	void SetInsideInt32(const int32& Value);
	void SetInsideInt64(const int64& Value);
	void SetInsideUint32(const uint32& Value);
	void SetInsideUint64(const uint64& Value);
	void SetInsideFloat(const float& Value);
	void SetInsideDouble(const double& Value);
	void SetInsideFixed128(const FString& Value);
	void SetInsideEnum(const EPragma_TestOneOfEnum& Value);
	void SetInsideStruct(const FPragma_TestOneOfStruct& Value);

	static bool SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
	static bool DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);

private:
	UPROPERTY()
	EPragma_TestOneOf_InsideType Type { EPragma_TestOneOf_InsideType::Invalid };

	union
	{
		TTypeCompatibleBytes<FString> InsideStr;
		TTypeCompatibleBytes<int32> InsideInt32;
		TTypeCompatibleBytes<int64> InsideInt64;
		TTypeCompatibleBytes<uint32> InsideUint32;
		TTypeCompatibleBytes<uint64> InsideUint64;
		TTypeCompatibleBytes<float> InsideFloat;
		TTypeCompatibleBytes<double> InsideDouble;
		TTypeCompatibleBytes<FString> InsideFixed128;
		TTypeCompatibleBytes<EPragma_TestOneOfEnum> InsideEnum;
		TTypeCompatibleBytes<FPragma_TestOneOfStruct> InsideStruct;
	} Union{};

public:
	bool operator==(const FPragma_TestOneOf_Inside& Other) const;
	bool operator!=(const FPragma_TestOneOf_Inside& Other) const { return !operator==(Other); }
};

USTRUCT(BlueprintType)
struct FPragma_TestOneOf
{
	GENERATED_BODY()

	UPROPERTY()
	FString OutsideStr;

	UPROPERTY()
	int32 OutsideInt32{0};

	UPROPERTY()
	int64 OutsideInt64{0};

	UPROPERTY()
	uint32 OutsideUint32{0};

	UPROPERTY()
	uint64 OutsideUint64{0};

	UPROPERTY()
	float OutsideFloat{0.0f};

	UPROPERTY()
	double OutsideDouble{0.0};

	UPROPERTY()
	EPragma_TestOneOfEnum OutsideEnum{EPragma_TestOneOfEnum::Zero};

	UPROPERTY()
	FString OutsideFixed128;

	UPROPERTY()
	FPragma_TestOneOfStruct OutsideStruct;

	UPROPERTY()
	TArray<FPragma_TestOneOfStruct> OutsideArray;

	UPROPERTY()
	TMap<FString, FPragma_TestOneOfStruct> OutsideMap;

	UPROPERTY(Transient)
	FPragma_TestOneOf_Inside Inside;

	PRAGMASDK_API static bool AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
	PRAGMASDK_API static bool AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);

	bool operator==(const FPragma_TestOneOf& Other) const;
	bool operator!=(const FPragma_TestOneOf& Other) const { return !operator==(Other); }
};

// Test proto3 optional (internally represented as a oneof).

USTRUCT(BlueprintType)
struct PRAGMASDK_API FPragma_TestOptional_OptionalField
{
	GENERATED_BODY()

	FPragma_TestOptional_OptionalField();
	~FPragma_TestOptional_OptionalField();

	FPragma_TestOptional_OptionalField(const FPragma_TestOptional_OptionalField& Other);
	FPragma_TestOptional_OptionalField& operator=(const FPragma_TestOptional_OptionalField& Other);

	bool Equals(const FPragma_TestOptional_OptionalField& Other) const;

	bool HasValue() const { return ValuePtr != nullptr; }
	void Reset();

	const FString& Value() const;

	void SetValue(const FString& Value);

	static bool SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
	static bool DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);

private:
	TUniquePtr<FString> ValuePtr;
};
bool operator==(const FPragma_TestOptional_OptionalField& Lhs, const FPragma_TestOptional_OptionalField& Rhs);
bool operator!=(const FPragma_TestOptional_OptionalField& Lhs, const FPragma_TestOptional_OptionalField& Rhs);

USTRUCT()
struct FPragma_TestOptional
{
	GENERATED_BODY()
	
	UPROPERTY(Transient)
	FPragma_TestOptional_OptionalField OptionalField;
	
	PRAGMASDK_API static bool AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj);
	PRAGMASDK_API static bool AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue);

	bool operator==(const FPragma_TestOptional& Other) const { return OptionalField == Other.OptionalField; }
	bool operator!=(const FPragma_TestOptional& Other) const { return !operator==(Other); }
};
