// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class LittleLightsEditorTarget : TargetRules
{
	public LittleLightsEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "LittleLights" } );
		bOverrideBuildEnvironment = true;
		CppStandard = CppStandardVersion.Cpp20;
		// 🔧 Esta línea es clave:
		 //BuildEnvironment = TargetBuildEnvironment.Unique;

		// Opciones personalizadas (ya no causarán conflicto):
		//bStrictConformanceMode = false;
	}
}
