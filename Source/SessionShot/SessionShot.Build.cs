// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class SessionShot : ModuleRules
{
	public SessionShot(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Niagara" });

        PublicIncludePaths.AddRange(new string[]
{
            "SessionShot/Public",
            "SessionShot/Public/Characters/Components",
            "SessionShot/Public/Logic",
            "SessionShot/Public/Player",
            "SessionShot/Public/Player/Input"
});

        PrivateDependencyModuleNames.AddRange(new string[] {
            "Slate",
            "SlateCore",
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks"
        });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
