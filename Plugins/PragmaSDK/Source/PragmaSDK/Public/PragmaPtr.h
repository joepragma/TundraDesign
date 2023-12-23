#pragma once

#include "PragmaSdkPackage.h"
#include "Templates/SharedPointer.h"

namespace Pragma
{

constexpr ESPMode SpMode = ESPMode::NotThreadSafe;

template <typename T>
using TPragmaSharedFromThis = TSharedFromThis<T, SpMode>;

// These exist to mirror TPtr (below) in the Pragma API. Additionally it lets us change them behind the scenes as necessary.

template <typename T>
using TStrongPtr = TSharedPtr<T, SpMode>;

template <typename T>
using TStrongRef = TSharedRef<T, SpMode>;

/*
 * This ptr class is equivalent to a TWeakPtr that cannot be pinned.
 * This is a ptr to a Pragma object that may be destroyed by internal operations of the SDK, and
 * as such strong ptrs should not be saved outside of the SDK module.
 *
 * These ptrs are NOT thread-safe. The Pragma SDK should only ever be accessed via a single thread.
 * Abiding by this assumption also means that it is safe to dereference after checking IsValid, even without a Pin.
 *
 * The alternative would be to allow user code to save strong ptrs, but guard check inside every method for
 * object validity. That is not only more error-prone, but would result in an API littered with optionals
 * that would still force the user to handle the validity.
 *
 * Instead we've chosen this method, which puts the onus on the user code to ensure they have a valid ptr.
 * See individual types for guidance on when they may be destroyed.
 */
template <typename T>
class TPtr final
{
public:
	TPtr() {}
	
	// Forwarding constructor from anything that can become a TWeakPtr.
	template<typename FromType>
	FORCEINLINE TPtr(FromType Other)
		: Data(Forward<FromType>(Other))
	{
		ResetActual();
	}

	// Forwarding assignment operator from anything that can become a TWeakPtr.
	template<typename FromType>
	FORCEINLINE void operator=(FromType Other)
	{
		Data = Forward<FromType>(Other);
		ResetActual();
	}

	// Enable implicit upcast between TPtr types.
	template<
		typename OtherType,
		typename = decltype(ImplicitConv<T*>((OtherType*)nullptr))
	>
	FORCEINLINE TPtr(const TPtr<OtherType>& InPtr)
		: Data{InPtr.Data}
	{
		ResetActual();
	}
	
	[[nodiscard]] FORCEINLINE bool IsValid() const { return Data.IsValid(); }

	[[nodiscard]] FORCEINLINE T& operator*() { checkSlow(IsValid()); return *ActualPtr; }
	[[nodiscard]] FORCEINLINE T& operator*() const { checkSlow(IsValid()); return *ActualPtr; }

	[[nodiscard]] FORCEINLINE T* operator->() { checkSlow(IsValid()); return ActualPtr; }
	[[nodiscard]] FORCEINLINE T* operator->() const { checkSlow(IsValid()); return ActualPtr; }
	
	[[nodiscard]] FORCEINLINE explicit operator bool() const { return IsValid(); }
	
PACKAGE_SCOPE:
	[[nodiscard]] FORCEINLINE TStrongPtr<T> Pin() const { return Data.Pin(); }

private:
	void ResetActual()
	{
		if (auto Pinned = Data.Pin())
		{
			ActualPtr = Pinned.Get();
		}
		else
		{
			ActualPtr = nullptr;
		}
	}

	[[nodiscard]] friend FORCEINLINE bool operator==(const TPtr<T>& Lhs, const TPtr<T>& Rhs)
	{
		if (!Lhs.IsValid() && !Rhs.IsValid())
		{
			return true;
		}
		return Lhs.IsValid() && Rhs.IsValid() && Lhs.ActualPtr == Rhs.ActualPtr;
	}
	[[nodiscard]] friend FORCEINLINE bool operator!=(const TPtr<T>& Lhs, const TPtr<T>& Rhs)
	{
		return !(Lhs == Rhs);
	}

	[[nodiscard]] friend FORCEINLINE bool operator==(const TPtr<T>& InPtr, TYPE_OF_NULLPTR)
	{
		return !InPtr.IsValid();
	}
	[[nodiscard]] friend FORCEINLINE bool operator!=(const TPtr<T>& InPtr, TYPE_OF_NULLPTR)
	{
		return InPtr.IsValid();
	}

	// Track using a weak ptr.
	TWeakPtr<T, SpMode> Data{nullptr};
	// Direct access to the ptr to avoid pinning.
	T* ActualPtr{nullptr};

	
	// We are friends of ourselves so e.g. constructors can convert properly.
	template<class OtherType> friend class TPtr;
};

}

#define PRAGMA_PTR_API(Type) \
	namespace Pragma { \
		class Type; \
		using Type##Ptr = TPtr<Type>; \
		using Type##StrongPtr = TStrongPtr<Type>; \
		using Type##StrongRef = TStrongRef<Type>; \
	}

#define PRAGMA_FWD(Type) PRAGMA_PTR_API(Type)
