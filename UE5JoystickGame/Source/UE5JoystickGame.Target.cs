// Copyright 2024. All rights reserved.

using System.IO;
using UnrealBuildTool;

public class UE5JoystickGameTarget : TargetRules
{
	public UE5JoystickGameTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.AddRange(new string[] { "UE5JoystickGame" });
	}
}
