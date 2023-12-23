#pragma once

/* This file was auto-generated, and should not be manually edited. See pragma.playerdata.codegen.generators.SubServiceUnrealGenerator */

#include "Delegates/Delegate.h"
#include "Misc/Optional.h"

#if ENGINE_MAJOR_VERSION < 5 || ENGINE_MINOR_VERSION < 2
#define NullOpt {}
#endif

struct FPragma_PlayerData_MychecklistResponseOneProto;

DECLARE_DELEGATE_OneParam(FOnMyMyOperationDelegate, TOptional<FPragma_PlayerData_MychecklistResponseOneProto>);