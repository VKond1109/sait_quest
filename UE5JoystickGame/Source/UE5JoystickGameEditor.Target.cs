// Copyright 2024. All rights reserved.

using System.IO;
using UnrealBuildTool;

public class UE5JoystickGameEditorTarget : TargetRules
{
	public UE5JoystickGameEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.AddRange(new string[] { "UE5JoystickGame" });
	}
}
