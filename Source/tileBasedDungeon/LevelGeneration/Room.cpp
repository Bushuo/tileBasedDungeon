// Fill out your copyright notice in the Description page of Project Settings.

#include "tileBasedDungeon.h"
#include "Room.h"

Room::Room(int x, int y, int length_x, int length_y)
{
	along_x_ = x;
	along_y_ = y;
	length_x_ = length_y;
	length_y_ = length_y;
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
}

int Room::GetLeft()
{
	return FMath::Min(along_y_, along_y_ + length_y_);
}

int Room::GetTop()
{
	return FMath::Min(along_x_, along_x_ + length_x_);
}

int Room::GetRight()
{
	return FMath::Max(along_y_, along_y_ + length_y_);
}

int Room::GetBottom()
{
	return FMath::Max(along_x_, along_x_ + length_x_);
}


