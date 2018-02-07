// Copyright 2017-2018, Paul Buschmann, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DungeonStatics.generated.h"

/**
* This enum represents the different values an array element can be inside the level array representation
*/
UENUM()
enum class EBlockType {
	EFloor,
	EWall,
	EDoor_Closed,
	EDoor_Open
};

/**
* This enum represents the different room presets that can be requested from the manager class
* naming convention ENameOfRoom3by5SpecialTag (unordered set) room has to be rotateable when spawned
*/
UENUM()
enum class ERoomPreset {
	ESpawnRoom2by2
};
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


/*TODO Idea List
* Generation Algorithm: small rooms on stage edges bigger rooms towards the middle
* Implementation Order:
* enhanced generation algorithm
* preset for 2-3 rooms (manager class)
* player character
* inventory
* item
* weapon pickup
* 4 basic weapons
* 2 minions
* 2 bosses
*/