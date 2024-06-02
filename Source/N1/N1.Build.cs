// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class N1 : ModuleRules
{
	public N1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { "N1" });

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"UMG",
			"Niagara",
			"NavigationSystem",
			"AIModule",
			"GameFeatures",
			"GameplayAbilities",
			"GameplayTasks",
			"GameplayTags",
			"ModularGameplayActors",
			"CommonUser",
			"CommonGame",
			"CommonUI",
			"UIExtension",
			"GameplayMessageRuntime",
			"NetCore",
			"PhysicsCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {
		});

		SetupIrisSupport(Target);
	}
}
