#pragma once

// Register a notification handler with the Protocol to call an existing event.
#ifndef REGISTER_PRAGMA_SERVICE_EVENT
#define REGISTER_PRAGMA_SERVICE_EVENT(EventName, PayloadType, BackendType) \
Session()->BackendType##Backend().AddNotificationHandler<PayloadType>([this](PayloadType Payload, const FPragmaMessageMetadata& Metadata) \
{ \
	EventName.Broadcast(Payload, Metadata); \
});
#define REGISTER_PRAGMA_GAME_EVENT(EventName, PayloadType) REGISTER_PRAGMA_SERVICE_EVENT(EventName, PayloadType, Game)
#define REGISTER_PRAGMA_SOCIAL_EVENT(EventName, PayloadType) REGISTER_PRAGMA_SERVICE_EVENT(EventName, PayloadType, Social)
#endif
