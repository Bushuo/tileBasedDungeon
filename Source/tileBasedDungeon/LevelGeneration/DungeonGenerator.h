// Copyright 2017-2018, Paul Buschmann, All rights reserved.

#pragma once
#include "GameFramework/Actor.h"
#include "DungeonStatics.h"
#include <random>
#include "Room.h"
#include "DungeonGenerator.generated.h"



/**
* This class generates a level based on a 2 dimensional array
* The coordinate system is lefthanded when looking down x+, y+ is right and z+ is up
*/
UCLASS()
class TILEBASEDDUNGEON_API ADungeonGenerator : public AActor
{
	GENERATED_BODY()
/*
members
*/
private: 
	/** tile size */
	UPROPERTY(EditAnywhere, Category = "Leveldata")
	uint32 TILE_SIZE;

	/** pointer to store the rectangular stage array in */
	EBlockType* Stage;
	/** pointer to store the region the position in stage is part of*/
	int32* Region;

	/** place to store the stage's rooms */
	TArray<FRoom> Rooms;

	uint32 StageSize;  // 0 to 65,535
	UPROPERTY(VisibleAnywhere, Category = "Leveldata")
	int32 StageLengthAlongY; // has to be odd along y-axis
	UPROPERTY(VisibleAnywhere, Category = "Leveldata")
	int32 StageLengthAlongX; // has to be odd along x-axis

	UPROPERTY(EditAnywhere, Category = "Leveldata")
	int32 TryPlaceRoom;  // how many times a room tries to get placed
	UPROPERTY(EditAnywhere, Category = "Leveldata", meta=(UIMin = "0.0", UIMax = "1.0", ClampMin = "0.0", ClampMax = "1.0"))
	float WindingPercentage;  // how windy the corridors of the maze should be


	int32 CurrentRegion;
	int32 DefaultRegion = -1;

	std::random_device RandomDevice;
	std::mt19937 RandomNumberGenerator;

	UPROPERTY(VisibleAnywhere, Category = "Leveldata")
	USceneComponent* Root;
	UPROPERTY(EditDefaultsOnly, Category = "Leveldata")
	UInstancedStaticMeshComponent* Wall;
	UPROPERTY(EditDefaultsOnly, Category = "Leveldata")
	UInstancedStaticMeshComponent* Floor;

/*
member functions
*/

	/** spawn InstancedStaticMesh actors for values inside the array */
	void SpawnInstancedStage();

	/** add randomly oriented rooms to the stage array */
	void AddRooms();

	/** fill the empty spaces with a maze starting at start */
	void GrowMaze(FVector2D start);

	/** connect the regions */
	void ConnectRegions();

	/** removes the dead ends from the labyrinth corridors */
	void RemoveDeadEnds();

	/** color the different regions */
	void DrawRegionColors();

	void Carve(FVector2D position);
	void Carve(int i);

	/** returns if position + direction can be carved out */
	bool CanCarve(FVector2D position, FVector2D direction);

	/** adds a junction at specified wall position */
	void CarveJunction(FVector2D position);

	/** sets tile to type or floor
	* @param position the position to carve out
	* @param type the blocktype to set (if NULL will be set to floor)
	*/
	void SetBlockAt(FVector2D position, EBlockType type = EBlockType::EFloor);
	void SetBlockAt(int i, EBlockType type = EBlockType::EFloor);

	/** sets the region of input position to input int
	* @param position the position to set the region
	* @param region the region to set to
	*/
	void SetRegionAt(FVector2D position, int region);
	void SetRegionAt(int i, int region);

	/** returns true if one in X was hit
	* false otherwise
	* example 1 in 6 for a regular dice
	*/
	bool OneIn(int x);

	/** initializes the stage to its most basic form: plain walls */
	void InitializeStage();

	/** gives back a candidate room based on specified generation parameters */
	FRoom GetCandidateRoom();

	bool IsRoomOverlapping(const FRoom& Room) const;

	void CarveRoom(const FRoom & Room);

	void AddPossibleCellsInCardinalDirections(TArray<FVector2D>& OutUncarvedCells, const FVector2D& StartingCell);

	FVector2D GetRandomCarveDirection(const TArray<FVector2D>& UncarvedCells, const FVector2D& LastDirection);

	void AddRegionsInCardinalDirections(TSet<int>& AdjecentRegions, int CurrentRegion, int PosX, int PosY);

public:	
	// Sets default values for this actor's properties
	ADungeonGenerator();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** called upon destroy - overrides the AActors base functionalliy */
	void BeginDestroy() override;

	UFUNCTION(BlueprintPure, category = "Leveldata")
	int GetStageSize() const;
	UFUNCTION(BlueprintPure, category = "Leveldata")
	int GetStageLengthY() const;
	UFUNCTION(BlueprintPure, category = "Leveldata")
	int GetStageLengthX() const;

	UFUNCTION(BlueprintPure, category = "Leveldata")
	EBlockType GetTile(FVector2D position);
};
