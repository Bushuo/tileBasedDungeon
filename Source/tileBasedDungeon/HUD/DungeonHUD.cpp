// Fill out your copyright notice in the Description page of Project Settings.

#include "tileBasedDungeon.h"
#include "DungeonHUD.h"

ADungeonHUD::ADungeonHUD()
{
	controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//ConstructorHelpers::FObjectFinder<UMaterialInterface> material(TEXT("Material'/Game/Materials/M_CursorDecal.M_CursorDecal'"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> material(TEXT("Material'/Game/Materials/M_Cursor.M_Cursor'"));
	cursor_material = (UMaterialInterface*)material.Object;
}

void ADungeonHUD::DrawHUD()
{
	float current_mouse_position_x, current_mouse_position_y;
	int viewport_size_x, viewport_size_y;
	float offset_size_x, offset_size_y;
	float offset_location_x, offset_location_y;
	float cursor_size = 32.f;

	controller->GetMousePosition(current_mouse_position_x, current_mouse_position_y);
	controller->GetViewportSize(viewport_size_x, viewport_size_y);
	offset_size_x = (viewport_size_x - Canvas->SizeX) / 2.f;
	offset_size_y = (viewport_size_y - Canvas->SizeY) / 2.f;

	offset_location_x = current_mouse_position_x + offset_size_x;
	offset_location_y = current_mouse_position_y + offset_size_y;

	offset_location_x -= cursor_size / 2.f;
	offset_location_y -= cursor_size / 2.f;

	DrawMaterial(cursor_material, offset_location_x, offset_location_y, cursor_size, cursor_size, 0.f, 0.f, 1.f, 1.f);
}


