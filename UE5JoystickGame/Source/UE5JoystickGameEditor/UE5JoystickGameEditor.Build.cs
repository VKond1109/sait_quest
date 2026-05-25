// Copyright 2024. All rights reserved.

using System.IO;
using UnrealBuildTool;

public class UE5JoystickGameEditor : ModuleRules
{
	public UE5JoystickGameEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.PCHThroughSharedPublicHeader;
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine",
			"UnrealEd"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		DynamicallyLoadedModuleNames.AddRange(new string[] { });
	}
}
