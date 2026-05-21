using UnrealBuildTool;

public class UE5UltimateGame : ModuleRules
{
    public UE5UltimateGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
        PublicDependencyModuleNames.AddRange(new string[] { 
            "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore",
            "EnhancedInput",
            "UMG",
            "Slate",
            "SlateCore",
            "NavigationSystem",
            "AIModule",
            "GameplayTasks",
            "OnlineSubsystem",
            "OnlineSubsystemUtils",
            "OnlineSubsystemSteam" // Для мультиплеера через Steam
        });

        PrivateDependencyModuleNames.AddRange(new string[] { 
            "Niagara", // VFX
            "Chaos",   // Физика разрушений
            "ChaosVehicles" // Транспорт (опционально)
        });
    }
}
