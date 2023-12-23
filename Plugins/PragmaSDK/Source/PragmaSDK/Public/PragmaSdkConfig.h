#pragma once

#include "CoreMinimal.h"
#include "PragmaJson.h"
#include "PragmaTypes.h"
#include "UObject/StrongObjectPtr.h"
#include "Runtime/Launch/Resources/Version.h"

#include "PragmaSdkConfig.generated.h"

//
// Runtime Pragma SDK Config.
//
// There are multiple ways to set these values. They are listed below, in order of priority. If a method higher in the
// list is used, it will override the ones below it.
//
// [PRAG-298] NOTE: These values can be overridden at runtime if config is changed and reloaded on disk. We plan to make the reloading opt-in.
//
//
// 1. Code:
// Set the members directly on the Pragma Runtime's Config.
//
//
// 2. Commandline arg:
// Specify a url on the commandline "-Pragma<variable>=1234".
// e.g. -PragmaBackendAddress=http://127.0.0.1:10000
//
//
// 3. Pragma.ini override. [RECOMMENDED]
//
// This method is recommended because it keeps config in a generic ini file. Ship it alongside your game and it just
// works, no need to configure command lines per distribution platform. It can be also be defined at deploy-time which
// means you can use different values across different deploys with the same server or client binaries.
//
// File: %RootDir%/Pragma.ini
// OR:	 %ProjectDir%/Config/Pragma.ini
//     [/Script/PragmaSdk.PragmaSdkConfig]
//     BackendAddress=http://127.0.0.1:10000
//
// The Pragma.ini path can be specified explicitly with the `-PragmaIni=my/path/Pragma.ini` command line. Note that
// you need to specify the filename as well in this case.
//
// Unlike Game.ini, this override will work in UE_BUILD_SHIPPING clients (only confirmed for desktop clients).
//
// NOTE: Do not package a Pragma.ini file into the build, or you will NOT be able to override it! The packaged version
// will always be used. Use the Game config to package SDK Config with the build.
//
//
// 4. Game.ini override.
//
// This does NOT work in UE_BUILD_SHIPPING _client_ builds. For this reason, it's recommended to use the Pragma.ini method.
//
// File: %ProjectDir%/Saved/Config/%Platform%/Game.ini
//     [/Script/PragmaSdk.PragmaSdkConfig]
//     BackendAddress=http://127.0.0.1:10000
//
// 5. Game.ini (packaged into build).
//
// This is a good way to set default values, but still recommend not putting internal urls in here that may accidentally
// leak into a shipping build.
//
// File: %ProjectDir%/Config/DefaultGame.ini
//     [/Script/PragmaSdk.PragmaSdkConfig]
//     BackendAddress=http://127.0.0.1:10000
//
UCLASS(Config=Game)
class PRAGMASDK_API UPragmaSdkConfig final : public UObject
{
    GENERATED_BODY()

public:
	const FString& GetBackendAddress(const bool PartnerSession) const { return PartnerSession && !PartnerBackendAddress.IsEmpty() ? PartnerBackendAddress : BackendAddress; }
	EPragmaProtocolType GetProtocolType(const bool PartnerSession) const { return PartnerSession ? PartnerProtocolType : ProtocolType; }
	const FString& GetGameClientVersion() const { return GameClientVersion; }
	const FString& GetPartnerSessionGameAuthToken() const { return PartnerSessionGameAuthToken; }
	const FString& GetPartnerSessionSocialAuthToken() const { return PartnerSessionSocialAuthToken; }
	bool GetDebugServerEnabled() const { return bDebugServer; }
	int32 GetDebugServerPort() const { return DebugServerPort; }
	float GetHeartbeatPeriodSeconds() const { return HeartbeatPeriodSeconds; }
	float GetBackoffInitialRetrySeconds() const { return BackoffInitialRetrySeconds; }
	float GetBackoffStaggerReconnectRandomSeconds() const { return BackoffStaggerReconnectRandomSeconds; }
	float GetBackoffMaxSecondsBetweenRetries() const { return BackoffMaxSecondsBetweenRetries; }
    float GetBackoffMaxConnectionReconnectSeconds() const { return BackoffMaxConnectionReconnectSeconds; }
	bool GetLogPragmaDetailedMessages() const { return bLogPragmaDetailedMessages; }

    virtual void PostInitProperties() override;

#if ENGINE_MAJOR_VERSION < 5 && ENGINE_MINOR_VERSION < 25
	typedef UProperty FProperty;
#endif

    // If the config reloads, we want to make sure we reload our override config as well.
    virtual void PostReloadConfig(FProperty* PropertyThatWasLoaded) override;

	template <typename T>
    bool GetUserDataAs(T& OutData) const
	{
		return PragmaJson::JsonStrToStruct(UserData, OutData);
	}

private:
    void LoadOverrideConfig();
	bool LoadConfigFromFile(const FString& Path);
	TOptional<FString> OverrideConfigIniFilePath() const;

	void ApplyBackendAddressOverrides();
	void ApplyProtocolHostPortOverrides();
	void ApplyProtocolTypeOverrides();
	void ApplyDebugOverrides();
	void ApplyPartnerTokenOverrides();

	template <typename T>
	void ApplyCliOverride(const FString& CliParam, T& Target)
	{
		T Value;
		if (FParse::Value(FCommandLine::Get(), *CliParam, Value))
		{
			Target = Value;
		}
	}

public:
	//
	// The platform backend address that will be used for Pragma::FPlayers.
	//
	// Must be in the form of scheme://hostname:port, e.g. http://127.0.0.1:10000
	// Note that this is only used for a single call to get the full backend info, which is then used to construct
	// full URLs for all supported schemes.
	//
	// Override via command line with -PragmaBackendAddress="http://127.0.0.1:10000"
	//
	// Does NOT support hot reload after a session's first connection attempt.
	//
	UPROPERTY(Config, VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString BackendAddress;

	//
	// The platform backend address that will be used for Pragma::FServers.
	//
	// Must be in the form of scheme://hostname:port, e.g. http://127.0.0.1:10100
	// Note that this is only used for a single call to get the full backend info, which is then used to construct
	// full URLs for all supported schemes.
	//
	// Override via command line with -PragmaPartnerBackendAddress="http://127.0.0.1:10100"
	//
	// If not specified, will fallback to using BackendAddress.
	//
	// Does NOT support hot reload after a session's first connection attempt.
	//
	UPROPERTY(Config, VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString PartnerBackendAddress;

	//
	// The game client version used for server version compatibility matching.
	//
	// Does NOT support hot reload after a session's first connection attempt.
	//
	UPROPERTY(Config, VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString GameClientVersion;

	//
	// In normal operation, social/game info is pulled from a /v1/info call prior to logging in.
	//
	// In development it can be necessary to override portions of that information, e.g. in the case of running a local game server
	// with a remote Pragma backend. These overrides exist to support those type of cases. If you're trying to use these in production,
	// something is probably misconfigured on the backend.
	//
	// The overrides follow the pattern:
	// <protocol>://<host>:<port>
	//
	// Does NOT support hot reload after a session's first connection attempt.
	//
	UPROPERTY(Config, VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString OverrideSocialProtocol;
	UPROPERTY(Config, VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString OverrideGameProtocol;
	UPROPERTY(Config, VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString OverrideSocialHost;
	UPROPERTY(Config, VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	FString OverrideGameHost;
	UPROPERTY(Config, VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	int32 OverrideSocialPort{-1};
	UPROPERTY(Config, VisibleAnywhere, BlueprintReadOnly, Category=Pragma)
	int32 OverrideGamePort{-1};

	//
	// This is a token that should be injected into the Pragma.ini file that deploys alongside a game server. The game
	// server uses this auth token to connect to the game backend.
	//
	// Does NOT support hot reload after a session's first connection attempt.
	//
	UPROPERTY(Config)
	FString PartnerSessionGameAuthToken;

	//
	// This is a token that should be injected into the Pragma.ini file that deploys alongside a game server. The game
	// server uses this auth token to connect to the social backend.
	//
	// Does NOT support hot reload after a session's first connection attempt.
	//
	UPROPERTY(Config)
	FString PartnerSessionSocialAuthToken;

	//
	// The Protocol type that will be used for Player RPC communication with the platform after authentication, e.g. Http or WebSocket.
	// NOTE: Http currently does not support notifications.
	//
	// Override via command line with -PragmaProtocolType={Http,WebSocket}
	//
	// Does NOT support hot reload after a session's first connection attempt.
	//
	UPROPERTY(Config)
	EPragmaProtocolType ProtocolType{EPragmaProtocolType::WebSocket};

	//
	// The Protocol type that will be used for Game Server RPC communication with the platform, e.g. Http or WebSocket.
	// NOTE: Http currently does not support notifications.
	//
	// Override with -PragmaServerProtocolType={Http,WebSocket}
	//
	// Does NOT support hot reload after a session's first connection attempt.
	//
	UPROPERTY(Config)
	EPragmaProtocolType PartnerProtocolType{EPragmaProtocolType::WebSocket};

	//
	// Convenience field for Pragma users to add json (or other data) to follow the same override semantics as the rest of Pragma.ini, e.g. for deploying
	// additional info alongside the binaries without rewriting this functionality.
	//
	UPROPERTY(Config)
	FString UserData;

	////////////////////////////////////////////////////////////////////////////////////////////////
	// Debug server.

	//
	// If the debug server should be started.
	// Overridden to if the command line -PragmaDebug is present. Explicitly disable server with -PragmaDebug=0
	//
	UPROPERTY(Config)
	bool bDebugServer{false};

	//
	// If the PRAGMA_LOG facility should log more detailed messages. Default is false to exclude them.
	// Primarily meant to reduce noise within the log file as these can occur at high volume.
	//
	UPROPERTY(Config)
	bool bLogPragmaDetailedMessages{false};

	//
	// Port to use for the debug server, if enabled.
	// Override with -PragmaDebug=1234.
	//
	UPROPERTY(Config)
	int32 DebugServerPort{9999};

	//
	// Period seconds between heartbeat invocations.
	//
	// Does NOT support hot reload after a session's first connection attempt.
	//
	UPROPERTY(Config)
	float HeartbeatPeriodSeconds{30.f};

	//
	// Seconds to wait before the first retry. Note that this has a direct effect on the rest of the retries, and how quickly we hit the maximum,
	// because it seeds the exponential backoff.
	//
	// Does NOT support hot reload after a session's first connection attempt.
	//
	UPROPERTY(Config)
	float BackoffInitialRetrySeconds{1.0f};

	//
	// Each client adds between 0 and staggerReconnectRandomSeconds seconds to each retry, to lessen the effect of synchronized retries across clients.
	//
	// Does NOT support hot reload after a session's first connection attempt.
	//
	UPROPERTY(Config)
	float BackoffStaggerReconnectRandomSeconds{1.0f};

	//
	// When time between retries hits this value, the retry time will stop getting longer.
	// Note that backoff increases exponentially, so a power of two value (32, 64, 128) is a sensible choice.
	//
	// Does NOT support hot reload after a session's first connection attempt.
	//
	UPROPERTY(Config)
	float BackoffMaxSecondsBetweenRetries{64.0f};

	// Each websocket will try to reconnect indefinitely in isolation. If both websockets are down, it's likely indicative of a larger issue.
	// In this case, it is prudent to stop trying after some amount of time and consider ourself completely disconnect (i.e. boot the player
	// back to the login screen).
	//
	// -1 indicates allow reconnect to retry forever even when both websockets are down.
	//
	// Does NOT support hot reload after a session's first connection attempt.
	UPROPERTY(Config)
	float BackoffMaxConnectionReconnectSeconds{300.0f};

};

namespace Pragma
{
typedef TStrongObjectPtr<UPragmaSdkConfig> FSdkConfigStrongPtr;
}
