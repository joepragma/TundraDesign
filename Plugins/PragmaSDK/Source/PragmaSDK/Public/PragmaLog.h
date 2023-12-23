#pragma once

#include "CoreMinimal.h"
#include "Misc/CommandLine.h"

//
// Pragma SDK logging macros.
//

PRAGMASDK_API DECLARE_LOG_CATEGORY_EXTERN(LogPragma, Log, All);

// When the command line flag -PragmaTest is defined, we will add "PragmaTest:" to log lines.
// This is necessary so we can tell the automation tool to expect pragma-specific errors.
#ifndef PRAGMA_TEST_PREFIX
#define PRAGMA_TEST_PREFIX "PragmaTest: "
#endif

// Effectively compile out the command-line test in shipping builds.
#if UE_BUILD_SHIPPING
#define PRAGMA_TEST_CONDITION false
#else
#define PRAGMA_TEST_CONDITION FParse::Param(FCommandLine::Get(), TEXT("PragmaTest"))
#endif

#define PRAGMA_BASE_LOG(LoggingCategory, Verbosity, Format, ...) \
{ \
	if (PRAGMA_TEST_CONDITION) \
	{ \
		UE_LOG(LoggingCategory, Verbosity, TEXT(PRAGMA_TEST_PREFIX Format), ##__VA_ARGS__); \
	} \
	else \
	{ \
		UE_LOG(LoggingCategory, Verbosity, TEXT(Format), ##__VA_ARGS__); \
	} \
}

#define PRAGMA_LOG(Verbosity, Format, ...) { PRAGMA_BASE_LOG(LogPragma, Verbosity, Format, ##__VA_ARGS__); }

#if UE_EDITOR
#ifdef PRAGMASDK_ENABLE_DEPRECATION_WARNINGS
#define PRAGMASDK_DEPRECATED(Message) [[deprecated(Message)]]
#endif
#endif

#ifndef PRAGMASDK_DEPRECATED
#define PRAGMASDK_DEPRECATED(...)
#endif
