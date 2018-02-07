// Copyright 2017-2018, Paul Buschmann, All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TILEBASEDDUNGEON_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** gets called when the lifecycle ends */
	UFUNCTION(BlueprintNativeEvent, Category = "Projectile")
	void EndLifecycle();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float speed_;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;




};
