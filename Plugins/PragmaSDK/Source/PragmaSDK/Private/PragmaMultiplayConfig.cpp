#include "PragmaMultiplayConfig.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "PragmaLog.h"
#include "PragmaJson.h"

void UPragmaMultiplayConfig::SetTimerManager(TSharedPtr<FPragmaTimerManager> InTimerManager)
{
	TimerManager = MoveTemp(InTimerManager);
}

void UPragmaMultiplayConfig::StartWatchingForChanges()
{
	if (!TimerManager.IsValid())
	{
		PRAGMA_LOG(Error, "UPragmaMultiplayConfig must be initialized with a PragmaTimerManager (see SetTimerManager()) from PragmaSession::GetTimerManager().");
		return;
	}
	if (!FParse::Value(FCommandLine::Get(), *ServerJsonCliParam, ServerJsonAbsolutePath))
	{
		Status = EPragmaMultiplayConfigStatus::MissingServerJsonCliParam;
		PRAGMA_LOG(Warning, "UPragmaMultiplayConfig expected a command-line param %s but did not find one..", *ServerJsonCliParam);
		return;
	}
#if PLATFORM_LINUX
	if (ServerJsonAbsolutePath.StartsWith("/") || ServerJsonAbsolutePath.StartsWith("\\"))
	{
		// Multiplay passes in relative paths starting with a slash. Unreal will read that as a root path.
		ServerJsonAbsolutePath = ServerJsonAbsolutePath.RightChop(1);
	}
#endif

	if (IsWatchingForChanges())
	{
		return;
	}
	PRAGMA_LOG(Log, "UPragmaMultiplayConfig::StartWatchingForChanges at path %s", *ServerJsonAbsolutePath);
	SetStatus(EPragmaMultiplayConfigStatus::Waiting);
	// Multiplay may start the server with a server.json file without a session id, and then update that session id
	// later. To handle this, we reload the file periodically until the session id exists. Note that it's possible to be
	// _significantly_ later, since Multiplay will warm up servers and let them hang around until needed.
	ServerJsonPollStartTime = static_cast<float>(FPlatformTime::Seconds());
	TimerManager->SetTimer(ServerJsonPollTimerHandle, FPragmaTimerDelegate::CreateUObject(this, &UPragmaMultiplayConfig::CheckForServerJsonChanges), /* Rate */ PollRateSeconds, /* Loop */ true);
}

void UPragmaMultiplayConfig::StopWatchingForChanges()
{
	if (!TimerManager.IsValid())
	{
		PRAGMA_LOG(Error, "UPragmaMultiplayConfig must be initialized with a PragmaTimerManager from the PragmaSession.");
		return;
	}
	if (!IsWatchingForChanges())
	{
		return;
	}
	PRAGMA_LOG(Log, "UPragmaMultiplayConfig::StopWatchingForChanges at path %s", *ServerJsonAbsolutePath);
	TimerManager->ClearTimer(ServerJsonPollTimerHandle);
}

FString UPragmaMultiplayConfig::GetAllocationId() const
{
	if (ServerJsonObject == nullptr)
	{
		return FString();
	}
	return ServerJsonObject->GetStringField(AllocationIdJsonKey);
}

void UPragmaMultiplayConfig::CheckForServerJsonChanges()
{
	ServerJsonObject = LoadServerJson();
	if (ServerJsonObject == nullptr)
	{
		return;
	}
	const double TimeSpentPolling = FPlatformTime::Seconds() - ServerJsonPollStartTime;
	PRAGMA_LOG(Log, "UPragmaMultiplayConfig::PollForMultiplayServerJson -- Found server.json with allocationId at path: %s after %f seconds of polling.",
		*ServerJsonAbsolutePath, TimeSpentPolling);
	SetStatus(EPragmaMultiplayConfigStatus::Valid);
}

TSharedPtr<FJsonObject> UPragmaMultiplayConfig::LoadServerJson()
{
	if (!FPaths::FileExists(ServerJsonAbsolutePath))
	{
		// Wait for file to exist.
		return nullptr;
	}
	FString ServerJson;
	if (!FFileHelper::LoadFileToString(ServerJson, *ServerJsonAbsolutePath))
	{
		SetStatus(EPragmaMultiplayConfigStatus::ErrorReadingFile);
		return nullptr;
	}
	
	auto JsonObject = PragmaJson::JsonStrToObj(ServerJson);
	if (JsonObject == nullptr)
	{
		SetStatus(EPragmaMultiplayConfigStatus::ErrorReadingFile);
		return nullptr;
	}
	const FString AllocationId = JsonObject->GetStringField(AllocationIdJsonKey);
	if (AllocationId.IsEmpty() || SeenAllocationIds.Contains(AllocationId))
	{
		return nullptr;
	}
	PRAGMA_LOG(Verbose, "Config changed -- server.json at path: %s, contents: %s", *ServerJsonAbsolutePath, *ServerJson);
	SeenAllocationIds.Emplace(AllocationId);
	return MoveTemp(JsonObject);
}

void UPragmaMultiplayConfig::SetStatus(const EPragmaMultiplayConfigStatus InStatus)
{
	if (InStatus == Status)
	{
		return;
	}
	Status = InStatus;
	if (Status == EPragmaMultiplayConfigStatus::Waiting)
	{
		return;
	}
	StopWatchingForChanges();
	OnStatusChanged.Broadcast(this, Status);
}
