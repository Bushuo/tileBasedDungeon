// Copyright 2017-2018, Paul Buschmann, All rights reserved.

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
}
