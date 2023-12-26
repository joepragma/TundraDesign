#pragma once

#include "PragmaConnectionInfo.h"
#include "PragmaProtocol.h"
#include "PragmaSdkConfig.h"
#include "PragmaConnection.h"
#include "PragmaPtr.h"
#include "PragmaConnectionStatus.h"

class IPragmaTimerManager;

PRAGMA_PTR_API(FSession);

// ************************************************************
// *** THIS FILE WAS STUBBED FOR THIS TUNDRA DESIGN PROJECT ***
// ************************************************************

namespace Pragma
{
/**
 * Base type for Pragma Session objects. Create different session types via the Runtime.
 */
class PRAGMASDK_API FSession
{
public:
	DECLARE_EVENT(FSession, FRestartRequiredEvent);
	FRestartRequiredEvent OnRestartRequired;

	FSession(FSdkConfigStrongPtr Config, TSharedPtr<IPragmaTimerManager> TimerManager);
	virtual ~FSession();

	// Move only since the destructor will destroy the core session.
	FSession(FSession&&) noexcept = default;
	FSession& operator=(FSession&&) noexcept = default;
	FSession(const FSession&) = delete;
	FSession& operator=(const FSession&) = delete;

	virtual FString GameShardId() const;

	// Returns true if the session is in the process of connecting.
	bool IsConnecting() const;

	// Returns true if the player is fully connected.
	bool IsConnected() const;

	// General information on connection status.
	FConnectionInfo ConnectionInfo() const;

	DECLARE_EVENT(FSession, FConnectionEvent);

	// The Session has successfully connected to the Pragma platform.
	FConnectionEvent OnConnected;

	// The Session was full disconnected from the Pragma platform and will need to log in again. See `ConnectionInfo()` for errors.
	FConnectionEvent OnDisconnected;

	// One or more of the Session's connections with the Pragma platform have failed. See `ConnectionInfo()` for more information.
	FConnectionEvent OnDegraded;

	// All connections have recovered and the Session is considered fully connected.
	FConnectionEvent OnReconnected;

	DECLARE_EVENT_OneParam(FSession, FBackendConnectionEvent, EPragmaConnectionStatus);

	// The status of the connection to the Social backend changed. May be fired in addition to the other connection events.
	FBackendConnectionEvent OnSocialStatusChanged;

	// The status of the connection to the Game backend changed. May be fired in addition to the other connection events.
	FBackendConnectionEvent OnGameStatusChanged;

	FPragmaProtocol& SocialBackend() const;
	FPragmaProtocol& GameBackend() const;

	DECLARE_EVENT(FSession, FInitializedEvent);
	FInitializedEvent OnInitialized;

	virtual bool IsInitialized() const;

	virtual bool IsPartnerSession() const = 0;

protected:
	// This only exists because legacy session/connection doesn't play nice w/ changing SdkConfig after its initialized.
	void InitLegacySession();
	virtual void OnInitLegacy() {}

	DECLARE_DELEGATE(FPreLogInDelegate);

	// These are protected since technically it's Runtime-global, so it's confusing to provide access on each player.
	UPragmaSdkConfig& Config() { return *ConfigPtr; }
	const UPragmaSdkConfig& Config() const { return *ConfigPtr; }

	const TSharedPtr<IPragmaTimerManager>& TimerManager() const { return TimerManagerPtr; }

	template <typename... Args>
	TFunction<void(Args...)> WeakLambda(FSession* This, TFunction<void(Args...)> Fn) const
	{
		TWeakPtr<FSession, SpMode> WeakThis = This->AsSharedSession();
		return [WeakThis, Fn](Args&&... ArgList)
		{
			if (auto Pinned = WeakThis.Pin())
			{
				Fn(Forward<Args>(ArgList)...);
			}
		};
	}

	virtual TSharedPtr<FSession, SpMode> AsSharedSession() = 0;

	UPragmaConnection& Connection() { return *ConnectionPtr; }
	const UPragmaConnection& Connection() const { return *ConnectionPtr; }


	// For tests. I hate it as much as you do.
	void SetConnectionForTest(UPragmaConnection* Connection)
	{
		ConnectionPtr = TStrongObjectPtr<UPragmaConnection>{Connection};
	}

private:
	FSdkConfigStrongPtr ConfigPtr;
	TSharedPtr<IPragmaTimerManager> TimerManagerPtr;

	TStrongObjectPtr<UPragmaConnection> ConnectionPtr;
};

}
