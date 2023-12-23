#include "PragmaBackendTypeConverter.h"

#include "Dto/PragmaGeneratedTypes.h"
#include "PragmaJson.h"

TSharedRef<FPragmaBackendTypeConverter> FPragmaBackendTypeConverter::GetTestInstance(TArray<FPragmaBackendTypeData> InData)
{
#if UE_BUILD_SHIPPING
	PRAGMA_LOG(Error, "FPragmaBackendTypeConverter::GetTestInstance -- don't use test instance in real builds!");
	return TSharedRef<FPragmaBackendTypeConverter>();
#else
	auto* Converter = new FPragmaBackendTypeConverter(MoveTemp(InData));
	return MakeShareable(Converter);
#endif
}

#if !UE_BUILD_SHIPPING
FPragmaBackendTypeConverter::FPragmaBackendTypeConverter(TArray<FPragmaBackendTypeData> InData)
	: Data(MoveTemp(InData))
{
	for (const FPragmaBackendTypeData& BackendType : Data)
	{
		ByNativeType.Emplace(BackendType.StaticStruct, &BackendType);
		ByName.Emplace(BackendType.BackendTypeName, &BackendType);
		ById.Emplace(BackendType.BackendTypeId, &BackendType);
	}
}
#endif

FPragmaBackendTypeConverter::FPragmaBackendTypeConverter()
	: Data(PragmaGeneratedTypes::Get())
{
	ByNativeType.Reserve(Data.Num());
	ByName.Reserve(Data.Num());
	for (const FPragmaBackendTypeData& BackendType : Data)
	{
		ByNativeType.Emplace(BackendType.StaticStruct, &BackendType);
		ByName.Emplace(BackendType.BackendTypeName, &BackendType);
		// Note that IDs are filled in at runtime, so the ById map is generated in LoadBackendIds.
	}
}

bool FPragmaBackendTypeConverter::GetBackendId(const UScriptStruct* StaticStruct, int32* OutBackendId) const
{
	checkf(OutBackendId != nullptr, TEXT("FPragmaBackendTypeConverter::GetBackendId -- OutBackendId cannot be nullptr."));
	const FPragmaBackendTypeData* const* BackendTypePtr = ByNativeType.Find(StaticStruct);
	if (BackendTypePtr == nullptr)
	{
		return false;
	}
	checkf(*BackendTypePtr != nullptr, TEXT("FPragmaBackendTypeConverter::LoadBackendIds -- Invalid ptr. This will only happen if the internal consistency of the FPragmaBackendTypeConverter is at broken. "));
	*OutBackendId = (*BackendTypePtr)->BackendTypeId;
	return true;
}

bool FPragmaBackendTypeConverter::GetBackendName(const UScriptStruct* StaticStruct, FString* OutBackendName) const
{
	checkf(OutBackendName != nullptr, TEXT("FPragmaBackendTypeConverter::GetBackendName -- OutBackendName cannot be nullptr."));
	const FPragmaBackendTypeData* const* BackendTypePtr = ByNativeType.Find(StaticStruct);
	if (BackendTypePtr == nullptr)
	{
		return false;
	}
	checkf(*BackendTypePtr != nullptr, TEXT("FPragmaBackendTypeConverter::LoadBackendIds -- Invalid ptr. This will only happen if the internal consistency of the FPragmaBackendTypeConverter is at broken. "));
	*OutBackendName = (*BackendTypePtr)->BackendTypeName;
	return true;
}

void FPragmaBackendTypeConverter::LoadBackendIds(const TSharedPtr<FJsonObject> TypesJsonObj)
{
	PRAGMA_LOG(Verbose, "FPragmaBackendTypeConverter::LoadBackendIds -- Loading runtime backend type IDs...");
	ById.Empty(Data.Num());
	TSet<FString> MissingTypes;
	TSet<FString> InvalidTypes;
	TSet<FString> DuplicateTypes;
	for (const auto& Pair : TypesJsonObj.Get()->Values)
	{
		const FString& BackendName = Pair.Key;
		int32 BackendId = 0;
		if (!Pair.Value->TryGetNumber(BackendId))
		{
			InvalidTypes.Add(BackendName);
			continue;
		}
		if (ById.Find(BackendId) != nullptr)
		{
			DuplicateTypes.Add(BackendName);
			continue;
		}
		const FPragmaBackendTypeData* const* BackendTypePtr = ByName.Find(BackendName);
		if (BackendTypePtr == nullptr)
		{
			MissingTypes.Add(BackendName);
			continue;
		}
		checkf(*BackendTypePtr != nullptr, TEXT("FPragmaBackendTypeConverter::LoadBackendIds -- Invalid ptr. This will only happen if the internal consistency of the FPragmaBackendTypeConverter is at broken. "));
		ById.Emplace(BackendId, *BackendTypePtr);
	}

	if (MissingTypes.Num() > 0)
	{
		const FString Separator = "\n    ";
		PRAGMA_LOG(Warning, "FPragmaBackendTypeConverter::LoadBackendIds -- Could not find generated backend type data for some backend types. May need to regenerate types from the platform build. Missing:%s%s",
			*Separator, *FString::Join(MissingTypes, *Separator));
	}
	if (InvalidTypes.Num() > 0)
	{
		const FString Separator = "\n    ";
		PRAGMA_LOG(Warning, "FPragmaBackendTypeConverter::LoadBackendIds -- Backend ID for type names with invalid ids:%s%s",
            *Separator, *FString::Join(InvalidTypes, *Separator));
	}
	if (DuplicateTypes.Num() > 0)
	{
		const FString Separator = "\n    ";
		PRAGMA_LOG(Warning, "FPragmaBackendTypeConverter::LoadBackendIds -- Duplicate IDs for backend type name:%s%s",
            *Separator, *FString::Join(DuplicateTypes, *Separator));
	}
}
