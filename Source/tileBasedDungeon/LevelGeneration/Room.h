// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class TILEBASEDDUNGEON_API Room
{
public:
	
	int x_;
	int y_;

	int width_;
	int height_;

	Room(int x = 0, int y = 0, int width = 0, int height = 0);
	/** returns the minimum length a corridor based on a grid would have to be to go from this to other  
	* returns -1 if overlapping
	*/
	int DistanceToOther(Room other);

	int GetTop();
	int GetLeft();
	int GetRight();
	int GetBottom();
};
