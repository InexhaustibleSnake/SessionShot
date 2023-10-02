// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class SessionShot : ModuleRules
{
	public SessionShot(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

        PublicIncludePaths.AddRange(new string[]
{
            "SessionShot/Public",
            "SessionShot/Public/Characters",
            "SessionShot/Public/Characters/Notify",
            "SessionShot/Public/Components",
            "SessionShot/Public/Components/Attack"
});

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
