// Fill out your copyright notice in the Description page of Project Settings.

#include "tileBasedDungeon.h"
#include "Room.h"

Room::Room(int along_w, int along_h, int width, int height)
{
	along_width_ = along_w;
	along_height_ = along_h;
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
	/*FVector2D start_location(along_width_, along_height_);
	FVector2D end_location(other.along_width_, other.along_height_);
	return FVector2D::Distance(start_location, end_location);*/
}

int Room::GetLeft()
{
	return FMath::Min(along_width_, along_width_ + width_);
}

int Room::GetTop()
{
	return FMath::Min(along_height_, along_height_ + height_);
}

int Room::GetRight()
{
	return FMath::Max(along_width_, along_width_ + width_);
}

int Room::GetBottom()
{
	return FMath::Max(along_height_, along_height_ + height_);
}


