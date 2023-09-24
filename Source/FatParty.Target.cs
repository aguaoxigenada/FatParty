// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class FatPartyTarget : TargetRules
{
	public FatPartyTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		bUsesSteam = true;
		ProjectDefinitions.Add("ONLINE_SUBSYSTEM_EOS_ENABLE_STEAM=1");
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("FatParty");
	}
}
