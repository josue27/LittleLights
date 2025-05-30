// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class LittleLightsTarget : TargetRules
{
	public LittleLightsTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "LittleLights" } );
		bOverrideBuildEnvironment = true;
		CppStandard = CppStandardVersion.Cpp20;

	}
}
