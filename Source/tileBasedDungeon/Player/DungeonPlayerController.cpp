// Copyright 2017-2018, Paul Buschmann, All rights reserved.

#include "tileBasedDungeon.h"
#include "DungeonPlayerController.h"


ADungeonPlayerController::ADungeonPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::None;
}

void ADungeonPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void ADungeonPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();
}
