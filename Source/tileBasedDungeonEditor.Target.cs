// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class tileBasedDungeonEditorTarget : TargetRules
{
	// public tileBasedDungeonEditorTarget(TargetInfo Target) changed from 4_13 to next line
	public tileBasedDungeonEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("tileBasedDungeon");
	}

	//
	// TargetRules interface.
	//

	// deprecated with 4_18
	//public override void SetupBinaries(
	//	TargetInfo Target,
	//	ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
	//	ref List<string> OutExtraModuleNames
	//	)
	//{
	//	OutExtraModuleNames.AddRange( new string[] { "tileBasedDungeon" } );
	//}
}
