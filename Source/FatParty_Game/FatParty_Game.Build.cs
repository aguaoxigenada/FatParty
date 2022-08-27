// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FatParty_Game : ModuleRules
{
	public FatParty_Game(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
