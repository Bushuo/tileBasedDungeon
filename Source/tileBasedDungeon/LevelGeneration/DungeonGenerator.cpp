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
	stage_length_along_y_ = 51;
	stage_length_along_x_ = 51;
	stage_size_ = stage_length_along_y_ * stage_length_along_x_;
	stage_ = new EBlockType[stage_size_];
	region_ = new int[stage_size_];

	current_region_ = -1;
	num_room_tries_ = 1;
	winding_percentage_ = .3f;

	root_ = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	RootComponent = root_;
	// create default subobjects for instanced static meshes
	wall_ = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Wall"));
	wall_->SetupAttachment(RootComponent);
	floor_ = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Floor"));
	floor_->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> material_test(TEXT("MaterialInstanceConstant'/Engine/VREditor/Tutorial/UMG/Specialized/Materials/flat_white.flat_white'"));
	floor_->SetMaterial(0, material_test.Object);
}

// Called when the game starts or when spawned
void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();

	rng = std::mt19937(rd());
	for (int i = 0; i < GetStageSize(); i++)
	{	
		// fill everyslot with wall
		stage_[i] = EBlockType::EWall;
		// set default region
		region_[i] = -1;
	}

	AddRooms();
	//DrawRegionColors();

	for (int y = 1; y < stage_length_along_y_; y += 2)
	{
		for (int x = 1; x < stage_length_along_x_; x += 2)
		{
			FVector2D pos(x, y);
			if (GetTile(pos) != EBlockType::EWall)
				continue;
			UE_LOG(LogTemp, Warning, TEXT("called GrowMaze()"));
			GrowMaze(FVector2D(pos));
		}
	}

	ConnectRegions();

	SpawnInstancedStage(); // last step
}

void ADungeonGenerator::SpawnInstancedStage()
{
	for (int i = 0; i < GetStageSize(); i++)
	{
		EBlockType current_block = stage_[i];
		FVector current_location(i / GetStageLengthY() * TILE_SIZE_, i % GetStageLengthY() * TILE_SIZE_, 0);
		switch (current_block)
		{
		case EBlockType::EFloor:
		{ // dont add a wall
			current_location.Z -= 50;
			floor_->AddInstance(FTransform(FVector(current_location)));
		}
			break;
		case EBlockType::EDoor_Closed:
		{ // dont add a wall
			current_location.Z -= 50;
			floor_->AddInstance(FTransform(FVector(current_location)));
		}
			break;
		case EBlockType::EDoor_Open:
		{ // dont add a wall
			current_location.Z -= 50;
			floor_->AddInstance(FTransform(FVector(current_location)));
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
		int length_x = size;
		int length_y = size;
		if (FMath::RandBool())
		{
			length_x += rectangularity;
		}
		else
		{
			length_y += rectangularity;
		}

		// set room origin point

		int room_x = FMath::RandRange(1, (stage_length_along_x_ - length_x) / 2) * 2 + 1;
		int room_y = FMath::RandRange(1, (stage_length_along_y_ - length_y) / 2) * 2 + 1;
		
		Room new_room(room_x, room_y, length_x, length_y);

		bool overlaps = false;

		if (((new_room.along_x_ + new_room.length_x_) >= (stage_length_along_x_ - 1)) ||
			((new_room.along_y_ + new_room.length_y_) >= (stage_length_along_y_ - 1))) // check if out of bounds
			overlaps = true;

		for (Room other : rooms_)
		{ // search for overlapping rooms
			if (new_room.DistanceToOther(other) <= 0.f)  // check if overlapping
			{
				overlaps = true;
				break;
			}
		}
		if (overlaps) 
			continue; // overlapping room! -> try to place new room

		rooms_.push_back(new_room);
		//UE_LOG(LogTemp, Warning, TEXT("Added new room (X: %d | Y: %d | length X: %d | length Y: %d)"),new_room.along_x_, new_room.along_y_, new_room.length_x_, new_room.length_y_);

		current_region_++; // each room should be a new region

		for (int pos_x = new_room.along_x_; pos_x < (new_room.along_x_ + new_room.length_x_); pos_x++)
		{
			for (int pos_y = new_room.along_y_; pos_y < (new_room.along_y_ + new_room.length_y_); pos_y++)
			{
				Carve(FVector2D(pos_x, pos_y));
			}
		}
	}
}

void ADungeonGenerator::GrowMaze(FVector2D start)
{
	TArray<FVector2D> cells;
	FVector2D last_direction;

	current_region_++;
	Carve(start);
	cells.Add(start);
	while (cells.Num() > 0)
	{
		FVector2D cell = cells.Top();
		cell = cells.Last(); 
		TArray<FVector2D> unmade_cells;

		// check all cardinal directions
		if (CanCarve(cell, FVector2D(0, 1)))
			unmade_cells.Add(FVector2D(0, 1));
		if (CanCarve(cell, FVector2D(0, -1)))
			unmade_cells.Add(FVector2D(0, -1));
		if (CanCarve(cell, FVector2D(1, 0)))
			unmade_cells.Add(FVector2D(1, 0));
		if (CanCarve(cell, FVector2D(-1, 0)))
			unmade_cells.Add(FVector2D(-1, 0));

		// carve in a random direction
		if (unmade_cells.Num() > 0)
		{
			FVector2D direction;
			std::uniform_real_distribution<float> uni_float(0.f, 1.f);
			float random_percentage = uni_float(rng);
			if (unmade_cells.Contains(last_direction) && (random_percentage > winding_percentage_))
			{
				//UE_LOG(LogTemp, Warning, TEXT("hit random percent = %f"), random_percentage);
				direction = last_direction;
			}
			else
			{
				std::uniform_int_distribution<int> uni_int(0, unmade_cells.Num() - 1);
				auto random_direction = uni_int(rng);
				direction = unmade_cells[random_direction];
				//UE_LOG(LogTemp, Warning, TEXT("random int = %d"), random_direction);
				//UE_LOG(LogTemp, Warning, TEXT("carved %d | %d"), cell.X + unmade_cells[random_direction].X, cell.Y + unmade_cells[random_direction].Y);
			}
			
			Carve(cell + direction);
			Carve(cell + direction * 2);

			cells.Add(cell + direction * 2);
			last_direction = direction;
		}
		else
		{
			cells.Pop();
			last_direction = FVector2D::ZeroVector;
		}
	}
}

void ADungeonGenerator::DrawRegionColors()
{
	TArray<FColor> colors;
	for (int i = 0; i < current_region_+1; i++)
	{
		colors.Add(FColor::MakeRandomColor());
	}
	//UE_LOG(LogTemp, Warning, TEXT(" color size is: %d"), colors.Num());
	for (int x = 0; x < stage_length_along_x_; x++)
	{
		for (int y = 0; y < stage_length_along_y_; y++)
		{

			if (GetTile(FVector2D(x, y)) == EBlockType::EFloor) 
			{
				FVector pos(x * TILE_SIZE_, y * TILE_SIZE_, 0);
				int index = region_[x * stage_length_along_y_ + y];
				FPlane plane = UKismetMathLibrary::MakePlaneFromPointAndNormal(pos, FVector(0, 0, 1));
				//UE_LOG(LogTemp, Warning, TEXT(" Draw Position is: x %d y %d\n tile is: %d\n regionidx: %d"),x,y,int(GetTile(FVector2D(x,y))),index);
				UKismetSystemLibrary::DrawDebugPlane(GetWorld(), plane, pos, 50, colors[index], 10.f);
				
			}
		}
	}
}

void ADungeonGenerator::ConnectRegions()
{
	TMap<FVector2D, TSet<int>> connector_regions;
	
	for (int x = 1; x < stage_length_along_x_ - 1; x++)
	{ // iterate over x Axis excluding the outer walls of the stage
		for (int y = 1; y < stage_length_along_y_ - 1; y++)
		{ // iterate over y Axis excluding the outer walls of the stage
			TSet<int> adjacent_regions; // adjacent region identifiers of current position
			int default_region = -1;
			FVector2D position(x,y);
			if (GetTile(position) != EBlockType::EWall) 
				continue;  // has to be a wall > which implies it is not already part of a region
			
			// check cardinal directions
			
			// UP
			int region = region_[(x + 1) * stage_length_along_y_ + y];  // region identifier of adjacent position
			if (region != default_region)
			{
				adjacent_regions.Add(region);
			}
			// DOWN
			region = region_[(x - 1) * stage_length_along_y_ + y];
			if (region != default_region)
			{
				adjacent_regions.Add(region);
			}
			// RIGHT
			region = region_[x * stage_length_along_y_ + y + 1];
			if (region != default_region)
			{
				adjacent_regions.Add(region);
			}
			// LEFT
			region = region_[x * stage_length_along_y_ + y - 1];
			if (region != default_region)
			{
				adjacent_regions.Add(region);
			}

			if (adjacent_regions.Num() < 2) // only proceed if the connector wall is adjacent to 2 or more regions
				continue;
			connector_regions.Add(position, adjacent_regions);
		}
	}

	// get list of connectors
	TArray<FVector2D> connectors;
	connector_regions.GetKeys(connectors);
	UE_LOG(LogTemp, Warning, TEXT("connectors size: %d"), connectors.Num());
	UE_LOG(LogTemp, Warning, TEXT("current region: %d"), current_region_);

	TMap<int,int> merged;  // maps the original region index to the one it has been merged to
	TSet<int> open_regions;  // regions that need to be merged including the last region
	for (int i = 0; i <= current_region_; i++)
	{ // init them
		merged.Add(i,i);
		open_regions.Add(i);
	}

	while (open_regions.Num() > 1)  // we dont need to merge the last region
	{
		std::uniform_int_distribution<int> uni_int(0, connectors.Num() - 1);
		auto random_element = uni_int(rng);
		FVector2D connector = connectors[random_element];

		CarveJunction(connector);
		// TODO FROM HERE

		TArray<int> regions;
		TArray<int> sources;
		for (auto elem : connector_regions[connector])
		{
			regions.Add(merged[elem]);
			sources.Add(merged[elem]);
		}

		int destination = regions[0];
		sources.RemoveAt(sources.Num() - 1);

		for (int i = 0; i < current_region_; i++)
		{
			if (sources.Contains(merged[i]))
			{
				merged.Add(i, destination);
			}
		}

		for (int i = 0; i < sources.Num(); i++)
		{
			open_regions.Remove(sources[i]);
		}

		// remove the merged connectors

		TArray<FVector2D> to_remove;

		for (auto positon : connectors)
		{
			// connectors shouldnt be directly beside each other
			if (FVector2D::Distance(positon, connector) < 2) 
			{
				to_remove.Add(positon);
				continue;
			}
			TSet<int> regions_set;
			for (auto region_id : connector_regions[positon])
			{
				regions_set.Add(merged[region_id]);
			}

			if (regions_set.Num() > 1)
				continue;
			// Maybe add this
			// if (rng.oneIn(extraConnectorChance)) _addJunction(pos);

			to_remove.Add(positon);
		}
		for (auto element_to_remove : to_remove)
		{
			connectors.Remove(element_to_remove);
		}
	}
}

void ADungeonGenerator::Carve(FVector2D position)
{
	SetBlockAt(position);
	SetRegionAt(position, current_region_);
}
void ADungeonGenerator::Carve(int i)
{
	SetBlockAt(i);
	SetRegionAt(i, current_region_);
}

/** returns if position + direction can be carved out */
bool ADungeonGenerator::CanCarve(FVector2D position, FVector2D direction)
{
	// times 2 because walls require a position in the maze array
	if (position.X + direction.X * 2 > stage_length_along_x_- 1 || position.X + direction.X * 2 < 1 || position.Y + direction.Y * 2 > stage_length_along_y_ - 1 || position.Y + direction.Y * 2 < 1)
	{
		return false;
	}
	return (GetTile(position + direction * 2) == EBlockType::EWall);
}

void ADungeonGenerator::CarveJunction(FVector2D position)
{
	if (OneIn(4))
	{
		SetBlockAt(position, OneIn(3) ? EBlockType::EDoor_Open : EBlockType::EFloor);
	}
	else
	{
		SetBlockAt(position, EBlockType::EDoor_Closed);
	}
}

void ADungeonGenerator::SetBlockAt(FVector2D position, EBlockType type)
{
	int i = position.X * stage_length_along_y_ + position.Y;
	stage_[i] = type;
}
void ADungeonGenerator::SetBlockAt(int i, EBlockType type)
{
	stage_[i] = type;
}

void ADungeonGenerator::SetRegionAt(FVector2D position, int region)
{
	int i = position.X * stage_length_along_y_ + position.Y;
	region_[i] = region;
}
void ADungeonGenerator::SetRegionAt(int i, int region)
{
	region_[i] = region;
}

bool ADungeonGenerator::OneIn(int x)
{
	std::uniform_int_distribution<int> uni_int(1,x);
	auto one_in_x = uni_int(rng);
	return one_in_x > 1 ? false : true;
}

void ADungeonGenerator::BeginDestroy()
{
	delete[] stage_;
	delete[] region_;
	stage_ = nullptr;
	region_ = nullptr;
	Super::BeginDestroy();
}

int ADungeonGenerator::GetStageSize() const
{
	return stage_size_;
}

int ADungeonGenerator::GetStageLengthY() const
{
	return stage_length_along_y_;
}

int ADungeonGenerator::GetStageLengthX() const
{
	return stage_length_along_x_;
}

EBlockType ADungeonGenerator::GetTile(FVector2D position)
{
	int i = position.X * stage_length_along_y_ + position.Y;
	return stage_[i];
}

