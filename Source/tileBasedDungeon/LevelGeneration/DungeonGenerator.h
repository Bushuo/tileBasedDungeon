// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include <vector>
#include <random>
#include "Room.h"
#include "DungeonGenerator.generated.h"

UENUM()
enum class EBlockType {
	EFloor,
	EWall
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

	// 0 to 65,535
	unsigned int stage_size_; 
	UPROPERTY(VisibleAnywhere, Category = leveldata)
	// has to be odd along y-axis
	int stage_width_;
	UPROPERTY(VisibleAnywhere, Category = leveldata)
	// has to be odd along x-axis
	int stage_height_; 

	UPROPERTY(EditAnywhere, Category = leveldata)
	int num_room_tries_; // numer of times a room tries to get placed

	int current_region_;

	std::random_device rd;
	std::mt19937 rng;

protected:
	UPROPERTY(EditDefaultsOnly, Category = leveldata)
	UInstancedStaticMeshComponent* wall_;

// member functions
///////////////////
private:

	/** spawn InstancedStaticMesh actors for values inside the array */
	void SpawnInstancedStage();

	/** add randomly oriented rooms to the stage array */
	void AddRooms();

	/** fill the empty spaces with a maze starting at start */
	void GrowMaze(FVector2D start);

	void Carve(FVector2D position);

	/** returns if position + direction can be carved out */
	bool CanCarve(FVector2D position, FVector2D direction);

	/** sets tile to type or floor
	* @param position the position to carve out
	* @param type the blocktype to set (if NULL will be set to floor)
	*/
	void SetBlockAt(FVector2D position, EBlockType type = EBlockType::EFloor);

	/** sets the region of input position to input int
	* @param position the position to set the region
	* @param region the region to set to
	*/
	void SetRegionAt(FVector2D position, int region);

public:	
	// Sets default values for this actor's properties
	ADungeonGenerator();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/** called upon destroy - overrides the AActors base functionalliy */
	void BeginDestroy() override;

	UFUNCTION(BlueprintPure, category = leveldata)
	int GetStageSize() const;
	UFUNCTION(BlueprintPure, category = leveldata)
	int GetStageWidth() const;
	UFUNCTION(BlueprintPure, category = leveldata)
	int GetStageHeight() const;

	UFUNCTION(BlueprintPure, category = leveldata)
	EBlockType GetTile(FVector2D position);
};
