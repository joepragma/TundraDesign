using UnrealBuildTool;

public class PragmaSDKAux : ModuleRules
{
	public PragmaSDKAux(ReadOnlyTargetRules Target) : base(Target)
	{
#if UE_4_24_OR_LATER
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ShadowVariableWarningLevel = WarningLevel.Error;
#endif

		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bLegacyPublicIncludePaths = false;

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			// Common Unreal types.
			"Core",
			"CoreUObject",
			// Subsystems.
			"Engine"
		});
		
		PublicDependencyModuleNames.Add("PragmaSDK");
	}
}