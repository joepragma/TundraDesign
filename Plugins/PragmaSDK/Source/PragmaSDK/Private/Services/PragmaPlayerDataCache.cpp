#include "Services/PragmaPlayerDataCache.h"

#include "Dto/PragmaPlayerDataServiceRaw.h"

bool FPlayerDataCache::HasVersion(int Version) const
{
	return CurrentVersion.IsSet() && CurrentVersion.GetValue() >= Version;
}

bool FPlayerDataCache::CanProcessSummary(const FPragma_PlayerData_PlayerDataUpdateSummary& UpdateSummary) const
{
	return CurrentVersion.IsSet() && UpdateSummary.Version - CurrentVersion.GetValue() == 1;
}

void FPlayerDataCache::UpdateFromSummary(const FPragma_PlayerData_PlayerDataUpdateSummary& UpdateSummary)
{
	if (!ensure(CanProcessSummary(UpdateSummary)))
	{
		return;
	}
	for (auto& Entity : UpdateSummary.ChangedEntities)
	{
		const auto EntityPtrPtr = PlayerDataEntitiesMap.Find(Entity.InstanceId);
		if (EntityPtrPtr != nullptr)
		{
			**EntityPtrPtr = Entity;
		}
		else
		{
			AddNewEntity(Entity);
		}
	}
	for (auto& InstanceId : UpdateSummary.DeletedEntities)
	{
		PlayerDataEntitiesMap.Remove(InstanceId);
	}

	TArray<FString> NamesToRemove;
	for (auto& DataEntitiesByName : PlayerDataEntitiesByName)
	{
		DataEntitiesByName.Value.RemoveAll([](auto Ptr) { return !Ptr.IsValid(); });
		if (DataEntitiesByName.Value.Num() == 0)
			NamesToRemove.Add(DataEntitiesByName.Key);
	}
	for (auto& Name : NamesToRemove)
	{
		PlayerDataEntitiesByName.Remove(Name);
	}
	CurrentVersion = UpdateSummary.Version;
}

void FPlayerDataCache::UpdateFromFull(const FPragma_PlayerData_PlayerData& PlayerData)
{
	PlayerDataEntitiesMap.Empty();
	PlayerDataEntitiesByName.Empty();
	for (const auto& Entity : PlayerData.Entities)
	{
		AddNewEntity(Entity);
	}
	CurrentVersion = PlayerData.Version;
}
void FPlayerDataCache::Clear()
{
	PlayerDataEntitiesMap.Empty();
	PlayerDataEntitiesByName.Empty();
	CurrentVersion.Reset();
}

void FPlayerDataCache::AddNewEntity(const FPragma_PlayerData_Entity& Entity)
{
	auto EntityPtr = MakeShared<FPragma_PlayerData_Entity>(Entity);
	PlayerDataEntitiesMap.Emplace(Entity.InstanceId, EntityPtr);
	PlayerDataEntitiesByName.FindOrAdd(Entity.EntityName.Name).Emplace(EntityPtr);
}

TArray<TWeakPtr<FPragma_PlayerData_Entity>> FPlayerDataCache::GetEntitiesByName(const FString& Name) const
{
	const auto Result = PlayerDataEntitiesByName.Find(Name);
	return Result ? *Result : TArray<TWeakPtr<FPragma_PlayerData_Entity>>();
}

TWeakPtr<FPragma_PlayerData_Entity> FPlayerDataCache::GetUniqueEntityByName(const FString& Name) const
{
	const auto& Entities = GetEntitiesByName(Name);
	return Entities.Num() == 1 ? Entities[0] : nullptr;
}

TArray<FString> FPlayerDataCache::AllInstanceIds() const
{
	TArray<FString> Result;
	PlayerDataEntitiesMap.GetKeys(Result);
	return Result;
}

TWeakPtr<FPragma_PlayerData_Entity> FPlayerDataCache::GetEntityByInstanceId(const FString& InstanceId) const
{
	if (PlayerDataEntitiesMap.Contains(InstanceId))
	{
		return PlayerDataEntitiesMap[InstanceId];
	}
	return nullptr;
}
