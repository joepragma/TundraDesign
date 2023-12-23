#pragma once

#include "CoreMinimal.h"
#include "PragmaError.h"
#include "CoreDto/PragmaServiceError.h"

//
// A TPragmaResult is a semantic wrapper for a TOptional to clarify meaning in context.
// A successful Result always has a payload.
// A failed Result never has a payload.
//
template <typename PayloadType = void>
struct TPragmaResult final : private TOptional<PayloadType>
{
public:
	// Recommend using other constructors/static methods for clarity.
	TPragmaResult() {}

	explicit TPragmaResult(const PayloadType& Payload) : TOptional<PayloadType>(Payload) {}
	explicit TPragmaResult(PayloadType&& Payload) : TOptional<PayloadType>(MoveTempIfPossible(Payload)) {}

	// ReSharper disable once CppNonExplicitConvertingConstructor
	TPragmaResult(const FPragmaError Error) : PragmaError(Error) {}

	static TPragmaResult<PayloadType> Failure(FPragmaError Error)
	{
		TPragmaResult<PayloadType> Result;
		Result.PragmaError = MoveTemp(Error);
		return Result;
	}

	static TPragmaResult<PayloadType> Failure(FPragmaServiceError Error)
	{
		TPragmaResult<PayloadType> Result;
		Result.PragmaError = MoveTemp(Error);
		return Result;
	}

	static TPragmaResult<PayloadType> Failure(FString ErrorCode)
	{
		TPragmaResult<PayloadType> Result;
		Result.ErrorCodeStr = ErrorCode;
		return Result;
	}

	static TPragmaResult<PayloadType> Failure()
	{
		return TPragmaResult<PayloadType>();
	}

	bool IsSuccessful() const { return TOptional<PayloadType>::IsSet(); }
	bool IsFailure() const { return !TOptional<PayloadType>::IsSet(); }

	const PayloadType& Payload() const
	{
		checkf(IsSuccessful(), TEXT("A TPragmaResult will never have a Payload if it failed. Check IsSuccessful() before accessing Payload()."));
		return TOptional<PayloadType>::GetValue();
	}
	PayloadType& Payload()
	{
		return const_cast<PayloadType&>(static_cast<const TPragmaResult&>(*this).Payload());
	}
	const FPragmaError& Error() const
	{
		checkf(IsFailure(), TEXT("A TPragmaResult will never have an Error if it succeeded. Check IsFailure() before accessing Error()."));
		return PragmaError;
	}
	FString ErrorCode() const
	{
		checkf(IsFailure(), TEXT("A TPragmaResult will never have an ErrorCode if it succeeded. Check IsFailure() before accessing ErrorCode()."));
		if (PragmaError.IsUnknown() && !ErrorCodeStr.IsEmpty())
		{
			return ErrorCodeStr;
		}
		return PragmaError.ErrorCode();
	}

private:
	FPragmaError PragmaError;
	FString ErrorCodeStr;
};

// Explicit specialization for void which semantically represents success without a payload, or failure with an error.
template <>
struct TPragmaResult<void>
{
public:
	// Recommend using other constructors/static methods for clarity.
	TPragmaResult(){}

	// ReSharper disable once CppNonExplicitConvertingConstructor
	TPragmaResult(const FPragmaError Error) : PragmaError(Error) {}

	static TPragmaResult<> Success()
	{
		TPragmaResult Result;
		Result.bSuccess = true;
		return Result;
	}

	static TPragmaResult Failure(FPragmaError Error)
	{
		TPragmaResult Result;
		Result.PragmaError = MoveTemp(Error);
		Result.bSuccess = false;
		return Result;
	}

	static TPragmaResult<> Failure(FString ErrorCode)
	{
		TPragmaResult<> Result;
		Result.ErrorCodeStr = ErrorCode;
		Result.bSuccess = false;
		return Result;
	}

	static TPragmaResult<> Failure(FPragmaServiceError Error)
	{
		TPragmaResult<> Result;
		Result.PragmaError = MoveTemp(Error);
		Result.bSuccess = false;
		return Result;
	}

	bool IsSuccessful() const { return bSuccess; }
	bool IsFailure() const { return !bSuccess; }

	const FPragmaError& Error() const
	{
		checkf(IsFailure(), TEXT("A TPragmaResult will never have an Error if it successful. Check IsFailure() before accessing Error()."));
		return PragmaError;
	}
	FString ErrorCode() const
	{
		checkf(IsFailure(), TEXT("A TPragmaResult will never have an ErrorCode if it successful. Check IsFailure() before accessing ErrorCode()."));
		if (PragmaError.IsUnknown())
		{
			return ErrorCodeStr;
		}
		return PragmaError.ErrorCode();
	}

private:
	bool bSuccess{false};
	FPragmaError PragmaError;
	FString ErrorCodeStr;
};
