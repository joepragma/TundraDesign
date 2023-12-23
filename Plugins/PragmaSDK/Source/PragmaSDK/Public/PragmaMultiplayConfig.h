#pragma once
#include "PragmaJson.h"
#include "PragmaTimerManager.h"
#include "PragmaMultiplayConfig.generated.h"

class FJsonObject;

UENUM(BlueprintType, Category=Pragma)
enum class EPragmaMultiplayConfigStatus : uint8
{
	Waiting, // Ok, Waiting for config. 
	Valid, // Successfully loaded server.json file with an allocationId.

	// The Cli parameter for where to find the server.json is missing. This is an error if you're expecting it
    // (i.e. when deploy to multiplay), but could also be that this object is created in local development, or is a client.
	// This is not harmful, but ideally the object is only created when deployed to Multiplay.
	MissingServerJsonCliParam,

	// Errors loading config.
	InvalidJson, // This likely means your template server.json on Multiplay contains invalid json or one of the template variables is breaking the json. 
	ErrorReadingFile, // Contact your Multiplay representative, this probably shouldn't happen.
};

//
// This is a helper class for using Multiplay from you game server (https://multiplay.com/).
//
// When a server is allocated it passes a command-line (ServerJsonCliParam) that points at a file on disk.
// This file contains information about the allocation relevant to the game server. Notably, that file is not necessarily immediately available,
// and it also may not have the Allocation ID filled out for a long time.
//
// This class handles waiting for the server json file AND the AllocationId, and notifying the caller when found.
//
// Usage:
// // This should be a UPROPERTY-annotated class member for garbage collection.
// MultiplayConfig = NewObject<UPragmaMultiplayConfig>();
// MultiplayConfig->SetTimerManager(PragmaSession->GetTimerManager());
// MultiplayConfig->OnStatusChanged.Add*** // Add Dynamic Multicast Delegate binding.
// MultiplayConfig->StartWatchingForChanges()
//
// // In your OnStatusChanged hook:
// OnStatusChanged(UPragmaMultiplayConfig* Config, EPragmaMultiplayConfigStatus, Status) {
//     if (Status != EPragmaMultiplayConfigStatus::Valid) {
//	       /* Error, config is not ready to be read. */
//	   }
// }
//
// This class can be configured by placing the following in you DefaultGame.ini (see below for variables):
// [/Script/PragmaSdk.PragmaMultiplayConfig]
// Variable=Value
//
UCLASS(BlueprintType, Config=Game, Category=Pragma)
class PRAGMASDK_API UPragmaMultiplayConfig final : public UObject
{
	GENERATED_BODY()

public:
	void SetTimerManager(TSharedPtr<FPragmaTimerManager> InTimerManager);
	
	// Called when either the server.json is available and has an allocationId, or an error occurred.
	// Whenever this fires, config will stop watching for changes to allow you to handle results as desired.
	DECLARE_MULTICAST_DELEGATE_TwoParams(FStatusChanged, UPragmaMultiplayConfig*, /*Config,*/ EPragmaMultiplayConfigStatus/*, Status*/);
	FStatusChanged OnStatusChanged;

	// Watch for server.json changes. OnStatusChanged will be called each time there is a change as long as we are watching.
	void StartWatchingForChanges();

	// Stop watching for server.json changes. OnStatusChanged will no longer be called.
	void StopWatchingForChanges();

	// True if we are currently watching for server.json changes.
	bool IsWatchingForChanges() const { return ServerJsonPollTimerHandle.IsValid(); };

	// Can be empty is Status is not Valid.
	FString GetAllocationId() const;

	// Can be null if Status is not Valid.
	TSharedPtr<FJsonObject> GetServerJson() const { return ServerJsonObject; }

	// Returns false if cannot be deserialized as the requested struct.
	template <typename T>
	bool GetServerJson(T& OutStruct) const
	{
		return PragmaJson::JsonObjToStruct(ServerJsonObject, &OutStruct);
	}

	bool IsConfigValid() const { return Status == EPragmaMultiplayConfigStatus::Valid && ServerJsonObject != nullptr; }
	EPragmaMultiplayConfigStatus GetStatus() const { return Status; }

private:
	void CheckForServerJsonChanges();
	TSharedPtr<FJsonObject> LoadServerJson();
	void SetStatus(const EPragmaMultiplayConfigStatus InStatus);

private:
	// The CLI param that will be passed to the executable where to find the Multiplay server.json file.
	UPROPERTY(Config)
	FString ServerJsonCliParam{ TEXT("-serverjson=") };

	// Name of the key within server.json where the AllocationId is found.
	UPROPERTY(Config)
	FString AllocationIdJsonKey{ TEXT("SessionId") };

	// Interval in seconds between checking for server.json file changes. 
	UPROPERTY(Config)
	float PollRateSeconds{ 2.0f };

	TSharedPtr<FPragmaTimerManager> TimerManager;
	
	TSharedPtr<FJsonObject> ServerJsonObject;
	EPragmaMultiplayConfigStatus Status;

	// Track allocationIds that we've seen so we can tell if the file has changed.
	TSet<FString> SeenAllocationIds;
	
	FString ServerJsonAbsolutePath;
	FPragmaTimerHandle ServerJsonPollTimerHandle;
	float ServerJsonPollStartTime{ 0.f };
};
