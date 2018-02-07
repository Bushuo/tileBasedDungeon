// Copyright 2017-2018, Paul Buschmann, All rights reserved.

#include "tileBasedDungeon.h"
#include "Room.h"
FRoom::FRoom(FIntVector Start, FIntVector Size)
{
	this->Start.X = Start.X;
	this->Start.Y = Start.Y;
	this->Size.X = Size.X;
	this->Size.Y = Size.Y;
	RoomLayout = new EBlockType[Size.X * Size.Y];
}

FRoom::FRoom(FIntVector Start, int32 Size) : FRoom(Start, FIntVector(Size))
{
}

FRoom::FRoom(int32 StartPointX, int32 StartPointY, int32 SizeX, int32 SizeY) : FRoom(FIntVector(StartPointX, StartPointY, 0), FIntVector(SizeX, SizeY, 0))
{
}

FIntVector FRoom::GetStartPoint() const { return FIntVector(Start.X, Start.Y, 0); }
FIntVector FRoom::GetSize() const { return FIntVector(Size.X, Size.Y, 0); }
int32 FRoom::GetTop() const { return FMath::Min(Start.X, Start.X + Size.X); }
int32 FRoom::GetBottom() const { return FMath::Max(Start.X, Start.X + Size.X); }
int32 FRoom::GetLeft() const { return FMath::Min(Start.Y, Start.Y + Size.Y); }
int32 FRoom::GetRight() const { return FMath::Max(Start.Y, Start.Y + Size.Y); }

/** returns the minimum length a corridor based on a grid would have to be to go from this to other
* returns -1 if overlapping
*/
int32 FRoom::DistanceToOther(const FRoom& Other) const 
{ 
	int32 VerticalDistance = CalculateVerticalDistance(Other);
	int32 HorizontalDistance = CalculateHorizontalDistance(Other);

	if ((VerticalDistance == -1) && (HorizontalDistance == -1)) { 
		return -1;
	}
	if (VerticalDistance == -1) {
		return HorizontalDistance;
	}
	if (HorizontalDistance == -1) {
			return VerticalDistance;
	}
	return HorizontalDistance + VerticalDistance;
}

void FRoom::Initialize()
{
	//TODO call manager method to fill the layout array with preset for room interior
}

// returns -1 if overlaping vertically with the other room
int32 FRoom::CalculateVerticalDistance(const FRoom& Other) const
{
	int32 VerticalDistance = 0;
	if (GetTop() >= Other.GetBottom()) {
		VerticalDistance = GetTop() - Other.GetBottom();
	}
	else if (GetBottom() <= Other.GetTop()) {
		VerticalDistance = Other.GetTop() - GetBottom();
	}
	else {
		VerticalDistance = -1;
	}
	return VerticalDistance;
}

// returns -1 if overlaping horizontaly with the other room
int32 FRoom::CalculateHorizontalDistance(const FRoom & Other) const
{
	int32 HorizontalDistance = 0;
	if (GetLeft() >= Other.GetRight()) {
		HorizontalDistance = GetLeft() - Other.GetRight();
	}
	else if (GetRight() <= Other.GetLeft()) {
		HorizontalDistance = Other.GetLeft() - GetRight();
	}
	else {
		HorizontalDistance = -1;
	}
	return HorizontalDistance;
}

