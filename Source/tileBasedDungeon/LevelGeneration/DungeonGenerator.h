// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include <vector>
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

	/** place to store the stage's rooms */
	std::vector<Room> rooms_;

	unsigned int stage_size_; // 0 to 65,535
	int stage_width_; // has to be odd along y-axis
	int stage_height_; // has to be odd along x-axis

	UPROPERTY(EditAnywhere, Category = leveldata)
	int num_room_tries_; // numer of times a room tries to get placed



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

	/** sets tile to type or floor
	* @param position the position to carve out
	* @param type the blocktype to set (if NULL will be set to floor)
	*/
	void SetBlockAt(FVector2D position, EBlockType type = EBlockType::EFloor);

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


};
