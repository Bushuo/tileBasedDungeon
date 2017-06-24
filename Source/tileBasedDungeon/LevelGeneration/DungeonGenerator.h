// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include <vector>
#include <random>
#include "Room.h"
#include "Kismet/KismetMathLibrary.h"
#include "DungeonGenerator.generated.h"

UENUM()
enum class EBlockType {
	EFloor,
	EWall,
	EDoor_Closed,
	EDoor_Open
};

UCLASS()
class TILEBASEDDUNGEON_API ADungeonGenerator : public AActor
{
	GENERATED_BODY()
// members
//////////
private: 
	/** tile size */
	UPROPERTY(VisibleAnywhere, Category = leveldata)
	int TILE_SIZE_;

	/** pointer to store the rectangular stage array in */
	EBlockType* stage_;
	/** pointer to store the region the position in stage is part of*/
	int* region_;

	/** place to store the stage's rooms */
	std::vector<Room> rooms_;

	unsigned int stage_size_;  // 0 to 65,535
	UPROPERTY(VisibleAnywhere, Category = leveldata)
	int stage_length_along_y_; // has to be odd along y-axis
	UPROPERTY(VisibleAnywhere, Category = leveldata)
	int stage_length_along_x_; // has to be odd along x-axis

	UPROPERTY(EditAnywhere, Category = leveldata)
	int num_room_tries_;  // numer of times a room tries to get placed
	UPROPERTY(EditAnywhere, Category = leveldata, meta=(UIMin = "0.0", UIMax = "1.0", ClampMin = "0.0", ClampMax = "1.0"))
	float winding_percentage_;  // how windy the corridors of the maze should be


	int current_region_;

	std::random_device rd;
	std::mt19937 rng;

	USphereComponent* root_;

protected:
	UPROPERTY(EditDefaultsOnly, Category = leveldata)
	UInstancedStaticMeshComponent* wall_;
	UPROPERTY(EditDefaultsOnly, Category = leveldata)
	UInstancedStaticMeshComponent* floor_;

// member functions
///////////////////
private:

	/** spawn InstancedStaticMesh actors for values inside the array */
	void SpawnInstancedStage();

	/** add randomly oriented rooms to the stage array */
	void AddRooms();

	/** fill the empty spaces with a maze starting at start */
	void GrowMaze(FVector2D start);

	/** connect the regions */
	void ConnectRegions();

	/** color the different regions */
	void DrawRegionColors();

	void Carve(FVector2D position);
	void Carve(int i);

	/** returns if position + direction can be carved out */
	bool CanCarve(FVector2D position, FVector2D direction);

	/** adds a junction at specified wall position */
	void AddJunction(FVector2D position);

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

public:	
	// Sets default values for this actor's properties
	ADungeonGenerator();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** called upon destroy - overrides the AActors base functionalliy */
	void BeginDestroy() override;

	UFUNCTION(BlueprintPure, category = leveldata)
	int GetStageSize() const;
	UFUNCTION(BlueprintPure, category = leveldata)
	int GetStageLengthY() const;
	UFUNCTION(BlueprintPure, category = leveldata)
	int GetStageLengthX() const;

	UFUNCTION(BlueprintPure, category = leveldata)
	EBlockType GetTile(FVector2D position);
};
