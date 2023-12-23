using UnrealBuildTool;

public class PragmaSDK : ModuleRules
{
	public PragmaSDK(ReadOnlyTargetRules Target) : base(Target)
	{
#if UE_4_24_OR_LATER
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ShadowVariableWarningLevel = WarningLevel.Error;
#endif

		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bLegacyPublicIncludePaths = false;

		// When compiling this library, enable "PACKAGE_SCOPE" macros.
		PrivateDefinitions.Add("PRAGMASDK_PACKAGE=1");

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			// Common Unreal types.
			"Core",
			// Object macros, e.g. for JSON conversion DTOs.
			"CoreUObject",
			// Sessions with backend use websockets.
			"WebSockets",
		});
		PublicDependencyModuleNames.AddRange(new string[]
		{
			// Converting DTOs to/from json. These must be public because json conversion is used in template functions
			// available to public functions, e.g. FPragmaSocket::SendMessage.
			"Json",
			"JsonUtilities",
			// Required for initial auth, get backend config, etc. Also messages can be sent over http.
			"HTTP",
			// Used for DebugServer that runs in !UE_BUILD_SHIPPING builds to host info for the SDK debug console.
			"HTTPServer",
		});

		var IsUsingEngine = Target.Type != TargetType.Program;
		if (IsUsingEngine)
		{
			// Usage of this within PragmaSDK should be very rare. If you find you need it, consider if you can instead move something to PragmaSDKAux.
			PublicDefinitions.Add("PRAGMASDK_USE_ENGINE=1");
			PrivateDependencyModuleNames.Add("Engine");
		}
	}
}
