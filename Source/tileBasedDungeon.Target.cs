// Copyright 2017-2018, Paul Buschmann, All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class tileBasedDungeonTarget : TargetRules
{
	//public tileBasedDungeonTarget(TargetInfo Target) changed from 4_13 to next line
	public tileBasedDungeonTarget(TargetInfo Target) : base (Target)
	{
		Type = TargetType.Game;
		
		ExtraModuleNames.Add("tileBasedDungeon");
	}
}
