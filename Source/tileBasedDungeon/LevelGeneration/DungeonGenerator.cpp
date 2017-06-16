// Fill out your copyright notice in the Description page of Project Settings.


#include "tileBasedDungeon.h"
#include "DungeonGenerator.h"

// Sets default values
ADungeonGenerator::ADungeonGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// set default level size
	TILE_SIZE_ = 100;
	stage_width_ = 51;
	stage_height_ = 101;
	stage_size_ = stage_width_ * stage_height_;

	stage_ = new EBlockType[stage_size_];

	num_room_tries_ = 10;

	// create default subobjects for instanced static meshes
	wall_ = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Wall"));
	wall_->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < GetStageSize(); i++)
	{ // fill everyslot with wall
		stage_[i] = EBlockType::EWall;
	}
	AddRooms();

	SpawnInstancedStage(); // last step
}

// Called every frame
void ADungeonGenerator::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void ADungeonGenerator::SpawnInstancedStage()
{
	for (int i = 0; i < GetStageSize(); i++)
	{
		EBlockType current_block = stage_[i];
		FVector current_location(i / GetStageWidth() * TILE_SIZE_, i % GetStageWidth() * TILE_SIZE_, TILE_SIZE_);
		switch (current_block)
		{
		case EBlockType::EFloor:
		{ // dont add a wall
			
		}
			break;
		case EBlockType::EWall:
		{ // spawn a wall mesh
			if (wall_)
				wall_->AddInstance(FTransform(FVector(current_location)));
		}
			break;
		default:
			break;
		}
	}
}

void ADungeonGenerator::AddRooms()
{
	for (int i = 0; i < num_room_tries_; i++)
	{
		// pick a random roomsize
		// avoid too narrow rooms
		// set it odd sized to line up with maze
		int size = FMath::RandRange(1, 3) * 2 + 1;
		int rectangularity = FMath::RandRange(0, 1 + size / 2) * 2;
		int width = size;
		int height = size;
		if (FMath::RandBool())
		{
			width += rectangularity;
		}
		else
		{
			height += rectangularity;
		}
		//TODO maybe switch x with y

		// set room origin point 
		int x = FMath::RandRange(1, (stage_width_ - width) / 2) * 2 + 1;
		int y = FMath::RandRange(1, (stage_height_ - height) / 2) * 2 + 1;

		Room new_room(x, y, width, height);

		bool overlaps = false;

		for (Room other : rooms_)
		{ // search for overlapping rooms
			if (new_room.DistanceToOther(other) <= 0.f) // should be the piecemeal distanceTo function //TODO TODO
			{
				overlaps = true;
				break;
			}
		}
		if (overlaps) continue; // overlapping room! -> try to place new room

		rooms_.push_back(new_room);
		//TODO increase region

		for (int pos_x = new_room.x_; pos_x < (new_room.x_ + new_room.width_); pos_x++)
		{
			for (int pos_y = new_room.y_; pos_y < (new_room.y_ + new_room.height_); pos_y++)
			{
				SetBlockAt(FVector2D(pos_x, pos_y));
			}
		}
	}
}

void ADungeonGenerator::SetBlockAt(FVector2D position, EBlockType type)
{
	int i = position.X + stage_width_ * position.Y;
	stage_[i] = type;
}

void ADungeonGenerator::BeginDestroy()
{
	delete[] stage_;
	stage_ = nullptr;
	Super::BeginDestroy();
}

int ADungeonGenerator::GetStageSize() const
{
	return stage_size_;
}

int ADungeonGenerator::GetStageWidth() const
{
	return stage_width_;
}

int ADungeonGenerator::GetStageHeight() const
{
	return stage_height_;
}

