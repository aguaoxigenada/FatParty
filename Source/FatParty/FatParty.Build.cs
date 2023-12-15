// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FatParty : ModuleRules
{
	public FatParty  (ReadOnlyTargetRules Target) : base(Target)
	{
        //PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "OnlineSubsystemSteam" });
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "AIModule", "GameplayTasks", "GameplayTags", "OnlineSubsystem", "OnlineSubsystemSteam" });
	}
}
