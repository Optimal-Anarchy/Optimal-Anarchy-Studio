// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Biomi_IOS_00_00_01Target : TargetRules
{
	public Biomi_IOS_00_00_01Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "Biomi_IOS_00_00_01" } );
	}
}
