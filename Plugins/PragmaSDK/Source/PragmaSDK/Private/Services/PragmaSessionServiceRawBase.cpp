#include "Services/PragmaSessionServiceRawBase.h"
#include "PragmaServiceImplMacros.h"

// Wrap service method macro to add common params automatically.
#define IMPL_PRAGMA_SESSION_SERVICE_SOCIAL_METHOD(FunctionName, RequestType, ResponseType, DelegateType) \
IMPL_PRAGMA_SOCIAL_METHOD(FSessionServiceRawBase, FunctionName, RequestType, ResponseType, DelegateType)
#define IMPL_PRAGMA_SESSION_SERVICE_GAME_METHOD(FunctionName, RequestType, ResponseType, DelegateType) \
IMPL_PRAGMA_GAME_METHOD(FSessionServiceRawBase, FunctionName, RequestType, ResponseType, DelegateType)

namespace Pragma
{
////////////////////////////////////////////////////////////////
// Notifications.
////////////////////////////////////////////////////////////////


void FSessionServiceRawBase::SetDependencies(const FSessionPtr InSession)
{
	SessionPtr = InSession;
}

void FSessionServiceRawBase::Initialize()
{
	REGISTER_PRAGMA_SOCIAL_EVENT(OnSocialSessionChangedV1Notification, FPragma_Session_SessionChangedV1Notification);
	REGISTER_PRAGMA_SOCIAL_EVENT(OnSocialSessionTerminatedV1Notification,
		FPragma_Session_SessionTerminatedV1Notification);
	REGISTER_PRAGMA_GAME_EVENT(OnGameSessionChangedV1Notification, FPragma_Session_SessionChangedV1Notification);
	REGISTER_PRAGMA_GAME_EVENT(OnGameSessionTerminatedV1Notification, FPragma_Session_SessionTerminatedV1Notification);
}

////////////////////////////////////////////////////////////////
// RPC Calls.
////////////////////////////////////////////////////////////////

IMPL_PRAGMA_SESSION_SERVICE_SOCIAL_METHOD(RequestSocialLogoutV1, FPragma_Session_RequestLogoutV1Request,
	FPragma_Session_RequestLogoutV1Response, FRequestLogoutV1Delegate);

IMPL_PRAGMA_SESSION_SERVICE_GAME_METHOD(RequestGameLogoutV1, FPragma_Session_RequestLogoutV1Request,
	FPragma_Session_RequestLogoutV1Response, FRequestLogoutV1Delegate);

IMPL_PRAGMA_SESSION_SERVICE_SOCIAL_METHOD(SocialHeartbeatV1, FPragma_Session_HeartbeatV1Request,
	FPragma_Session_HeartbeatV1Response, FHeartbeatV1Delegate);

IMPL_PRAGMA_SESSION_SERVICE_GAME_METHOD(GameHeartbeatV1, FPragma_Session_HeartbeatV1Request,
	FPragma_Session_HeartbeatV1Response, FHeartbeatV1Delegate);

IMPL_PRAGMA_SESSION_SERVICE_SOCIAL_METHOD(SocialHeartbeatPartnerV1, FPragma_Session_HeartbeatPartnerV1Request,
	FPragma_Session_HeartbeatPartnerV1Response, FHeartbeatPartnerV1Delegate);

IMPL_PRAGMA_SESSION_SERVICE_GAME_METHOD(GameHeartbeatPartnerV1, FPragma_Session_HeartbeatPartnerV1Request,
	FPragma_Session_HeartbeatPartnerV1Response, FHeartbeatPartnerV1Delegate);

IMPL_PRAGMA_SESSION_SERVICE_SOCIAL_METHOD(SocialSessionRefreshV1, FPragma_Session_SessionRefreshV1Request,
	FPragma_Session_SessionRefreshV1Response, FSessionRefreshV1Delegate);

IMPL_PRAGMA_SESSION_SERVICE_GAME_METHOD(GameSessionRefreshV1, FPragma_Session_SessionRefreshV1Request,
	FPragma_Session_SessionRefreshV1Response, FSessionRefreshV1Delegate);

}
