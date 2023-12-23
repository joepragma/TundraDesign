#pragma once
#include "PragmaProtocol.h"
#include "PragmaPlayer.h"
#include "PragmaServer.h"

struct FPragmaMessageMetadata;

//
// Helper macros for implementing service methods.
// These macros define ways to call into the Connection protocol's SendMessage wrapped in either a TUniqueFunction
// or a delegate.
//
// We intentionally do _not_ use macros for header files, so it's very clear what they are when referencing them.
//
// In service cpp files there is lots of duplication since everything just calls the socket SendMessage with different
// parameters. In this case it's clearer to just use a macro.
//

// Implement a method with a payload defined in the header file with both TUniqueFunction and Delegate options.
#define IMPL_PRAGMA_SERVICE_METHOD(FunctionOwnerType, FunctionName, RequestType, ResponseType, DelegateType, BackendType) \
void FunctionOwnerType::FunctionName(const RequestType& Payload, TUniqueFunction<void(TPragmaResult<ResponseType>, const FPragmaMessageMetadata&)> Callback) const \
{ \
	Session()->BackendType##Backend().SendMessage(Payload, MoveTemp(Callback)); \
} \
void FunctionOwnerType::FunctionName(const RequestType& Payload, DelegateType Delegate) const \
{ \
	Session()->BackendType##Backend().SendMessage<RequestType, ResponseType>(Payload, \
		[Delegate = MoveTemp(Delegate)](TPragmaResult<ResponseType> Result, const FPragmaMessageMetadata& Metadata) \
	{ \
		Delegate.ExecuteIfBound(MoveTemp(Result), Metadata); \
	}); \
}

// Game/Social.
#define IMPL_PRAGMA_GAME_METHOD(FunctionOwnerType, FunctionName, RequestType, ResponseType, DelegateType) \
	IMPL_PRAGMA_SERVICE_METHOD(FunctionOwnerType, FunctionName, RequestType, ResponseType, DelegateType, Game)
#define IMPL_PRAGMA_SOCIAL_METHOD(FunctionOwnerType, FunctionName, RequestType, ResponseType, DelegateType) \
	IMPL_PRAGMA_SERVICE_METHOD(FunctionOwnerType, FunctionName, RequestType, ResponseType, DelegateType, Social)
