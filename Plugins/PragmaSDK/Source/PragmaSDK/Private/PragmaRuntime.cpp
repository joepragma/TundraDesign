#include "PragmaRuntime.h"

#include "PragmaPlayer.h"
#include "PragmaServer.h"
#include "PragmaSdkConfig.h"


namespace Pragma
{

FRuntime::FRuntime()
{
	PRAGMA_LOG(Display, "Creating new Runtime...");
	ConfigPtr = TStrongObjectPtr<UPragmaSdkConfig>{NewObject<UPragmaSdkConfig>()};
	Ticker = FMainThreadTicker::Create();
	TimerManager = MakeShared<FPragmaTimerManager>();
}

void FRuntime::InitializeTick()
{
	// Must do the shared pointer init after constructor for AsShared() to work.

	// todo should be able to use AddSP here, but iOS build fails with it.
	TWeakPtr<FRuntime, SpMode> WeakThisLocal = AsShared();
	Ticker->OnTick.AddLambda([WeakThisLocal]
	{
		const auto This = WeakThisLocal.Pin();
		if (This != nullptr)
		{
			This->Tick();
		}
	});
	Ticker->AddTickable(TimerManager);
}

FRuntime::~FRuntime()
{
	PRAGMA_LOG(Display, "Destroying Runtime...");
	// Stop ticking immediately to prevent calling tick/delegates while destroying.
	Ticker->Stop();
}

FPlayerPtr FRuntime::CreatePlayer()
{
	PRAGMA_LOG(Display, "Creating new Player...");
	auto Player = FPlayer::Create(ConfigPtr, TimerManager);
	ApplyRegisteredApisOnSession(*Player);
	Players.Add(Player);
	return Player;
}

void FRuntime::DestroyPlayer(const FPlayerPtr Player)
{
	if (const auto Pinned = Player.Pin())
	{
		PRAGMA_LOG(Display, "Destroying Player...");
		Players.Remove(Pinned.ToSharedRef());
	}
}

int FRuntime::PlayerCount() const
{
	return Players.Num();
}

bool FRuntime::HasPlayers() const
{
	return PlayerCount() > 0;
}

FPlayerPtr FRuntime::GetOrCreatePlayer()
{
	return Player() != nullptr ? Player() : CreatePlayer();
}

FPlayerPtr FRuntime::Player() const
{
	return Players.Num() > 0 ? Players[0] : FPlayerPtr{};
}

FServerPtr FRuntime::GetOrCreateServer()
{
	if (ServerPtr == nullptr)
	{
		PRAGMA_LOG(Display, "Creating new Server...");
		ServerPtr = FServer::Create(ConfigPtr, TimerManager);
		ApplyRegisteredApisOnSession(*ServerPtr);
	}
	return ServerPtr;
}

FServerPtr FRuntime::Server() const
{
	return ServerPtr != nullptr ? ServerPtr : nullptr;
}

bool FRuntime::HasServer() const
{
	return ServerPtr != nullptr;
}

void FRuntime::DestroyServer()
{
	if (!ServerPtr.IsValid())
	{
		return;
	}
	PRAGMA_LOG(Display, "Destroying Server...");
	ServerPtr.Reset();
}

void FRuntime::Tick()
{
}

void FRuntime::RegisterApi(UClass* StaticClass)
{
	RegisteredApis.Add(StaticClass);
	ApplyRegisteredApiOnAllSessions(StaticClass);
}

void FRuntime::ApplyRegisteredApisOnSession(FSession& Session)
{
	for (auto* StaticClass : RegisteredApis)
	{
		if (StaticClass->GetSuperClass() == UPragmaPlayerApi::StaticClass())
		{
			StaticCast<FPlayer&>(Session).RegisterApi(StaticClass);
		}
		else if (StaticClass->GetSuperClass() == UPragmaServerApi::StaticClass())
		{
			if (ServerPtr.IsValid())
			{
				StaticCast<FServer&>(Session).RegisterApi(StaticClass);
			}
		}
		else
		{
			PRAGMA_LOG(Warning, "Unable to register class %s as it was neither a UPragmaPlayerApi nor a UPragmaServerApi.", *StaticClass->GetName());
		}
	}
}

void FRuntime::ApplyRegisteredApiOnAllSessions(UClass* StaticClass)
{
	if (StaticClass->GetSuperClass() == UPragmaPlayerApi::StaticClass())
	{
		for (const auto& Player : Players)
		{
			Player->RegisterApi(StaticClass);
		}
	}
	else if (StaticClass->GetSuperClass() == UPragmaServerApi::StaticClass())
	{
		if (ServerPtr.IsValid())
		{
			ServerPtr->RegisterApi(StaticClass);
		}
	}
	else
	{
		PRAGMA_LOG(Warning, "Unable to register class %s as it was neither a UPragmaPlayerApi nor a UPragmaServerApi.", *StaticClass->GetName());
	}
}
}
