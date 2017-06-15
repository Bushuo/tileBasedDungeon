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

float Room::DistanceToOther(Room other)
{
	FVector2D start_location(x_, y_);
	FVector2D end_location(other.x_, other.y_);
	return FVector2D::Distance(start_location, end_location);
}
