// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OpenWorldRPG : ModuleRules
{
	public OpenWorldRPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", "GameplayAbilities","GameplayTags","ModularGameplay","EnhancedInput", "GameplayTasks","StateTreeModule","NavigationSystem"
		});
		
		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"InputCore",
				"Slate",
				"SlateCore",
				"RenderCore",
				"DeveloperSettings",
				"EnhancedInput",
				"NetCore",
				"RHI",
				"Projects",
				"UMG",
				"CommonInput",
				"AudioMixer",
				"ClientPilot",
				"EngineSettings",
				"DTLSHandlerComponent",
				"Json", "ModularGameplay"
			}
		);
	}
}
