// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

// Can't be #pragma once because other modules may define PACKAGE_SCOPE

// Intended to be the last include in an exported class definition
// Properly defines some members as "public to the module" vs "private to the consumer/user"

// NOTE: This is a common pattern in Unreal modules.

#undef PACKAGE_SCOPE
#ifdef PRAGMASDK_PACKAGE
#define PACKAGE_SCOPE public
#else
#define PACKAGE_SCOPE protected
#endif