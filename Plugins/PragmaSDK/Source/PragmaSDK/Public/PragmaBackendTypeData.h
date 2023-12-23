#pragma once

#include "CoreMinimal.h"

struct FPragmaBackendTypeData
{
	const UScriptStruct* StaticStruct{nullptr};
	FString BackendTypeName;
	int32 BackendTypeId{0};
};
