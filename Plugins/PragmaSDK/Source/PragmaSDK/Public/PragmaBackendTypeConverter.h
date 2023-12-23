#pragma once

#include "CoreMinimal.h"

#include "PragmaBackendTypeData.h"

class FJsonObject;

class PRAGMASDK_API FPragmaBackendTypeConverter : public TSharedFromThis<FPragmaBackendTypeConverter>
{
public:
	FPragmaBackendTypeConverter();

	bool GetBackendId(const UScriptStruct* StaticStruct, int32* OutBackendId) const;
	bool GetBackendName(const UScriptStruct* StaticStruct, FString* OutBackendName) const;

	void LoadBackendIds(const TSharedPtr<FJsonObject> TypesJsonObj);
	
private:
	TArray<FPragmaBackendTypeData> Data;
	// These are map accessors to Data.
	TMap<const UScriptStruct*, const FPragmaBackendTypeData*> ByNativeType;
	TMap<int32, const FPragmaBackendTypeData*> ById;
	TMap<FString, const FPragmaBackendTypeData*> ByName;

// For unit testing only.
public:
	// This is a static fn to private constructor just to make the usage explicit.
	static TSharedRef<FPragmaBackendTypeConverter> GetTestInstance(TArray<FPragmaBackendTypeData> InData);
#if !UE_BUILD_SHIPPING
private:
	explicit FPragmaBackendTypeConverter(TArray<FPragmaBackendTypeData> InData);
#endif
	
};
