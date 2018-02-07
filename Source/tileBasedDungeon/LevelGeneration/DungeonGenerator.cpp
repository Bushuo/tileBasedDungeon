// Copyright 2017-2018, Paul Buschmann, All rights reserved.

#include "tileBasedDungeon.h"
#include "DungeonGenerator.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADungeonGenerator::ADungeonGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// set default level size
	TILE_SIZE = 100;
	StageLengthAlongY = 51;
	StageLengthAlongX = 51;

	CurrentRegion = -1;
	TryPlaceRoom = 1;
	WindingPercentage = .3f;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	RootComponent->SetMobility(EComponentMobility::Static);

	// create default subobjects for instanced static meshes
	Wall = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Wall"));
	Wall->SetupAttachment(RootComponent);
	Floor = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Floor"));
	Floor->SetupAttachment(RootComponent);
	//static ConstructorHelpers::FObjectFinder<UMaterialInterface> material_test(TEXT("MaterialInstanceConstant'/Engine/VREditor/Tutorial/UMG/Specialized/Materials/flat_white.flat_white'"));
	//Floor->SetMaterial(0, material_test.Object);
}

// Called when the game starts or when spawned
void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();
	InitializeStage();

	AddRooms();
	//DrawRegionColors();

	for (int32 y = 1; y < StageLengthAlongY; y += 2)
	{
		for (int32 x = 1; x < StageLengthAlongX; x += 2)
		{
			FVector2D pos(x, y);
			if (GetTile(pos) != EBlockType::EWall)
				continue;
			UE_LOG(LogTemp, Warning, TEXT("called GrowMaze()"));
			GrowMaze(FVector2D(pos));
		}
	}

	ConnectRegions();
	RemoveDeadEnds();
	SpawnInstancedStage(); // last step
}

void ADungeonGenerator::InitializeStage()
{
	/// check if stage length is odd on both axis
	/// if not make odd
	if (StageLengthAlongX % 2 == 0) {
		StageLengthAlongX++;
	}
	if (StageLengthAlongY % 2 == 0) {
		StageLengthAlongY++;
	}
	/// calculate stage size an setup containers
	StageSize = StageLengthAlongY * StageLengthAlongX;
	Stage = new EBlockType[StageSize];
	Region = new int32[StageSize];
	RandomNumberGenerator = std::mt19937(RandomDevice());

	/// fillup the stage and set default region
	for (int i = 0; i < GetStageSize(); i++)
	{
		Stage[i] = EBlockType::EWall;
		Region[i] = -1;
	}
}

void ADungeonGenerator::AddRooms()
{
	for (int i = 0; i < TryPlaceRoom; i++)
	{
		FRoom RoomToAdd = GetCandidateRoom();

		bool bIsOverlapping = IsRoomOverlapping(RoomToAdd);
		if (bIsOverlapping) {
			continue; // overlapping room! -> try to place new room
		}

		Rooms.Push(RoomToAdd);
		CurrentRegion++; // each room should be a new region

		CarveRoom(RoomToAdd);
	}
}

FRoom ADungeonGenerator::GetCandidateRoom()
{
	/// pick a random roomsize
	/// set it odd sized to line up with maze
	int RoomSize = FMath::RandRange(1, 3) * 2 + 1;
	int Rectangularity = FMath::RandRange(0, 1 + RoomSize / 2) * 2;
	int RoomSizeX = RoomSize;
	int RoomSizeY = RoomSize;

	/// avoid too narrow rooms. make the room more rectangular
	if (FMath::RandBool()) {
		RoomSizeX += Rectangularity;
	}
	else {
		RoomSizeY += Rectangularity;
	}

	/// set room origin point TODO something is odd here (probably because x is up and y is right)
	int RoomStartPointX = FMath::RandRange(1, (StageLengthAlongX - RoomSizeX) / 2) * 2 + 1;
	int RoomStartPointY = FMath::RandRange(1, (StageLengthAlongY - RoomSizeY) / 2) * 2 + 1;

	return FRoom(RoomStartPointX, RoomStartPointY, RoomSizeX, RoomSizeY);
}

bool ADungeonGenerator::IsRoomOverlapping(const FRoom& Room) const
{
	bool bOverlapping = false;
	if (((Room.GetStartPoint().X + Room.GetSize().X) >= (StageLengthAlongX - 1)) ||
		((Room.GetStartPoint().Y + Room.GetSize().Y) >= (StageLengthAlongY - 1))) { // check if out of bounds
		bOverlapping = true; //TODO maybe just make room smaller
	}
	else {
		for (FRoom Other : Rooms) { // search for overlapping rooms
			if (Room.DistanceToOther(Other) <= 0.f) {  // check if overlapping
				bOverlapping = true;
				break;
			}
		}
	}
	return bOverlapping;
}

// takes an odd wall tile as start position to grow a maze from it
void ADungeonGenerator::GrowMaze(FVector2D StartPosition)
{
	TArray<FVector2D> MazeCells;
	FVector2D LastDirection;

	CurrentRegion++; // make the maze a new region
	Carve(StartPosition);
	MazeCells.Add(StartPosition);
	while (MazeCells.Num() > 0)
	{
		FVector2D Cell = MazeCells.Top();
		TArray<FVector2D> UncarvedCells;
		AddPossibleCellsInCardinalDirections(UncarvedCells, Cell);

		// if cell(s) got added get back a random one and carve
		if (UncarvedCells.Num() > 0)
		{
			FVector2D DirectionToCarve = GetRandomCarveDirection(UncarvedCells, LastDirection);
			
			Carve(Cell + DirectionToCarve);
			Carve(Cell + DirectionToCarve * 2);

			MazeCells.Add(Cell + DirectionToCarve * 2);
			LastDirection = DirectionToCarve;
		}
		else
		{
			MazeCells.Pop();
			LastDirection = FVector2D::ZeroVector;
		}
	}
}

// checks all cardinal directions and adds the tiles to array if possbile
void ADungeonGenerator::AddPossibleCellsInCardinalDirections(TArray<FVector2D>& OutUncarvedCells, const FVector2D& StartingCell)
{
	// check all cardinal directions for possible path
	if (CanCarve(StartingCell, FVector2D(0, 1))) {
		OutUncarvedCells.Add(FVector2D(0, 1));
	}
	if (CanCarve(StartingCell, FVector2D(0, -1))) {
		OutUncarvedCells.Add(FVector2D(0, -1));
	}
	if (CanCarve(StartingCell, FVector2D(1, 0))) {
		OutUncarvedCells.Add(FVector2D(1, 0));
	}
	if (CanCarve(StartingCell, FVector2D(-1, 0))) {
		OutUncarvedCells.Add(FVector2D(-1, 0));
	}
}

// expects a non empty TArray to get the direction from
FVector2D ADungeonGenerator::GetRandomCarveDirection(const TArray<FVector2D>& UncarvedCells,const FVector2D& LastDirection)
{
	FVector2D DirectionToCarve = FVector2D::ZeroVector;
	
	/// get float percentage
	std::uniform_real_distribution<float> UniformFloatRange(0.f, 1.f);
	float RandomPercentage = UniformFloatRange(RandomNumberGenerator); 
	// if the chance is right and the last direction can be carved do it
	if (UncarvedCells.Contains(LastDirection) && (RandomPercentage > WindingPercentage)) { 
		return DirectionToCarve = LastDirection;
	}
	else { // otherwise get a random direction to carve to
		int NumberOfPossibleDirections = UncarvedCells.Num() - 1;
		std::uniform_int_distribution<int> UniformIntRange(0, NumberOfPossibleDirections);
		int RandomDirection = UniformIntRange(RandomNumberGenerator);
		return DirectionToCarve = UncarvedCells[RandomDirection];
	}
}

void ADungeonGenerator::DrawRegionColors()
{
	TArray<FColor> colors;
	for (int i = 0; i < CurrentRegion+1; i++)
	{
		colors.Add(FColor::MakeRandomColor());
	}
	//UE_LOG(LogTemp, Warning, TEXT(" color size is: %d"), colors.Num());
	for (int x = 0; x < StageLengthAlongX; x++)
	{
		for (int y = 0; y < StageLengthAlongY; y++)
		{

			if (GetTile(FVector2D(x, y)) == EBlockType::EFloor) 
			{
				FVector pos(x * TILE_SIZE, y * TILE_SIZE, 0);
				int index = Region[x * StageLengthAlongY + y];
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
	
	for (int x = 1; x < StageLengthAlongX - 1; x++)
	{ // iterate over x Axis excluding the outer walls of the stage
		for (int y = 1; y < StageLengthAlongY - 1; y++)
		{ // iterate over y Axis excluding the outer walls of the stage
			TSet<int> adjacent_regions; // adjacent region identifiers of current position
			int default_region = -1;
			FVector2D position(x,y);
			if (GetTile(position) != EBlockType::EWall) 
				continue;  // has to be a wall > which implies it is not already part of a region
			
			// check cardinal directions
			
			// UP
			int region = Region[(x + 1) * StageLengthAlongY + y];  // region identifier of adjacent position
			if (region != default_region) {
				adjacent_regions.Add(region);
			}
			// DOWN
			region = Region[(x - 1) * StageLengthAlongY + y];
			if (region != default_region) {
				adjacent_regions.Add(region);
			}
			// RIGHT
			region = Region[x * StageLengthAlongY + y + 1];
			if (region != default_region) {
				adjacent_regions.Add(region);
			}
			// LEFT
			region = Region[x * StageLengthAlongY + y - 1];
			if (region != default_region) {
				adjacent_regions.Add(region);
			}

			if (adjacent_regions.Num() < 2) { // only proceed if the connector wall is adjacent to 2 or more regions
				continue;
			}
			connector_regions.Add(position, adjacent_regions);
		}
	}

	// get list of connectors
	TArray<FVector2D> connectors;
	connector_regions.GetKeys(connectors);
	UE_LOG(LogTemp, Warning, TEXT("connectors size: %d"), connectors.Num());
	UE_LOG(LogTemp, Warning, TEXT("current region: %d"), CurrentRegion);

	TMap<int,int> merged;  // maps the original region index to the one it has been merged to
	TSet<int> open_regions;  // regions that need to be merged including the last region
	for (int i = 0; i <= CurrentRegion; i++)
	{ // init them
		merged.Add(i,i);
		open_regions.Add(i);
	}

	while (open_regions.Num() > 1)  // we dont need to merge the last region
	{
		std::uniform_int_distribution<int> uni_int(0, connectors.Num() - 1);
		auto random_element = uni_int(RandomNumberGenerator);
		FVector2D connector = connectors[random_element];

		CarveJunction(connector);

		TArray<int> regions;
		TArray<int> sources;
		for (auto elem : connector_regions[connector])
		{
			regions.Add(merged[elem]);
			sources.Add(merged[elem]);
		}

		int destination = regions[0];
		sources.RemoveAt(sources.Num() - 1);

		for (int i = 0; i < CurrentRegion; i++)
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
			// if (RandomNumberGenerator.oneIn(extraConnectorChance)) _addJunction(pos);

			to_remove.Add(positon);
		}
		for (auto element_to_remove : to_remove)
		{
			connectors.Remove(element_to_remove);
		}
	}
}

void ADungeonGenerator::RemoveDeadEnds()
{
	bool b_done = false;
	while (!b_done)
	{
		b_done = true;
		for (int x = 1; x < StageLengthAlongX - 1; x++)
		{ // iterate over x Axis excluding the outer walls of the stage
			for (int y = 1; y < StageLengthAlongY - 1; y++)
			{ // iterate over y Axis excluding the outer walls of the stage
				if (GetTile(FVector2D(x, y)) == EBlockType::EWall) {
					continue;
				}
				// if it only has one exit, it is a dead end
				int exits = 0;
				if (GetTile(FVector2D(x + 1, y)) != EBlockType::EWall) {
					exits++;
				}
				if (GetTile(FVector2D(x - 1, y)) != EBlockType::EWall) {
					exits++;
				}
				if (GetTile(FVector2D(x, y + 1)) != EBlockType::EWall) {
					exits++;
				}
				if (GetTile(FVector2D(x, y - 1)) != EBlockType::EWall) {
					exits++;
				}

				if (exits != 1) {
					continue;
				}
				b_done = false;
				SetBlockAt(FVector2D(x, y), EBlockType::EWall);
			}
		}
	}
}

void ADungeonGenerator::SpawnInstancedStage()
{
	for (int i = 0; i < GetStageSize(); i++)
	{
		EBlockType current_block = Stage[i];
		FVector current_location(i / GetStageLengthY() * TILE_SIZE, i % GetStageLengthY() * TILE_SIZE, 0);
		switch (current_block)
		{
		case EBlockType::EFloor:
		{ // dont add a wall
			current_location.Z -= 50;
			Floor->AddInstance(FTransform(FVector(current_location)));
		}
		break;
		case EBlockType::EDoor_Closed:
		{ // dont add a wall
			current_location.Z -= 50;
			Floor->AddInstance(FTransform(FVector(current_location)));
		}
		break;
		case EBlockType::EDoor_Open:
		{ // dont add a wall
			current_location.Z -= 50;
			Floor->AddInstance(FTransform(FVector(current_location)));
		}
		break;
		case EBlockType::EWall:
		{ // spawn a wall mesh
			if (Wall)
				Wall->AddInstance(FTransform(FVector(current_location)));
		}
		break;
		default:
			break;
		}
	}
}

void ADungeonGenerator::Carve(FVector2D position)
{
	SetBlockAt(position);
	SetRegionAt(position, CurrentRegion);
}
void ADungeonGenerator::Carve(int i)
{
	SetBlockAt(i);
	SetRegionAt(i, CurrentRegion);
}

/** returns if position + direction can be carved out */
bool ADungeonGenerator::CanCarve(FVector2D position, FVector2D direction)
{
	// times 2 because walls require a position in the maze array
	if (position.X + direction.X * 2 > StageLengthAlongX- 1 || position.X + direction.X * 2 < 1 || position.Y + direction.Y * 2 > StageLengthAlongY - 1 || position.Y + direction.Y * 2 < 1) {
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

void ADungeonGenerator::CarveRoom(const FRoom& Room)
{
	for (int CarvePosX = Room.GetStartPoint().X; CarvePosX < (Room.GetStartPoint().X + Room.GetSize().X); CarvePosX++) {
		for (int CarvePosY = Room.GetStartPoint().Y; CarvePosY < (Room.GetStartPoint().Y + Room.GetSize().Y); CarvePosY++) {
			Carve(FVector2D(CarvePosX, CarvePosY));
		}
	}
}



void ADungeonGenerator::SetBlockAt(FVector2D position, EBlockType type)
{
	int i = position.X * StageLengthAlongY + position.Y;
	Stage[i] = type;
}
void ADungeonGenerator::SetBlockAt(int i, EBlockType type)
{
	Stage[i] = type;
}

void ADungeonGenerator::SetRegionAt(FVector2D position, int region)
{
	int i = position.X * StageLengthAlongY + position.Y;
	Region[i] = region;
}
void ADungeonGenerator::SetRegionAt(int i, int region)
{
	Region[i] = region;
}

bool ADungeonGenerator::OneIn(int x)
{
	std::uniform_int_distribution<int> uni_int(1,x);
	auto one_in_x = uni_int(RandomNumberGenerator);
	return one_in_x > 1 ? false : true;
}

void ADungeonGenerator::BeginDestroy()
{
	delete[] Stage;
	delete[] Region;
	Stage = nullptr;
	Region = nullptr;
	Super::BeginDestroy();
}

int ADungeonGenerator::GetStageSize() const
{
	return StageSize;
}

int ADungeonGenerator::GetStageLengthY() const
{
	return StageLengthAlongY;
}

int ADungeonGenerator::GetStageLengthX() const
{
	return StageLengthAlongX;
}

EBlockType ADungeonGenerator::GetTile(FVector2D position)
{
	int i = position.X * StageLengthAlongY + position.Y;
	return Stage[i];
}

