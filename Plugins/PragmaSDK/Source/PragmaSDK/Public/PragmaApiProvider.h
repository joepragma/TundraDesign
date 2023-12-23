#pragma once

#include "Containers/Map.h"
#include "Templates/SubclassOf.h"
#include "UObject/StrongObjectPtr.h"
#include "PragmaLog.h"
#include "PragmaEmptyObject.h"

namespace Pragma {

template <typename SessionType>
class TApiProvider
{
public:
	virtual ~TApiProvider() = default;

	// Get a Pragma Api object by type previously registered via RegisterApi().
	template <typename T>
	T& Api() const
	{
		auto* ApiPtr = Cast<T>(Api(T::StaticClass()));
		checkf(ApiPtr, TEXT("Session Api() -- Api '%s' is not registered. If this is a Pragma-provided Service class, ensure you are creating this"
			" session object through the Pragma::FRuntime methods. If this is your Service class, you need to register it with RegisterApi()"), *T::StaticClass()->GetName());
		return *ApiPtr;
	}

	SessionType* Api(UClass* StaticClass) const
	{
		return Apis().FindRef(StaticClass).Get();
	}

	// Register a Pragma Api object with the session.
	template <typename T>
	void RegisterApi()
	{
		// todo define static FName on class instead of using UObject StaticClass stuff.
		// todo error on multiple add
		static_assert(std::is_base_of<SessionType, T>::value, "FSession::RegisterApi -- Only classes"
			" extending from UPragmaPlayerApi or UPragmaServerApi can be registered.");
		RegisterApi(T::StaticClass());
	}

	// Register a Pragma Api object with the session.
	void RegisterApi(UClass* StaticClass)
	{
		if (!OuterWorkaround.IsValid())
		{
			OuterWorkaround = TStrongObjectPtr<UPragmaEmptyObject>{NewObject<UPragmaEmptyObject>()};
		}
		// OuterWorkaround exists because there were errors creating an object in this way without an outer. This is temporary since we will
		// soon move to non-UObject API classes anyway.
		Apis().Add(StaticClass, TStrongObjectPtr<SessionType>{NewObject<SessionType>(OuterWorkaround.Get(), StaticClass)});
		InitApi(StaticClass);
	}

protected:
	// These are just so we don't spam users with the deprecation warning.
	mutable bool CalledRegisterService{false};
	mutable bool CalledGetService{false};

	typedef TMap<TSubclassOf<SessionType>, TStrongObjectPtr<SessionType>> FSessionMap;
	virtual FSessionMap& Apis() = 0;
	virtual const FSessionMap& Apis() const = 0;
	virtual void InitApi(UClass* StaticClass) = 0;

private:
	TStrongObjectPtr<UPragmaEmptyObject> OuterWorkaround;
};

}
