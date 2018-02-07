// Copyright 2017-2018, Paul Buschmann, All rights reserved.

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
