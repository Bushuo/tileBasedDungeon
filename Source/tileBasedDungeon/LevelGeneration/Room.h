// Copyright 2017-2018, Paul Buschmann, All rights reserved.

#pragma once

#include "CoreMinimal.h"
/**
* for internal use in FRoom only
*/
struct FIntVector2D
{
	int32 X;
	int32 Y;
};

/**
 * This struct represents a rectangular room interior that is used to create rooms during world generation
 * The coordinate system is lefthanded when looking down x+, y+ is right and z+ is up
 * The StartPoint is defined as the lower left point of the rectangle. 
 * Size is the edge length
 * The z values of the classes interface are never used
 */
class TILEBASEDDUNGEON_API FRoom
{
public:
	FRoom(FIntVector Start, FIntVector Size);
	FRoom(FIntVector Start, int32 Size); // defines quadratic room
	FRoom(int32 StartPointX = 0, int32 StartPointY = 0, int32 SizeX = 0, int32 SizeY = 0);
	
	FIntVector GetStartPoint() const;
	FIntVector GetSize() const;
	int32 GetTop() const;
	int32 GetBottom() const;
	int32 GetLeft() const;
	int32 GetRight() const;
	
	int32 DistanceToOther(FRoom Other) const;

private:
	FIntVector2D Start;
	FIntVector2D Size;
};
