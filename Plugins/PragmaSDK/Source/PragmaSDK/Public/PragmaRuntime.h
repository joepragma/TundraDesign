#pragma once

#include "CoreMinimal.h"
#include "PragmaMainThreadTicker.h"
#include "PragmaTimerManager.h"
#include "PragmaPtr.h"
#include "UObject/StrongObjectPtr.h"

class UPragmaSdkConfig;

PRAGMA_FWD(FPlayer);
PRAGMA_FWD(FServer);

PRAGMA_PTR_API(FRuntime);

namespace Pragma
{
class FSession;

/**
 * Entrypoint into the Pragma SDK. Only one should ever exist.
 *
 * Use the Runtime modify SDK configuration and to create and retrieve `Player` or `Server` sessions,
 * which provide more specific functionality relevant to their respective use cases.
 */
class PRAGMASDK_API FRuntime final : public TPragmaSharedFromThis<FRuntime>
{
public:
	static FRuntimeStrongPtr Create()
	{
		auto Ptr = FRuntimeStrongPtr{new FRuntime()};
		Ptr->InitializeTick();
		return Ptr;
	}
	~FRuntime();

	// Direct access to the SDK configuration. See PragmaSdkConfig fields for when changed values will propagate.
	// NOTE: Set configuration BEFORE logging in Players or initializing the Server!
	UPragmaSdkConfig& Config() const { return *ConfigPtr; }

	// Create a new Player session. The Player object is owned by the Runtime and will be destroyed on Runtime destruction, or through an explicit call to
	// DestroyPlayer(). In the typical use case where you create a single Player and never destroy it, the ptr will be valid until shutdown. You can have
	// multiple Players, e.g. split screen coop.
	FPlayerPtr CreatePlayer();

	// Destroy the given Player. Any FServerPtrs referencing this Player will become invalid and any active session will be disconnected.
	void DestroyPlayer(FPlayerPtr Player);

	// Number of existing Player sessions. Note this will count Players in any state.
	int PlayerCount() const;

	// If we have any Player sessions at all.
	bool HasPlayers() const;

	// Get (or create) the first available Player. Convenience accessor for when your game only ever has a single Player session.
	FPlayerPtr GetOrCreatePlayer();

	// Get the first available Player, if it exists.
	FPlayerPtr Player() const;

	// Get (or create) the Server session. The Server object is owned by the Runtime and will be destroyed on Runtime destruction, or through an explicit call to
	// DestroyServer(). In the typical use case where you create a Server and never destroy it, the ptr will be valid until shutdown.
	//
	// If you plan to support multiple concurrent or sequential matches within the same unreal process, you still only need one Server object;
	// that logic is handled by Pragma.
	FServerPtr GetOrCreateServer();

	// Get the Server, if it exists.
	FServerPtr Server() const;

	// True if Server exists.
	bool HasServer() const;

	// Destroy the Server. Any FServerPtrs referencing this Player will become invalid and any active session will be disconnected.
	void DestroyServer();

	// Register an Api type that should be created and registered with all current and future Players or Servers.
	template <typename T>
	void RegisterApi()
	{
		RegisterApi(T::StaticClass());
	}

private:
	// Instantiate using static Create.
	FRuntime();

	void InitializeTick();

	void Tick();

	void RegisterApi(UClass* StaticClass);
	void ApplyRegisteredApisOnSession(FSession& Session);
	void ApplyRegisteredApiOnAllSessions(UClass* StaticClass);

	TStrongObjectPtr<UPragmaSdkConfig> ConfigPtr;
	TArray<FPlayerStrongRef> Players;
	FServerStrongPtr ServerPtr;

	FMainThreadTickerPtr Ticker;
	TSharedPtr<FPragmaTimerManager> TimerManager;

	TSet<UClass*> RegisteredApis;

	// Deprecation warning.
	bool CalledRegisterService{false};
};

}
