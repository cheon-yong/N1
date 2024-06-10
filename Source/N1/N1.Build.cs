// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class N1 : ModuleRules
{
	public N1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { 
			"N1" 
		});

		PublicDependencyModuleNames.AddRange(new string[] {
			"AudioModulation",
			"ApplicationCore",
			"Core",
			"Slate",
			"SlateCore",
			"CoreUObject",
			"RenderCore",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"SignificanceManager",
			"UMG",
			"Niagara",
			"NavigationSystem",
			"AIModule",
			"GameFeatures",
			"GameplayAbilities",
			"GameplayTasks",
			"GameplayTags",
			"GameSettings",
			"ModularGameplayActors",
			"CommonUser",
			"CommonGame",
			"CommonUI",
			"CommonInput",
			"UIExtension",
			"GameplayMessageRuntime",
			"NetCore",
			"PhysicsCore",
			"CommonLoadingScreen",
			"AudioMixer",
			"AudioModulation",
			"GameSubtitles",
			"PropertyPath"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {
			"DeveloperSettings",
		});

		SetupIrisSupport(Target);
	}
}
