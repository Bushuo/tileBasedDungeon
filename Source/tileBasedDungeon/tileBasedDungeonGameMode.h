// Copyright 2017-2018, Paul Buschmann, All rights reserved.

#pragma once

#include "GameFramework/GameMode.h"
#include "tileBasedDungeonGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TILEBASEDDUNGEON_API AtileBasedDungeonGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	//Set default values for this gamemode
	AtileBasedDungeonGameMode(const FObjectInitializer& ObjectInitializer);
	
};
