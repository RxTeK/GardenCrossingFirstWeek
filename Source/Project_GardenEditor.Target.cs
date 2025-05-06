// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Project_GardenEditorTarget : TargetRules
{
	public Project_GardenEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		ExtraModuleNames.AddRange( new string[] { "Project_Garden" } );
	}
}
