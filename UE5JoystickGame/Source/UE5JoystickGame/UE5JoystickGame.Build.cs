// Copyright 2024. All rights reserved.

using System.IO;
using UnrealBuildTool;

public class UE5JoystickGame : ModuleRules
{
        public UE5JoystickGame(ReadOnlyTargetRules Target) : base(Target)
        {
                PCHUsage = PCHUsageMode.PCHThroughSharedPublicHeader;
                PublicDependencyModuleNames.AddRange(new string[] {
                        "Core",
                        "CoreUObject",
                        "Engine",
                        "InputCore",
                        "EnhancedInput",
                        "Slate",
                        "SlateCore",
                        "UMG",
                        "OnlineSubsystem",
                        "OnlineSubsystemUtils",
                        "Networking",
                        "Sockets"
                });

                PrivateDependencyModuleNames.AddRange(new string[] {  
                        "GameplayTasks"
                });

                DynamicallyLoadedModuleNames.AddRange(new string[] { });
                
                // Поддержка плагина Online Subsystem Steam для мультиплеера
                if (Target.Platform == UnrealTargetPlatform.Win64 || Target.Platform == UnrealTargetPlatform.Linux)
                {
                        PublicDependencyModuleNames.Add("OnlineSubsystemSteam");
                }
        }
}
