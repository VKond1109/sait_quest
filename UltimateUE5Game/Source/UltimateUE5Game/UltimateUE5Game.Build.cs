// Copyright 2024 UltimateUE5Game. All Rights Reserved.

using UnrealBuildTool;

public class UltimateUE5Game : ModuleRules
{
	public UltimateUE5Game(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.PCHOrSharedThroughUnity;
		
		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",
			"UMG", 
			"Slate", 
			"SlateCore",
			"Niagara",
			"Chaos",
			"ChaosVehicles",
			"PhysicsCore",
			"AIModule",
			"GameplayTasks",
			"NavigationSystem",
			"OnlineSubsystem",
			"OnlineSubsystemUtils",
			"HTTP",
			"Json",
			"JsonUtilities"
		});

		PrivateDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore" 
		});

		if (Target.bBuildEditor)
		{
			PublicDependencyModuleNames.AddRange(new string[] { "UnrealEd", "Blutility" });
		}
	}
}
