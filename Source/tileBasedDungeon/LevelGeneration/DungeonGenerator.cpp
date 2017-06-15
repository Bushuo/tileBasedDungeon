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

	// create default subobjects for instanced static meshes
	wall_ = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("testmesh"));
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

	SpawnInstancedStage();
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
		FVector current_location(i / GetStageHeight() * TILE_SIZE_, i % GetStageHeight() * TILE_SIZE_, 10.f);
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

