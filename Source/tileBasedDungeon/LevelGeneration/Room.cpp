// Fill out your copyright notice in the Description page of Project Settings.

#include "tileBasedDungeon.h"
#include "Room.h"

Room::Room(int x, int y, int width, int height)
{
	x_ = x;
	y_ = y;
	width_ = width;
	height_ = height;
}

int Room::DistanceToOther(Room other)
{
	int vertical;
	if (GetTop() >= other.GetBottom())
	{
		vertical = GetTop() - other.GetBottom();
	}
	else if (GetBottom() <= other.GetTop())
	{
		vertical = other.GetTop() - GetBottom();
	}
	else
	{
		vertical = -1;
	}

	int horizontal;
	if (GetLeft() >= other.GetRight()) {
		horizontal = GetLeft() - other.GetRight();
	}
	else if (GetRight() <= other.GetLeft()) {
		horizontal = other.GetLeft() - GetRight();
	}
	else {
		horizontal = -1;
	}

	if ((vertical == -1) && (horizontal == -1)) return -1;
	if (vertical == -1) return horizontal;
	if (horizontal == -1) return vertical;
	return horizontal + vertical;

	//OLD IMPLEMENTATION : Simple vector distance would return a float
	/*FVector2D start_location(x_, y_);
	FVector2D end_location(other.x_, other.y_);
	return FVector2D::Distance(start_location, end_location);*/
}

int Room::GetLeft()
{
	return FMath::Min(x_, x_ + width_);
}

int Room::GetTop()
{
	return FMath::Min(y_, y_ + height_);
}


int Room::GetRight()
{
	return FMath::Max(x_, x_ + width_);
}

int Room::GetBottom()
{
	return FMath::Max(y_, y_ + height_);
}


