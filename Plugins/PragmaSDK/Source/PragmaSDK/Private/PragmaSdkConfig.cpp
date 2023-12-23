
#include "PragmaSdkConfig.h"

#include "Misc/Paths.h"
#include "PragmaLog.h"

#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1
#include "Misc/ConfigCacheIni.h"
#endif

namespace
{
void ApplyBackendAddressOverride(const FString& CliParam, FString& OutValue)
{
	FString Value;
	if (FParse::Value(FCommandLine::Get(), *CliParam, Value)
		&& OutValue != Value)
	{
		PRAGMA_LOG(Log, "UPragmaSdkConfig::ApplyBackendAddressOverride -- Found command line override: %s=%s", *CliParam, *Value);
		OutValue = MoveTemp(Value);
	}
	if (!OutValue.IsEmpty() && !OutValue.StartsWith("http"))
	{
		PRAGMA_LOG(Log, "UPragmaSdkConfig::ApplyBackendAddressOverride -- cli override '%s' does not start with http or https. Defaulting to https.", *CliParam);
		OutValue = FString::Printf(TEXT("%s%s"), TEXT("https://"), *OutValue);
	}
}

void ApplyProtocolTypeOverride(const FString& CliParam, EPragmaProtocolType& OutValue)
{
	FString Name;
	if (FParse::Value(FCommandLine::Get(), *CliParam, Name))
	{
		const int64 Value = StaticEnum<EPragmaProtocolType>()->GetValueByNameString(Name);
		if (Value != INDEX_NONE)
		{
			OutValue = static_cast<EPragmaProtocolType>(Value);
			PRAGMA_LOG(Log, "UPragmaSdkConfig::ApplyProtocolTypeOverride -- Found command line override: %s=%s", *CliParam, *Name);
		}
		else
		{
			PRAGMA_LOG(Warning, "UPragmaSdkConfig::ApplyProtocolTypeOverride -- Unknown value %s specified in CLI override %s", *Name, *CliParam);
		}
	}
}
}

void UPragmaSdkConfig::PostInitProperties()
{
	Super::PostInitProperties();
	LoadOverrideConfig();
}

// If the config reloads, we want to make sure we reload out override config as well.
void UPragmaSdkConfig::PostReloadConfig(FProperty* PropertyThatWasLoaded)
{
	Super::PostReloadConfig(PropertyThatWasLoaded);
	LoadOverrideConfig();
}

void UPragmaSdkConfig::LoadOverrideConfig()
{
	// If the more specific file exists, load over top of existing config.
	// A few options are available for choice of location.

	if (auto OverrideIniPath = OverrideConfigIniFilePath())
	{
		LoadConfigFromFile(OverrideIniPath.GetValue());
	}
	else
	{
		static const FString FileName = "Pragma.ini";
		if (!LoadConfigFromFile(FPaths::RootDir() + FileName))
		{
			LoadConfigFromFile(FPaths::ProjectConfigDir() + FileName);
		}
	}

	ApplyBackendAddressOverrides();
	ApplyDebugOverrides();
	ApplyProtocolHostPortOverrides();
	ApplyProtocolTypeOverrides();
	ApplyPartnerTokenOverrides();
}

bool UPragmaSdkConfig::LoadConfigFromFile(const FString& Path)
{
	if (FPaths::FileExists(Path))
	{
		PRAGMA_LOG(Log, "UPragmaSdkConfig -- Found override config at %s, loading...", *Path);
#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 1
		LoadConfig(nullptr, *FConfigCacheIni::NormalizeConfigIniPath(Path));
#else
		LoadConfig(nullptr, *Path);
#endif
		return true;
	}
	return false;
}

TOptional<FString> UPragmaSdkConfig::OverrideConfigIniFilePath() const
{
	FString OverrideIniPath;
	if (FParse::Value(FCommandLine::Get(), TEXT("-PragmaIni="), OverrideIniPath))
	{
		return OverrideIniPath;
	}
	return {};
}

void UPragmaSdkConfig::ApplyBackendAddressOverrides()
{
	ApplyBackendAddressOverride("-PragmaBackendAddress=", BackendAddress);
	ApplyBackendAddressOverride("-PragmaPartnerBackendAddress=", PartnerBackendAddress);
}

void UPragmaSdkConfig::ApplyProtocolHostPortOverrides()
{
	ApplyCliOverride(TEXT("-OverrideSocialProtocol="), OverrideSocialProtocol);
	ApplyCliOverride(TEXT("-OverrideGameProtocol="), OverrideGameProtocol);
	ApplyCliOverride(TEXT("-OverrideSocialHost="), OverrideSocialHost);
	ApplyCliOverride(TEXT("-OverrideGameHost="), OverrideGameHost);
	ApplyCliOverride(TEXT("-OverrideSocialPort="), OverrideSocialPort);
	ApplyCliOverride(TEXT("-OverrideGamePort="), OverrideGamePort);
}

void UPragmaSdkConfig::ApplyProtocolTypeOverrides()
{
	ApplyProtocolTypeOverride("-PragmaProtocolType=", ProtocolType);
	PRAGMA_LOG(Log, "UPragmaSdkConfig -- ProtocolType set to %s", *StaticEnum<EPragmaProtocolType>()->GetNameStringByValue(static_cast<int64>(ProtocolType)));
	ApplyProtocolTypeOverride("-PragmaPartnerProtocolType=", PartnerProtocolType);
	PRAGMA_LOG(Log, "UPragmaSdkConfig -- PartnerProtocolType set to %s", *StaticEnum<EPragmaProtocolType>()->GetNameStringByValue(static_cast<int64>(PartnerProtocolType)));
}

void UPragmaSdkConfig::ApplyPartnerTokenOverrides()
{
	ApplyCliOverride(TEXT("-PragmaPartnerSessionSocialAuthToken="), PartnerSessionSocialAuthToken);
	ApplyCliOverride(TEXT("-PragmaPartnerSessionGameAuthToken="), PartnerSessionGameAuthToken);
}

void UPragmaSdkConfig::ApplyDebugOverrides()
{
#if UE_BUILD_SHIPPING
	bDebugServer = false;
#else
	if (FParse::Value(FCommandLine::Get(), TEXT("-PragmaDebug="), DebugServerPort))
	{
		PRAGMA_LOG(Log, "UPragmaSdkConfig::LoadOverrideConfig -- Found command line override for PragmaDebug: %d", DebugServerPort);
		bDebugServer = (DebugServerPort > 0);
		if (!bDebugServer)
		{
			PRAGMA_LOG(Log, "UPragmaSdkConfig::LoadOverrideConfig -- PragmaDebug is <= 0. Debug server disabled.");
		}
	}
#endif
}
