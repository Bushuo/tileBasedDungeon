// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DungeonStatics.generated.h"

/**
 * 
 */
UCLASS()
class TILEBASEDDUNGEON_API UDungeonStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/** calculate angle difference in the range [-180, 180] */
	UFUNCTION(BlueprintCallable, Category = "Math")
	static float FindDeltaAngleDegrees(float A1, float A2);

	/** finds the angle between two 2D points */
	UFUNCTION(BlueprintPure, Category = "Math|Rotator", meta = (Keywords = "rotation rotate"))
	static bool FindLookAtAngle2D(const FVector2D& Start, const FVector2D& Target, float& Angle);
};
