#pragma once

#include "Dto/PragmaPlayerDataDto.h"
#include "Dto/PragmaPlayerDataServiceRpcDto.h"

class UPragmaPlayerDataServiceRaw;

class PRAGMASDK_API FPlayerDataCache
{
public:
	TWeakPtr<FPragma_PlayerData_Entity>         GetEntityByInstanceId(const FString& InstanceId) const;
	TArray<TWeakPtr<FPragma_PlayerData_Entity>> GetEntitiesByName(const FString& Name) const;
	TWeakPtr<FPragma_PlayerData_Entity>         GetUniqueEntityByName(const FString& Name) const;
	TArray<FString>                             AllInstanceIds() const;

	bool HasVersion(int Version) const;
	TOptional<int> GetVersion() const { return CurrentVersion; }

	bool CanProcessSummary(const FPragma_PlayerData_PlayerDataUpdateSummary& UpdateSummary) const;
	void UpdateFromSummary(const FPragma_PlayerData_PlayerDataUpdateSummary& UpdateSummary);
	void UpdateFromFull(const FPragma_PlayerData_PlayerData& PlayerData);
	void Clear();

private:
	TOptional<int>                                             CurrentVersion;
	TMap<FString, TSharedPtr<FPragma_PlayerData_Entity>>       PlayerDataEntitiesMap;
	TMap<FString, TArray<TWeakPtr<FPragma_PlayerData_Entity>>> PlayerDataEntitiesByName;

	void AddNewEntity(const FPragma_PlayerData_Entity& Entity);
};
