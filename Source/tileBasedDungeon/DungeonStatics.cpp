// Copyright 2017-2018, Paul Buschmann, All rights reserved.

#include "tileBasedDungeon.h"
#include "DungeonStatics.h"


float UDungeonStatics::FindDeltaAngleDegrees(float A1, float A2)
{
	//Find the difference
	float delta = A2 - A1;
	if (delta > 180.f)
	{
		delta -= 360.f;
	}
	else if (delta < -180.f)
	{
		delta += 360.f;
	}
	return delta;
}

bool UDungeonStatics::FindLookAtAngle2D(const FVector2D& start, const FVector2D& target, float& angle)
{
	FVector2D normal = (target - start).GetSafeNormal();
	if (!normal.IsNearlyZero())
	{
		angle = FMath::RadiansToDegrees(FMath::Atan2(normal.Y, normal.X));
		return true;
	}
	return false;
}

