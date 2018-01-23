// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "DungeonPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TILEBASEDDUNGEON_API ADungeonPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADungeonPlayerController();
protected:
	
	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface
};
