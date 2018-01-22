// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "DungeonCharacter.generated.h"

UCLASS()
class TILEBASEDDUNGEON_API ADungeonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADungeonCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

protected:
	/** the characters controller */
	APlayerController* controller_;

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, Category = camera)
	UCameraComponent* camera_;
	
	/** Camera boom to position cam above character */
	UPROPERTY(VisibleAnywhere, Category = camera)
	USpringArmComponent* camera_boom_;

	/** The base characters health */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float health_;

	/** Tells wether the character is casting */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool casting1h_;

	/** Called on Forward/Backward input*/
	void MoveStraight(float value);

	/** Called on left right input */
	void MoveSideways(float value);

	virtual void OnClickToInteractPressed();
	virtual void OnClickToInteractReleased();

private:
	//FVector TargetLocation;

	
};
