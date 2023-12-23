#include "TestDto.h"
#include "CoreMinimal.h"
#include "PragmaJson.h"


//
// OneOf test structs/enum below were generated via code gen.
//


bool FPragma_TestOneOfInnerStruct::operator==(const FPragma_TestOneOfInnerStruct& Other) const
{
	return InnerStr == Other.InnerStr
        && InnerInt32 == Other.InnerInt32
        && InnerInt64 == Other.InnerInt64
        && InnerUint32 == Other.InnerUint32
        && InnerUint64 == Other.InnerUint64
        && InnerFloat == Other.InnerFloat
        && InnerDouble == Other.InnerDouble
        && InnerEnum == Other.InnerEnum
        && InnerFixed128 == Other.InnerFixed128;
}

FPragma_TestOneOfStruct_Inside::FPragma_TestOneOfStruct_Inside() {}
FPragma_TestOneOfStruct_Inside::~FPragma_TestOneOfStruct_Inside()
{
	Reset();
}

FPragma_TestOneOfStruct_Inside::FPragma_TestOneOfStruct_Inside(const FPragma_TestOneOfStruct_Inside& Other)
{
	*this = Other;
}

FPragma_TestOneOfStruct_Inside& FPragma_TestOneOfStruct_Inside::operator=(const FPragma_TestOneOfStruct_Inside& Other)
{
	Reset();
	Type = Other.OneOfType();
	switch (Other.OneOfType())
	{
	case EPragma_TestOneOfStruct_InsideType::InsideStr:
		new(Union.InsideStr.GetTypedPtr()) FString(Other.InsideStr());
		break;
	case EPragma_TestOneOfStruct_InsideType::InsideInt32:
		new(Union.InsideInt32.GetTypedPtr()) int32(Other.InsideInt32());
		break;
	case EPragma_TestOneOfStruct_InsideType::InsideInt64:
		new(Union.InsideInt64.GetTypedPtr()) int64(Other.InsideInt64());
		break;
	case EPragma_TestOneOfStruct_InsideType::InsideUint32:
		new(Union.InsideUint32.GetTypedPtr()) uint32(Other.InsideUint32());
		break;
	case EPragma_TestOneOfStruct_InsideType::InsideUint64:
		new(Union.InsideUint64.GetTypedPtr()) uint64(Other.InsideUint64());
		break;
	case EPragma_TestOneOfStruct_InsideType::InsideFloat:
		new(Union.InsideFloat.GetTypedPtr()) float(Other.InsideFloat());
		break;
	case EPragma_TestOneOfStruct_InsideType::InsideDouble:
		new(Union.InsideDouble.GetTypedPtr()) double(Other.InsideDouble());
		break;
	case EPragma_TestOneOfStruct_InsideType::InsideFixed128:
		new(Union.InsideFixed128.GetTypedPtr()) FString(Other.InsideFixed128());
		break;
	case EPragma_TestOneOfStruct_InsideType::InsideEnum:
		new(Union.InsideEnum.GetTypedPtr()) EPragma_TestOneOfEnum(Other.InsideEnum());
		break;
	case EPragma_TestOneOfStruct_InsideType::InsideStruct:
		new(Union.InsideStruct.GetTypedPtr()) FPragma_TestOneOfInnerStruct(Other.InsideStruct());
		break;
	case EPragma_TestOneOfStruct_InsideType::Invalid:
	default:
		break;
	}
	return *this;
}

void FPragma_TestOneOfStruct_Inside::Reset()
{
	switch (OneOfType())
	{
	case EPragma_TestOneOfStruct_InsideType::InsideStr:
		Union.InsideStr.GetTypedPtr()->~FString();
		break;
	case EPragma_TestOneOfStruct_InsideType::InsideInt32:
		// Primitive type, no destructor.
		break;
	case EPragma_TestOneOfStruct_InsideType::InsideInt64:
		// Primitive type, no destructor.
		break;
	case EPragma_TestOneOfStruct_InsideType::InsideUint32:
		// Primitive type, no destructor.
		break;
	case EPragma_TestOneOfStruct_InsideType::InsideUint64:
		// Primitive type, no destructor.
		break;
	case EPragma_TestOneOfStruct_InsideType::InsideFloat:
		// Primitive type, no destructor.
		break;
	case EPragma_TestOneOfStruct_InsideType::InsideDouble:
		// Primitive type, no destructor.
		break;
	case EPragma_TestOneOfStruct_InsideType::InsideFixed128:
		Union.InsideFixed128.GetTypedPtr()->~FString();
		break;
	case EPragma_TestOneOfStruct_InsideType::InsideEnum:
		// Primitive type, no destructor.
		break;
	case EPragma_TestOneOfStruct_InsideType::InsideStruct:
		Union.InsideStruct.GetTypedPtr()->~FPragma_TestOneOfInnerStruct();
		break;
	case EPragma_TestOneOfStruct_InsideType::Invalid:
	default:
		break;
	}
	Type = EPragma_TestOneOfStruct_InsideType::Invalid;
}

const FString& FPragma_TestOneOfStruct_Inside::InsideStr() const {
	checkf(OneOfType() == EPragma_TestOneOfStruct_InsideType::InsideStr, TEXT("Failed to retrieve InsideStr from FPragma_TestOneOfStruct_Inside. Check the OneOfType() before accessing the value."));
	return *Union.InsideStr.GetTypedPtr();
}
const int32& FPragma_TestOneOfStruct_Inside::InsideInt32() const {
	checkf(OneOfType() == EPragma_TestOneOfStruct_InsideType::InsideInt32, TEXT("Failed to retrieve InsideInt32 from FPragma_TestOneOfStruct_Inside. Check the OneOfType() before accessing the value."));
	return *Union.InsideInt32.GetTypedPtr();
}
const int64& FPragma_TestOneOfStruct_Inside::InsideInt64() const {
	checkf(OneOfType() == EPragma_TestOneOfStruct_InsideType::InsideInt64, TEXT("Failed to retrieve InsideInt64 from FPragma_TestOneOfStruct_Inside. Check the OneOfType() before accessing the value."));
	return *Union.InsideInt64.GetTypedPtr();
}
const uint32& FPragma_TestOneOfStruct_Inside::InsideUint32() const {
	checkf(OneOfType() == EPragma_TestOneOfStruct_InsideType::InsideUint32, TEXT("Failed to retrieve InsideUint32 from FPragma_TestOneOfStruct_Inside. Check the OneOfType() before accessing the value."));
	return *Union.InsideUint32.GetTypedPtr();
}
const uint64& FPragma_TestOneOfStruct_Inside::InsideUint64() const {
	checkf(OneOfType() == EPragma_TestOneOfStruct_InsideType::InsideUint64, TEXT("Failed to retrieve InsideUint64 from FPragma_TestOneOfStruct_Inside. Check the OneOfType() before accessing the value."));
	return *Union.InsideUint64.GetTypedPtr();
}
const float& FPragma_TestOneOfStruct_Inside::InsideFloat() const {
	checkf(OneOfType() == EPragma_TestOneOfStruct_InsideType::InsideFloat, TEXT("Failed to retrieve InsideFloat from FPragma_TestOneOfStruct_Inside. Check the OneOfType() before accessing the value."));
	return *Union.InsideFloat.GetTypedPtr();
}
const double& FPragma_TestOneOfStruct_Inside::InsideDouble() const {
	checkf(OneOfType() == EPragma_TestOneOfStruct_InsideType::InsideDouble, TEXT("Failed to retrieve InsideDouble from FPragma_TestOneOfStruct_Inside. Check the OneOfType() before accessing the value."));
	return *Union.InsideDouble.GetTypedPtr();
}
const FString& FPragma_TestOneOfStruct_Inside::InsideFixed128() const {
	checkf(OneOfType() == EPragma_TestOneOfStruct_InsideType::InsideFixed128, TEXT("Failed to retrieve InsideFixed128 from FPragma_TestOneOfStruct_Inside. Check the OneOfType() before accessing the value."));
	return *Union.InsideFixed128.GetTypedPtr();
}
const EPragma_TestOneOfEnum& FPragma_TestOneOfStruct_Inside::InsideEnum() const {
	checkf(OneOfType() == EPragma_TestOneOfStruct_InsideType::InsideEnum, TEXT("Failed to retrieve InsideEnum from FPragma_TestOneOfStruct_Inside. Check the OneOfType() before accessing the value."));
	return *Union.InsideEnum.GetTypedPtr();
}
const FPragma_TestOneOfInnerStruct& FPragma_TestOneOfStruct_Inside::InsideStruct() const {
	checkf(OneOfType() == EPragma_TestOneOfStruct_InsideType::InsideStruct, TEXT("Failed to retrieve InsideStruct from FPragma_TestOneOfStruct_Inside. Check the OneOfType() before accessing the value."));
	return *Union.InsideStruct.GetTypedPtr();
}

void FPragma_TestOneOfStruct_Inside::SetInsideStr(const FString& Value)
{
	Reset();
	Type = EPragma_TestOneOfStruct_InsideType::InsideStr;
	new(Union.InsideStr.GetTypedPtr()) FString(Value);
}
void FPragma_TestOneOfStruct_Inside::SetInsideInt32(const int32& Value)
{
	Reset();
	Type = EPragma_TestOneOfStruct_InsideType::InsideInt32;
	new(Union.InsideInt32.GetTypedPtr()) int32(Value);
}
void FPragma_TestOneOfStruct_Inside::SetInsideInt64(const int64& Value)
{
	Reset();
	Type = EPragma_TestOneOfStruct_InsideType::InsideInt64;
	new(Union.InsideInt64.GetTypedPtr()) int64(Value);
}
void FPragma_TestOneOfStruct_Inside::SetInsideUint32(const uint32& Value)
{
	Reset();
	Type = EPragma_TestOneOfStruct_InsideType::InsideUint32;
	new(Union.InsideUint32.GetTypedPtr()) uint32(Value);
}
void FPragma_TestOneOfStruct_Inside::SetInsideUint64(const uint64& Value)
{
	Reset();
	Type = EPragma_TestOneOfStruct_InsideType::InsideUint64;
	new(Union.InsideUint64.GetTypedPtr()) uint64(Value);
}
void FPragma_TestOneOfStruct_Inside::SetInsideFloat(const float& Value)
{
	Reset();
	Type = EPragma_TestOneOfStruct_InsideType::InsideFloat;
	new(Union.InsideFloat.GetTypedPtr()) float(Value);
}
void FPragma_TestOneOfStruct_Inside::SetInsideDouble(const double& Value)
{
	Reset();
	Type = EPragma_TestOneOfStruct_InsideType::InsideDouble;
	new(Union.InsideDouble.GetTypedPtr()) double(Value);
}
void FPragma_TestOneOfStruct_Inside::SetInsideFixed128(const FString& Value)
{
	Reset();
	Type = EPragma_TestOneOfStruct_InsideType::InsideFixed128;
	new(Union.InsideFixed128.GetTypedPtr()) FString(Value);
}
void FPragma_TestOneOfStruct_Inside::SetInsideEnum(const EPragma_TestOneOfEnum& Value)
{
	Reset();
	Type = EPragma_TestOneOfStruct_InsideType::InsideEnum;
	new(Union.InsideEnum.GetTypedPtr()) EPragma_TestOneOfEnum(Value);
}
void FPragma_TestOneOfStruct_Inside::SetInsideStruct(const FPragma_TestOneOfInnerStruct& Value)
{
	Reset();
	Type = EPragma_TestOneOfStruct_InsideType::InsideStruct;
	new(Union.InsideStruct.GetTypedPtr()) FPragma_TestOneOfInnerStruct(Value);
}

bool FPragma_TestOneOfStruct_Inside::SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj)
{
	const FPragma_TestOneOfStruct_Inside& OneOf = static_cast<const FPragma_TestOneOfStruct*>(Value)->Inside;
	switch (OneOf.OneOfType())
	{
		case EPragma_TestOneOfStruct_InsideType::InsideStr:
			OutJsonObj->SetField("insideStr", PragmaJson::StringToJsonValue(*OneOf.Union.InsideStr.GetTypedPtr()));
			return true;
		case EPragma_TestOneOfStruct_InsideType::InsideInt32:
			OutJsonObj->SetField("insideInt32", PragmaJson::Int32ToJsonValue(*OneOf.Union.InsideInt32.GetTypedPtr()));
			return true;
		case EPragma_TestOneOfStruct_InsideType::InsideInt64:
			OutJsonObj->SetField("insideInt64", PragmaJson::Int64ToJsonValue(*OneOf.Union.InsideInt64.GetTypedPtr()));
			return true;
		case EPragma_TestOneOfStruct_InsideType::InsideUint32:
			OutJsonObj->SetField("insideUint32", PragmaJson::UInt32ToJsonValue(*OneOf.Union.InsideUint32.GetTypedPtr()));
			return true;
		case EPragma_TestOneOfStruct_InsideType::InsideUint64:
			OutJsonObj->SetField("insideUint64", PragmaJson::UInt64ToJsonValue(*OneOf.Union.InsideUint64.GetTypedPtr()));
			return true;
		case EPragma_TestOneOfStruct_InsideType::InsideFloat:
			OutJsonObj->SetField("insideFloat", PragmaJson::FloatToJsonValue(*OneOf.Union.InsideFloat.GetTypedPtr()));
			return true;
		case EPragma_TestOneOfStruct_InsideType::InsideDouble:
			OutJsonObj->SetField("insideDouble", PragmaJson::DoubleToJsonValue(*OneOf.Union.InsideDouble.GetTypedPtr()));
			return true;
		case EPragma_TestOneOfStruct_InsideType::InsideFixed128:
			OutJsonObj->SetField("insideFixed128", PragmaJson::StringToJsonValue(*OneOf.Union.InsideFixed128.GetTypedPtr()));
			return true;
		case EPragma_TestOneOfStruct_InsideType::InsideEnum:
			OutJsonObj->SetField("insideEnum", PragmaJson::EnumToJsonValue(*OneOf.Union.InsideEnum.GetTypedPtr()));
			return true;
		case EPragma_TestOneOfStruct_InsideType::InsideStruct:
			OutJsonObj->SetField("insideStruct", PragmaJson::StructToJsonValue(*OneOf.Union.InsideStruct.GetTypedPtr()));
			return true;
		default:
			return false;
	}
}
bool FPragma_TestOneOfStruct_Inside::DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue)
{
	FPragma_TestOneOfStruct_Inside& OutOneOf = static_cast<FPragma_TestOneOfStruct*>(OutValue)->Inside;
	if (JsonObj->HasField("insideStr"))
	{
		OutOneOf.Type = EPragma_TestOneOfStruct_InsideType::InsideStr;
		new(OutOneOf.Union.InsideStr.GetTypedPtr()) FString(PragmaJson::JsonValueToString(JsonObj->TryGetField(TEXT("insideStr")).ToSharedRef()));
		return true;
	}
	if (JsonObj->HasField("insideInt32"))
	{
		OutOneOf.Type = EPragma_TestOneOfStruct_InsideType::InsideInt32;
		new(OutOneOf.Union.InsideInt32.GetTypedPtr()) int32(PragmaJson::JsonValueToInt32(JsonObj->TryGetField(TEXT("insideInt32")).ToSharedRef()));
		return true;
	}
	if (JsonObj->HasField("insideInt64"))
	{
		OutOneOf.Type = EPragma_TestOneOfStruct_InsideType::InsideInt64;
		new(OutOneOf.Union.InsideInt64.GetTypedPtr()) int64(PragmaJson::JsonValueToInt64(JsonObj->TryGetField(TEXT("insideInt64")).ToSharedRef()));
		return true;
	}
	if (JsonObj->HasField("insideUint32"))
	{
		OutOneOf.Type = EPragma_TestOneOfStruct_InsideType::InsideUint32;
		new(OutOneOf.Union.InsideUint32.GetTypedPtr()) uint32(PragmaJson::JsonValueToUInt32(JsonObj->TryGetField(TEXT("insideUint32")).ToSharedRef()));
		return true;
	}
	if (JsonObj->HasField("insideUint64"))
	{
		OutOneOf.Type = EPragma_TestOneOfStruct_InsideType::InsideUint64;
		new(OutOneOf.Union.InsideUint64.GetTypedPtr()) uint64(PragmaJson::JsonValueToUInt64(JsonObj->TryGetField(TEXT("insideUint64")).ToSharedRef()));
		return true;
	}
	if (JsonObj->HasField("insideFloat"))
	{
		OutOneOf.Type = EPragma_TestOneOfStruct_InsideType::InsideFloat;
		new(OutOneOf.Union.InsideFloat.GetTypedPtr()) float(PragmaJson::JsonValueToFloat(JsonObj->TryGetField(TEXT("insideFloat")).ToSharedRef()));
		return true;
	}
	if (JsonObj->HasField("insideDouble"))
	{
		OutOneOf.Type = EPragma_TestOneOfStruct_InsideType::InsideDouble;
		new(OutOneOf.Union.InsideDouble.GetTypedPtr()) double(PragmaJson::JsonValueToDouble(JsonObj->TryGetField(TEXT("insideDouble")).ToSharedRef()));
		return true;
	}
	if (JsonObj->HasField("insideFixed128"))
	{
		OutOneOf.Type = EPragma_TestOneOfStruct_InsideType::InsideFixed128;
		new(OutOneOf.Union.InsideFixed128.GetTypedPtr()) FString(PragmaJson::JsonValueToString(JsonObj->TryGetField(TEXT("insideFixed128")).ToSharedRef()));
		return true;
	}
	if (JsonObj->HasField("insideEnum"))
	{
		OutOneOf.Type = EPragma_TestOneOfStruct_InsideType::InsideEnum;
		new(OutOneOf.Union.InsideEnum.GetTypedPtr()) EPragma_TestOneOfEnum(PragmaJson::JsonValueToEnum<EPragma_TestOneOfEnum>(JsonObj->TryGetField(TEXT("insideEnum")).ToSharedRef()));
		return true;
	}
	if (JsonObj->HasField("insideStruct"))
	{
		OutOneOf.Type = EPragma_TestOneOfStruct_InsideType::InsideStruct;
		new(OutOneOf.Union.InsideStruct.GetTypedPtr()) FPragma_TestOneOfInnerStruct(PragmaJson::JsonValueToStruct<FPragma_TestOneOfInnerStruct>(JsonObj->TryGetField(TEXT("insideStruct")).ToSharedRef()));
		return true;
	}
	return false;
}

bool FPragma_TestOneOfStruct_Inside::operator==(const FPragma_TestOneOfStruct_Inside& Other) const
{
	if (OneOfType() != Other.OneOfType())
	{
		return false;
	}
	switch (OneOfType())
	{
	case EPragma_TestOneOfStruct_InsideType::InsideStr: return *Union.InsideStr.GetTypedPtr() == Other.InsideStr();
	case EPragma_TestOneOfStruct_InsideType::InsideInt32: return *Union.InsideInt32.GetTypedPtr() == Other.InsideInt32();
	case EPragma_TestOneOfStruct_InsideType::InsideInt64: return *Union.InsideInt64.GetTypedPtr() == Other.InsideInt64();
	case EPragma_TestOneOfStruct_InsideType::InsideUint32: return *Union.InsideUint32.GetTypedPtr() == Other.InsideUint32();
	case EPragma_TestOneOfStruct_InsideType::InsideUint64: return *Union.InsideUint64.GetTypedPtr() == Other.InsideUint64();
	case EPragma_TestOneOfStruct_InsideType::InsideFloat: return *Union.InsideFloat.GetTypedPtr() == Other.InsideFloat();
	case EPragma_TestOneOfStruct_InsideType::InsideDouble: return *Union.InsideDouble.GetTypedPtr() == Other.InsideDouble();
	case EPragma_TestOneOfStruct_InsideType::InsideFixed128: return *Union.InsideFixed128.GetTypedPtr() == Other.InsideFixed128();
	case EPragma_TestOneOfStruct_InsideType::InsideEnum: return *Union.InsideEnum.GetTypedPtr() == Other.InsideEnum();
	case EPragma_TestOneOfStruct_InsideType::InsideStruct: return *Union.InsideStruct.GetTypedPtr() == Other.InsideStruct();
	case EPragma_TestOneOfStruct_InsideType::Invalid: return true;
	default:
        return false;
	}
}

bool FPragma_TestOneOfStruct::AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj)
{
	bool Success = true;
	Success &= FPragma_TestOneOfStruct_Inside::SerializeIntoParentJson(Value, OutJsonObj);
	return Success;
}

bool FPragma_TestOneOfStruct::AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue)
{
	bool Success = true;
	Success &= FPragma_TestOneOfStruct_Inside::DeserializeFromParentJson(JsonObj, OutValue);
	return Success;
}

bool FPragma_TestOneOfStruct::operator==(const FPragma_TestOneOfStruct& Other) const
{
	const bool Equal = OutsideStr == Other.OutsideStr
        && OutsideInt32 == Other.OutsideInt32
        && OutsideInt64 == Other.OutsideInt64
        && OutsideUint32 == Other.OutsideUint32
        && OutsideUint64 == Other.OutsideUint64
        && OutsideFloat == Other.OutsideFloat
        && OutsideDouble == Other.OutsideDouble
        && OutsideEnum == Other.OutsideEnum
        && OutsideFixed128 == Other.OutsideFixed128
        && OutsideStruct == Other.OutsideStruct
        && OutsideArray == Other.OutsideArray
        && Inside == Other.Inside;
	if (!Equal)
	{
		return false;
	}
	if (OutsideMap.Num() != Other.OutsideMap.Num())
	{
		return false;
	}
	for (const auto& Entry : OutsideMap)
	{
		const auto* OtherPtr = Other.OutsideMap.Find(Entry.Key);
		if (OtherPtr == nullptr || Entry.Value != *OtherPtr)
		{
			return false;
		}
	}
	return true;
}

FPragma_TestOneOf_Inside::FPragma_TestOneOf_Inside() {}
FPragma_TestOneOf_Inside::~FPragma_TestOneOf_Inside()
{
	Reset();
}

FPragma_TestOneOf_Inside::FPragma_TestOneOf_Inside(const FPragma_TestOneOf_Inside& Other)
{
	*this = Other;
}

FPragma_TestOneOf_Inside& FPragma_TestOneOf_Inside::operator=(const FPragma_TestOneOf_Inside& Other)
{
	Reset();
	Type = Other.OneOfType();
	switch (Other.OneOfType())
	{
	case EPragma_TestOneOf_InsideType::InsideStr:
		new(Union.InsideStr.GetTypedPtr()) FString(Other.InsideStr());
		break;
	case EPragma_TestOneOf_InsideType::InsideInt32:
		new(Union.InsideInt32.GetTypedPtr()) int32(Other.InsideInt32());
		break;
	case EPragma_TestOneOf_InsideType::InsideInt64:
		new(Union.InsideInt64.GetTypedPtr()) int64(Other.InsideInt64());
		break;
	case EPragma_TestOneOf_InsideType::InsideUint32:
		new(Union.InsideUint32.GetTypedPtr()) uint32(Other.InsideUint32());
		break;
	case EPragma_TestOneOf_InsideType::InsideUint64:
		new(Union.InsideUint64.GetTypedPtr()) uint64(Other.InsideUint64());
		break;
	case EPragma_TestOneOf_InsideType::InsideFloat:
		new(Union.InsideFloat.GetTypedPtr()) float(Other.InsideFloat());
		break;
	case EPragma_TestOneOf_InsideType::InsideDouble:
		new(Union.InsideDouble.GetTypedPtr()) double(Other.InsideDouble());
		break;
	case EPragma_TestOneOf_InsideType::InsideFixed128:
		new(Union.InsideFixed128.GetTypedPtr()) FString(Other.InsideFixed128());
		break;
	case EPragma_TestOneOf_InsideType::InsideEnum:
		new(Union.InsideEnum.GetTypedPtr()) EPragma_TestOneOfEnum(Other.InsideEnum());
		break;
	case EPragma_TestOneOf_InsideType::InsideStruct:
		new(Union.InsideStruct.GetTypedPtr()) FPragma_TestOneOfStruct(Other.InsideStruct());
		break;
	case EPragma_TestOneOf_InsideType::Invalid:
	default:
		break;
	}
	return *this;
}

void FPragma_TestOneOf_Inside::Reset()
{
	switch (OneOfType())
	{
	case EPragma_TestOneOf_InsideType::InsideStr:
		Union.InsideStr.GetTypedPtr()->~FString();
		break;
	case EPragma_TestOneOf_InsideType::InsideInt32:
		// Primitive type, no destructor.
		break;
	case EPragma_TestOneOf_InsideType::InsideInt64:
		// Primitive type, no destructor.
		break;
	case EPragma_TestOneOf_InsideType::InsideUint32:
		// Primitive type, no destructor.
		break;
	case EPragma_TestOneOf_InsideType::InsideUint64:
		// Primitive type, no destructor.
		break;
	case EPragma_TestOneOf_InsideType::InsideFloat:
		// Primitive type, no destructor.
		break;
	case EPragma_TestOneOf_InsideType::InsideDouble:
		// Primitive type, no destructor.
		break;
	case EPragma_TestOneOf_InsideType::InsideFixed128:
		Union.InsideFixed128.GetTypedPtr()->~FString();
		break;
	case EPragma_TestOneOf_InsideType::InsideEnum:
		// Primitive type, no destructor.
		break;
	case EPragma_TestOneOf_InsideType::InsideStruct:
		Union.InsideStruct.GetTypedPtr()->~FPragma_TestOneOfStruct();
		break;
	case EPragma_TestOneOf_InsideType::Invalid:
	default:
		break;
	}
	Type = EPragma_TestOneOf_InsideType::Invalid;
}

const FString& FPragma_TestOneOf_Inside::InsideStr() const {
	checkf(OneOfType() == EPragma_TestOneOf_InsideType::InsideStr, TEXT("Failed to retrieve InsideStr from FPragma_TestOneOfV1Response_Inside. Check the OneOfType() before accessing the value."));
	return *Union.InsideStr.GetTypedPtr();
}
const int32& FPragma_TestOneOf_Inside::InsideInt32() const {
	checkf(OneOfType() == EPragma_TestOneOf_InsideType::InsideInt32, TEXT("Failed to retrieve InsideInt32 from FPragma_TestOneOfV1Response_Inside. Check the OneOfType() before accessing the value."));
	return *Union.InsideInt32.GetTypedPtr();
}
const int64& FPragma_TestOneOf_Inside::InsideInt64() const {
	checkf(OneOfType() == EPragma_TestOneOf_InsideType::InsideInt64, TEXT("Failed to retrieve InsideInt64 from FPragma_TestOneOfV1Response_Inside. Check the OneOfType() before accessing the value."));
	return *Union.InsideInt64.GetTypedPtr();
}
const uint32& FPragma_TestOneOf_Inside::InsideUint32() const {
	checkf(OneOfType() == EPragma_TestOneOf_InsideType::InsideUint32, TEXT("Failed to retrieve InsideUint32 from FPragma_TestOneOfV1Response_Inside. Check the OneOfType() before accessing the value."));
	return *Union.InsideUint32.GetTypedPtr();
}
const uint64& FPragma_TestOneOf_Inside::InsideUint64() const {
	checkf(OneOfType() == EPragma_TestOneOf_InsideType::InsideUint64, TEXT("Failed to retrieve InsideUint64 from FPragma_TestOneOfV1Response_Inside. Check the OneOfType() before accessing the value."));
	return *Union.InsideUint64.GetTypedPtr();
}
const float& FPragma_TestOneOf_Inside::InsideFloat() const {
	checkf(OneOfType() == EPragma_TestOneOf_InsideType::InsideFloat, TEXT("Failed to retrieve InsideFloat from FPragma_TestOneOfV1Response_Inside. Check the OneOfType() before accessing the value."));
	return *Union.InsideFloat.GetTypedPtr();
}
const double& FPragma_TestOneOf_Inside::InsideDouble() const {
	checkf(OneOfType() == EPragma_TestOneOf_InsideType::InsideDouble, TEXT("Failed to retrieve InsideDouble from FPragma_TestOneOfV1Response_Inside. Check the OneOfType() before accessing the value."));
	return *Union.InsideDouble.GetTypedPtr();
}
const FString& FPragma_TestOneOf_Inside::InsideFixed128() const {
	checkf(OneOfType() == EPragma_TestOneOf_InsideType::InsideFixed128, TEXT("Failed to retrieve InsideFixed128 from FPragma_TestOneOfV1Response_Inside. Check the OneOfType() before accessing the value."));
	return *Union.InsideFixed128.GetTypedPtr();
}
const EPragma_TestOneOfEnum& FPragma_TestOneOf_Inside::InsideEnum() const {
	checkf(OneOfType() == EPragma_TestOneOf_InsideType::InsideEnum, TEXT("Failed to retrieve InsideEnum from FPragma_TestOneOfV1Response_Inside. Check the OneOfType() before accessing the value."));
	return *Union.InsideEnum.GetTypedPtr();
}
const FPragma_TestOneOfStruct& FPragma_TestOneOf_Inside::InsideStruct() const {
	checkf(OneOfType() == EPragma_TestOneOf_InsideType::InsideStruct, TEXT("Failed to retrieve InsideStruct from FPragma_TestOneOfV1Response_Inside. Check the OneOfType() before accessing the value."));
	return *Union.InsideStruct.GetTypedPtr();
}

void FPragma_TestOneOf_Inside::SetInsideStr(const FString& Value)
{
	Reset();
	Type = EPragma_TestOneOf_InsideType::InsideStr;
	new(Union.InsideStr.GetTypedPtr()) FString(Value);
}
void FPragma_TestOneOf_Inside::SetInsideInt32(const int32& Value)
{
	Reset();
	Type = EPragma_TestOneOf_InsideType::InsideInt32;
	new(Union.InsideInt32.GetTypedPtr()) int32(Value);
}
void FPragma_TestOneOf_Inside::SetInsideInt64(const int64& Value)
{
	Reset();
	Type = EPragma_TestOneOf_InsideType::InsideInt64;
	new(Union.InsideInt64.GetTypedPtr()) int64(Value);
}
void FPragma_TestOneOf_Inside::SetInsideUint32(const uint32& Value)
{
	Reset();
	Type = EPragma_TestOneOf_InsideType::InsideUint32;
	new(Union.InsideUint32.GetTypedPtr()) uint32(Value);
}
void FPragma_TestOneOf_Inside::SetInsideUint64(const uint64& Value)
{
	Reset();
	Type = EPragma_TestOneOf_InsideType::InsideUint64;
	new(Union.InsideUint64.GetTypedPtr()) uint64(Value);
}
void FPragma_TestOneOf_Inside::SetInsideFloat(const float& Value)
{
	Reset();
	Type = EPragma_TestOneOf_InsideType::InsideFloat;
	new(Union.InsideFloat.GetTypedPtr()) float(Value);
}
void FPragma_TestOneOf_Inside::SetInsideDouble(const double& Value)
{
	Reset();
	Type = EPragma_TestOneOf_InsideType::InsideDouble;
	new(Union.InsideDouble.GetTypedPtr()) double(Value);
}
void FPragma_TestOneOf_Inside::SetInsideFixed128(const FString& Value)
{
	Reset();
	Type = EPragma_TestOneOf_InsideType::InsideFixed128;
	new(Union.InsideFixed128.GetTypedPtr()) FString(Value);
}
void FPragma_TestOneOf_Inside::SetInsideEnum(const EPragma_TestOneOfEnum& Value)
{
	Reset();
	Type = EPragma_TestOneOf_InsideType::InsideEnum;
	new(Union.InsideEnum.GetTypedPtr()) EPragma_TestOneOfEnum(Value);
}
void FPragma_TestOneOf_Inside::SetInsideStruct(const FPragma_TestOneOfStruct& Value)
{
	Reset();
	Type = EPragma_TestOneOf_InsideType::InsideStruct;
	new(Union.InsideStruct.GetTypedPtr()) FPragma_TestOneOfStruct(Value);
}

bool FPragma_TestOneOf_Inside::SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj)
{
	const FPragma_TestOneOf_Inside& OneOf = static_cast<const FPragma_TestOneOf*>(Value)->Inside;
	switch (OneOf.OneOfType())
	{
		case EPragma_TestOneOf_InsideType::InsideStr:
			OutJsonObj->SetField("insideStr", PragmaJson::StringToJsonValue(*OneOf.Union.InsideStr.GetTypedPtr()));
			return true;
		case EPragma_TestOneOf_InsideType::InsideInt32:
			OutJsonObj->SetField("insideInt32", PragmaJson::Int32ToJsonValue(*OneOf.Union.InsideInt32.GetTypedPtr()));
			return true;
		case EPragma_TestOneOf_InsideType::InsideInt64:
			OutJsonObj->SetField("insideInt64", PragmaJson::Int64ToJsonValue(*OneOf.Union.InsideInt64.GetTypedPtr()));
			return true;
		case EPragma_TestOneOf_InsideType::InsideUint32:
			OutJsonObj->SetField("insideUint32", PragmaJson::UInt32ToJsonValue(*OneOf.Union.InsideUint32.GetTypedPtr()));
			return true;
		case EPragma_TestOneOf_InsideType::InsideUint64:
			OutJsonObj->SetField("insideUint64", PragmaJson::UInt64ToJsonValue(*OneOf.Union.InsideUint64.GetTypedPtr()));
			return true;
		case EPragma_TestOneOf_InsideType::InsideFloat:
			OutJsonObj->SetField("insideFloat", PragmaJson::FloatToJsonValue(*OneOf.Union.InsideFloat.GetTypedPtr()));
			return true;
		case EPragma_TestOneOf_InsideType::InsideDouble:
			OutJsonObj->SetField("insideDouble", PragmaJson::DoubleToJsonValue(*OneOf.Union.InsideDouble.GetTypedPtr()));
			return true;
		case EPragma_TestOneOf_InsideType::InsideFixed128:
			OutJsonObj->SetField("insideFixed128", PragmaJson::StringToJsonValue(*OneOf.Union.InsideFixed128.GetTypedPtr()));
			return true;
		case EPragma_TestOneOf_InsideType::InsideEnum:
			OutJsonObj->SetField("insideEnum", PragmaJson::EnumToJsonValue(*OneOf.Union.InsideEnum.GetTypedPtr()));
			return true;
		case EPragma_TestOneOf_InsideType::InsideStruct:
			OutJsonObj->SetField("insideStruct", PragmaJson::StructToJsonValue(*OneOf.Union.InsideStruct.GetTypedPtr()));
			return true;
		default:
			return false;
	}
}
bool FPragma_TestOneOf_Inside::DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue)
{
	FPragma_TestOneOf_Inside& OutOneOf = static_cast<FPragma_TestOneOf*>(OutValue)->Inside;
	if (JsonObj->HasField("insideStr"))
	{
		OutOneOf.Type = EPragma_TestOneOf_InsideType::InsideStr;
		new(OutOneOf.Union.InsideStr.GetTypedPtr()) FString(PragmaJson::JsonValueToString(JsonObj->TryGetField(TEXT("insideStr")).ToSharedRef()));
		return true;
	}
	if (JsonObj->HasField("insideInt32"))
	{
		OutOneOf.Type = EPragma_TestOneOf_InsideType::InsideInt32;
		new(OutOneOf.Union.InsideInt32.GetTypedPtr()) int32(PragmaJson::JsonValueToInt32(JsonObj->TryGetField(TEXT("insideInt32")).ToSharedRef()));
		return true;
	}
	if (JsonObj->HasField("insideInt64"))
	{
		OutOneOf.Type = EPragma_TestOneOf_InsideType::InsideInt64;
		new(OutOneOf.Union.InsideInt64.GetTypedPtr()) int64(PragmaJson::JsonValueToInt64(JsonObj->TryGetField(TEXT("insideInt64")).ToSharedRef()));
		return true;
	}
	if (JsonObj->HasField("insideUint32"))
	{
		OutOneOf.Type = EPragma_TestOneOf_InsideType::InsideUint32;
		new(OutOneOf.Union.InsideUint32.GetTypedPtr()) uint32(PragmaJson::JsonValueToUInt32(JsonObj->TryGetField(TEXT("insideUint32")).ToSharedRef()));
		return true;
	}
	if (JsonObj->HasField("insideUint64"))
	{
		OutOneOf.Type = EPragma_TestOneOf_InsideType::InsideUint64;
		new(OutOneOf.Union.InsideUint64.GetTypedPtr()) uint64(PragmaJson::JsonValueToUInt64(JsonObj->TryGetField(TEXT("insideUint64")).ToSharedRef()));
		return true;
	}
	if (JsonObj->HasField("insideFloat"))
	{
		OutOneOf.Type = EPragma_TestOneOf_InsideType::InsideFloat;
		new(OutOneOf.Union.InsideFloat.GetTypedPtr()) float(PragmaJson::JsonValueToFloat(JsonObj->TryGetField(TEXT("insideFloat")).ToSharedRef()));
		return true;
	}
	if (JsonObj->HasField("insideDouble"))
	{
		OutOneOf.Type = EPragma_TestOneOf_InsideType::InsideDouble;
		new(OutOneOf.Union.InsideDouble.GetTypedPtr()) double(PragmaJson::JsonValueToDouble(JsonObj->TryGetField(TEXT("insideDouble")).ToSharedRef()));
		return true;
	}
	if (JsonObj->HasField("insideFixed128"))
	{
		OutOneOf.Type = EPragma_TestOneOf_InsideType::InsideFixed128;
		new(OutOneOf.Union.InsideFixed128.GetTypedPtr()) FString(PragmaJson::JsonValueToString(JsonObj->TryGetField(TEXT("insideFixed128")).ToSharedRef()));
		return true;
	}
	if (JsonObj->HasField("insideEnum"))
	{
		OutOneOf.Type = EPragma_TestOneOf_InsideType::InsideEnum;
		new(OutOneOf.Union.InsideEnum.GetTypedPtr()) EPragma_TestOneOfEnum(PragmaJson::JsonValueToEnum<EPragma_TestOneOfEnum>(JsonObj->TryGetField(TEXT("insideEnum")).ToSharedRef()));
		return true;
	}
	if (JsonObj->HasField("insideStruct"))
	{
		OutOneOf.Type = EPragma_TestOneOf_InsideType::InsideStruct;
		new(OutOneOf.Union.InsideStruct.GetTypedPtr()) FPragma_TestOneOfStruct(PragmaJson::JsonValueToStruct<FPragma_TestOneOfStruct>(JsonObj->TryGetField(TEXT("insideStruct")).ToSharedRef()));
		return true;
	}
	return false;
}

bool FPragma_TestOneOf_Inside::operator==(const FPragma_TestOneOf_Inside& Other) const
{
	if (OneOfType() != Other.OneOfType())
	{
		return false;
	}
	switch (OneOfType())
	{
	case EPragma_TestOneOf_InsideType::InsideStr: return *Union.InsideStr.GetTypedPtr() == Other.InsideStr();
	case EPragma_TestOneOf_InsideType::InsideInt32: return *Union.InsideInt32.GetTypedPtr() == Other.InsideInt32();
	case EPragma_TestOneOf_InsideType::InsideInt64: return *Union.InsideInt64.GetTypedPtr() == Other.InsideInt64();
	case EPragma_TestOneOf_InsideType::InsideUint32: return *Union.InsideUint32.GetTypedPtr() == Other.InsideUint32();
	case EPragma_TestOneOf_InsideType::InsideUint64: return *Union.InsideUint64.GetTypedPtr() == Other.InsideUint64();
	case EPragma_TestOneOf_InsideType::InsideFloat: return *Union.InsideFloat.GetTypedPtr() == Other.InsideFloat();
	case EPragma_TestOneOf_InsideType::InsideDouble: return *Union.InsideDouble.GetTypedPtr() == Other.InsideDouble();
	case EPragma_TestOneOf_InsideType::InsideFixed128: return *Union.InsideFixed128.GetTypedPtr() == Other.InsideFixed128();
	case EPragma_TestOneOf_InsideType::InsideEnum: return *Union.InsideEnum.GetTypedPtr() == Other.InsideEnum();
	case EPragma_TestOneOf_InsideType::InsideStruct: return *Union.InsideStruct.GetTypedPtr() == Other.InsideStruct();
	case EPragma_TestOneOf_InsideType::Invalid: return true;
	default: return false;
	}
}

bool FPragma_TestOneOf::AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj)
{
	bool Success = true;
	Success &= FPragma_TestOneOf_Inside::SerializeIntoParentJson(Value, OutJsonObj);
	return Success;
}

bool FPragma_TestOneOf::AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue)
{
	bool Success = true;
	Success &= FPragma_TestOneOf_Inside::DeserializeFromParentJson(JsonObj, OutValue);
	return Success;
}

bool FPragma_TestOneOf::operator==(const FPragma_TestOneOf& Other) const
{
	const bool Equal = OutsideStr == Other.OutsideStr
    && OutsideInt32 == Other.OutsideInt32
    && OutsideInt64 == Other.OutsideInt64
    && OutsideUint32 == Other.OutsideUint32
    && OutsideUint64 == Other.OutsideUint64
    && OutsideFloat == Other.OutsideFloat
    && OutsideDouble == Other.OutsideDouble
    && OutsideEnum == Other.OutsideEnum
    && OutsideFixed128 == Other.OutsideFixed128
    && OutsideStruct == Other.OutsideStruct
    && OutsideArray == Other.OutsideArray
    && Inside == Other.Inside;
	if (!Equal)
	{
		return false;
	}
	if (OutsideMap.Num() != Other.OutsideMap.Num())
	{
		return false;
	}
	for (const auto& Entry : OutsideMap)
	{
		const auto* OtherPtr = Other.OutsideMap.Find(Entry.Key);
		if (OtherPtr == nullptr || Entry.Value != *OtherPtr)
		{
			return false;
		}
	}
	return true;
}


FPragma_TestOptional_OptionalField::FPragma_TestOptional_OptionalField() {}
FPragma_TestOptional_OptionalField::~FPragma_TestOptional_OptionalField()
{
	Reset();
}

FPragma_TestOptional_OptionalField::FPragma_TestOptional_OptionalField(const FPragma_TestOptional_OptionalField& Other)
{
	*this = Other;
}

bool FPragma_TestOptional_OptionalField::Equals(const FPragma_TestOptional_OptionalField& Other) const
{
	if (!HasValue() && !Other.HasValue())
	{
		return true;
	}
	if (HasValue() != Other.HasValue())
	{
		return false;
	}
	return *ValuePtr == *Other.ValuePtr;
}

FPragma_TestOptional_OptionalField& FPragma_TestOptional_OptionalField::operator=(const FPragma_TestOptional_OptionalField& Other)
{
	if (Other.HasValue())
	{
		SetValue(Other.Value());
	}
	else
	{
		Reset();
	}
	return *this;
}

void FPragma_TestOptional_OptionalField::Reset()
{
	ValuePtr.Reset();
}

const FString& FPragma_TestOptional_OptionalField::Value() const {
	checkf(HasValue(), TEXT("Failed to retrieve OptionalField from FPragma_TestOptional_OptionalField. Check HasValue() first."));
	return *ValuePtr;
}

void FPragma_TestOptional_OptionalField::SetValue(const FString& Value)
{
	ValuePtr.Reset(new FString{Value});
}

bool FPragma_TestOptional_OptionalField::SerializeIntoParentJson(const void* Value, TSharedRef<FJsonObject>& OutJsonObj)
{
	const FPragma_TestOptional_OptionalField& Optional = static_cast<const FPragma_TestOptional*>(Value)->OptionalField;
	if (Optional.HasValue()) {
		OutJsonObj->SetField("optionalField", PragmaJson::StringToJsonValue(Optional.Value()));
	}
	return true;
}
bool FPragma_TestOptional_OptionalField::DeserializeFromParentJson(const TSharedRef<FJsonObject>& JsonObj, void* OutValue)
{
	FPragma_TestOptional_OptionalField& OutOneOf = static_cast<FPragma_TestOptional*>(OutValue)->OptionalField;
	if (JsonObj->HasField("optionalField"))
	{
		OutOneOf.ValuePtr.Reset(new FString{PragmaJson::JsonValueToString(JsonObj->TryGetField(TEXT("optionalField")).ToSharedRef())});
	}
	return true;
}

bool operator==(const FPragma_TestOptional_OptionalField& Lhs, const FPragma_TestOptional_OptionalField& Rhs)
{
	return Lhs.Equals(Rhs);
}
bool operator!=(const FPragma_TestOptional_OptionalField& Lhs, const FPragma_TestOptional_OptionalField& Rhs)
{
	return !(Lhs == Rhs);
}


bool FPragma_TestOptional::AddonSerializer(const void* Value, TSharedRef<FJsonObject>& OutJsonObj)
{
	bool Success = true;
	Success &= FPragma_TestOptional_OptionalField::SerializeIntoParentJson(Value, OutJsonObj);
	return Success;
}

bool FPragma_TestOptional::AddonDeserializer(const TSharedRef<FJsonObject>& JsonObj, void* OutValue)
{
	bool Success = true;
	Success &= FPragma_TestOptional_OptionalField::DeserializeFromParentJson(JsonObj, OutValue);
	return Success;
}

