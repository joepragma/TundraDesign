// <auto-generated>
//      Generated by Pragma Platform protoc-custom-plugins sdk-unreal4 target.  DO NOT EDIT!
//      Source template: unreal4/Dto.cpp.mustache
// </auto-generated>

#include "PragmaPaymentRpcDto.h"
#include "CoreMinimal.h"
#include "PragmaJson.h"



bool operator==(const FPragma_Payment_AnnotatedProductEntry& Lhs, const FPragma_Payment_AnnotatedProductEntry& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.Id == Rhs.Id
			&& Lhs.Price == Rhs.Price
			&& Lhs.CurrencyCode == Rhs.CurrencyCode
;
}
bool operator!=(const FPragma_Payment_AnnotatedProductEntry& Lhs, const FPragma_Payment_AnnotatedProductEntry& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Payment_GetProductEntriesV1Response& Lhs, const FPragma_Payment_GetProductEntriesV1Response& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.ProductEntries == Rhs.ProductEntries
;
}
bool operator!=(const FPragma_Payment_GetProductEntriesV1Response& Lhs, const FPragma_Payment_GetProductEntriesV1Response& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Payment_GetProductEntriesV1Request& Lhs, const FPragma_Payment_GetProductEntriesV1Request& Rhs)
{
	return true // Initial true allows simpler codegen.
;
}
bool operator!=(const FPragma_Payment_GetProductEntriesV1Request& Lhs, const FPragma_Payment_GetProductEntriesV1Request& Rhs)
{
	return !(Lhs == Rhs);
}


bool operator==(const FPragma_Payment_OrderRecord& Lhs, const FPragma_Payment_OrderRecord& Rhs)
{
	return true // Initial true allows simpler codegen.
			&& Lhs.PragmaSocialId == Rhs.PragmaSocialId
			&& Lhs.OrderId == Rhs.OrderId
			&& Lhs.GameShardId == Rhs.GameShardId
			&& Lhs.CreatedTimestampMillis == Rhs.CreatedTimestampMillis
			&& Lhs.LastUpdateTimestampMillis == Rhs.LastUpdateTimestampMillis
			&& Lhs.OrderStatus == Rhs.OrderStatus
			&& Lhs.ItemBundleId == Rhs.ItemBundleId
			&& Lhs.OrderPrice == Rhs.OrderPrice
			&& Lhs.CurrencyCode == Rhs.CurrencyCode
			&& Lhs.PaymentProviderId == Rhs.PaymentProviderId
;
}
bool operator!=(const FPragma_Payment_OrderRecord& Lhs, const FPragma_Payment_OrderRecord& Rhs)
{
	return !(Lhs == Rhs);
}
