// Fill out your copyright notice in the Description page of Project Settings.

#include "tileBasedDungeon.h"
#include "Player/DungeonPlayerController.h"
#include "HUD/DungeonHUD.h"
#include "tileBasedDungeonGameMode.h"

AtileBasedDungeonGameMode::AtileBasedDungeonGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FObjectFinder<UBlueprint> PlayerBlueprint(TEXT("Blueprint'/Game/Blueprints/BluePrintCharacter.BluePrintCharacter'"));
	if (PlayerBlueprint.Object)
	{
		DefaultPawnClass = Cast<UClass>(PlayerBlueprint.Object->GeneratedClass);
	}

	PlayerControllerClass = ADungeonPlayerController::StaticClass();
	HUDClass = ADungeonHUD::StaticClass();
}

