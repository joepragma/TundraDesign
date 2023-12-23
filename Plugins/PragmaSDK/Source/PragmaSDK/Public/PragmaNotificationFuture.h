#pragma once

#include "Async/Future.h"
#include "PragmaMessageMetadata.h"
#include "Runtime/Launch/Resources/Version.h"

namespace Pragma
{
template <typename NotificationType>
struct TNotificationEventPayload
{
	NotificationType Notification;
	FPragmaMessageMetadata Metadata;
};

template <typename NotificationType>
#if ENGINE_MAJOR_VERSION < 5 && ENGINE_MINOR_VERSION < 26
TFuture<TNotificationEventPayload<NotificationType>> NotificationFuture(TBaseMulticastDelegate<void, NotificationType, const FPragmaMessageMetadata&>& Delegate)
#else
TFuture<TNotificationEventPayload<NotificationType>> NotificationFuture(TMulticastDelegate<void(NotificationType, const FPragmaMessageMetadata&)>& Delegate)
#endif
{
	// Need to use a shared ptr for the promise because unique lambdas don't work with delegates.
	TSharedPtr<TPromise<TNotificationEventPayload<NotificationType>>> Promise = MakeShared<TPromise<TNotificationEventPayload<NotificationType>>>();
	auto Future = Promise->GetFuture();
	auto Handle = MakeShared<FDelegateHandle>();
	*Handle = Delegate.AddLambda([Promise, Delegate, Handle](NotificationType Notification, const FPragmaMessageMetadata& Metadata) mutable
	{
		if (Promise.IsValid())
		{
			TNotificationEventPayload<NotificationType> Result;
			Result.Notification = MoveTempIfPossible(Notification);
			Result.Metadata = Metadata;
			Promise->SetValue(MoveTemp(Result));
		}
		Delegate.Remove(*Handle);
	});
	return Future;
}
}
