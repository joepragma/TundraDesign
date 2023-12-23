#pragma once
#include "Dto/PragmaPlayerSessionRpcDto.h"
#include "Dto/PragmaPartnerSessionRpcDto.h"
#include "PragmaResult.h"
#include "PragmaMessageMetadata.h"
#include "PragmaPtr.h"

struct FPragmaMessageMetadata;

PRAGMA_FWD(FSession);

namespace Pragma {

// Proof of concept of supporting futures.
template <typename PayloadType>
struct TPragmaResultWithMetadata
{
	TPragmaResult<PayloadType> Result;
	FPragmaMessageMetadata Metadata;
};

class PRAGMASDK_API FSessionServiceRawBase
{
public:
	virtual ~FSessionServiceRawBase() = default;
	
	void SetDependencies(FSessionPtr InSession);
	
	////////////////////////////////////////////////////////////////
	// Notifications.
	////////////////////////////////////////////////////////////////

	DECLARE_EVENT_TwoParams(FSessionServiceRawBase, FSessionChangedV1NotificationEvent,
		FPragma_Session_SessionChangedV1Notification /* Notification */, const FPragmaMessageMetadata& /* Metadata */);

	FSessionChangedV1NotificationEvent OnSocialSessionChangedV1Notification;
	FSessionChangedV1NotificationEvent OnGameSessionChangedV1Notification;

	DECLARE_EVENT_TwoParams(FSessionServiceRawBase, FSessionTerminatedV1NotificationEvent,
		FPragma_Session_SessionTerminatedV1Notification /* Notification */,
		const FPragmaMessageMetadata& /* Metadata */);

	FSessionTerminatedV1NotificationEvent OnSocialSessionTerminatedV1Notification;
	FSessionTerminatedV1NotificationEvent OnGameSessionTerminatedV1Notification;

	////////////////////////////////////////////////////////////////
	// RPC Calls.
	////////////////////////////////////////////////////////////////

	DECLARE_DELEGATE_TwoParams(FRequestLogoutV1Delegate,
		TPragmaResult<FPragma_Session_RequestLogoutV1Response> /* Result */,
		const FPragmaMessageMetadata& /* Metadata */);
	void RequestSocialLogoutV1(const FPragma_Session_RequestLogoutV1Request& Request,
		FRequestLogoutV1Delegate Delegate) const;
	void RequestSocialLogoutV1(const FPragma_Session_RequestLogoutV1Request& Request,
		TUniqueFunction<void(TPragmaResult<FPragma_Session_RequestLogoutV1Response>, const FPragmaMessageMetadata&)>
		Callback) const;
	void RequestGameLogoutV1(const FPragma_Session_RequestLogoutV1Request& Request,
		FRequestLogoutV1Delegate Delegate) const;
	void RequestGameLogoutV1(const FPragma_Session_RequestLogoutV1Request& Request,
		TUniqueFunction<void(TPragmaResult<FPragma_Session_RequestLogoutV1Response>, const FPragmaMessageMetadata&)>
		Callback) const;

	DECLARE_DELEGATE_TwoParams(FHeartbeatV1Delegate, TPragmaResult<FPragma_Session_HeartbeatV1Response> /* Result */,
		const FPragmaMessageMetadata& /* Metadata */);
	void SocialHeartbeatV1(const FPragma_Session_HeartbeatV1Request& Request, FHeartbeatV1Delegate Delegate) const;
	void SocialHeartbeatV1(const FPragma_Session_HeartbeatV1Request& Request,
		TUniqueFunction<void(TPragmaResult<FPragma_Session_HeartbeatV1Response>, const FPragmaMessageMetadata&)>
		Callback) const;
	void GameHeartbeatV1(const FPragma_Session_HeartbeatV1Request& Request, FHeartbeatV1Delegate Delegate) const;
	void GameHeartbeatV1(const FPragma_Session_HeartbeatV1Request& Request,
		TUniqueFunction<void(TPragmaResult<FPragma_Session_HeartbeatV1Response>, const FPragmaMessageMetadata&)>
		Callback) const;

	DECLARE_DELEGATE_TwoParams(FHeartbeatPartnerV1Delegate, TPragmaResult<FPragma_Session_HeartbeatPartnerV1Response> /* Result */,
	const FPragmaMessageMetadata& /* Metadata */);
	void SocialHeartbeatPartnerV1(const FPragma_Session_HeartbeatPartnerV1Request& Request, FHeartbeatPartnerV1Delegate Delegate) const;
	void SocialHeartbeatPartnerV1(const FPragma_Session_HeartbeatPartnerV1Request& Request,
		TUniqueFunction<void(TPragmaResult<FPragma_Session_HeartbeatPartnerV1Response>, const FPragmaMessageMetadata&)>
		Callback) const;
	void GameHeartbeatPartnerV1(const FPragma_Session_HeartbeatPartnerV1Request& Request, FHeartbeatPartnerV1Delegate Delegate) const;
	void GameHeartbeatPartnerV1(const FPragma_Session_HeartbeatPartnerV1Request& Request,
		TUniqueFunction<void(TPragmaResult<FPragma_Session_HeartbeatPartnerV1Response>, const FPragmaMessageMetadata&)>
		Callback) const;

	DECLARE_DELEGATE_TwoParams(FSessionRefreshV1Delegate,
		TPragmaResult<FPragma_Session_SessionRefreshV1Response> /* Result */,
		const FPragmaMessageMetadata& /* Metadata */);
	virtual void SocialSessionRefreshV1(const FPragma_Session_SessionRefreshV1Request& Request,
		FSessionRefreshV1Delegate Delegate) const;
	void SocialSessionRefreshV1(const FPragma_Session_SessionRefreshV1Request& Request,
		TUniqueFunction<void(TPragmaResult<FPragma_Session_SessionRefreshV1Response>, const FPragmaMessageMetadata&)>
		Callback) const;
	virtual void GameSessionRefreshV1(const FPragma_Session_SessionRefreshV1Request& Request,
		FSessionRefreshV1Delegate Delegate) const;
	void GameSessionRefreshV1(const FPragma_Session_SessionRefreshV1Request& Request,
		TUniqueFunction<void(TPragmaResult<FPragma_Session_SessionRefreshV1Response>, const FPragmaMessageMetadata&)>
		Callback) const;

protected:
	FSessionPtr Session() const { return SessionPtr; }
	void Initialize();

private:
	FSessionPtr SessionPtr;
};

}

