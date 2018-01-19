// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DungeonHUD.generated.h"

/**
 * 
 */
UCLASS()
class TILEBASEDDUNGEON_API ADungeonHUD : public AHUD
{
	GENERATED_BODY()
public:
	ADungeonHUD();
protected:
	APlayerController* controller;

	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, Category = Cursor)
	UMaterialInterface* cursor_material;
	
};
