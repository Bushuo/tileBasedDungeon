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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* camera_;

	/** The base characters health */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	float health_;

	/** The healthvalue a character restores each second */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	float health_regeneration_;

	/** Tells wether the character is casting */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	bool casting1h_;

	/** Tells wether the character is basic attacking */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	bool basic_attacking1h_;

	//FUNCTIONS//

	/** Called on Forward/Backward input*/
	void MoveStraight(float value);

	/** Called on left/right input */
	void MoveSideways(float value);

	/** Called on basic attack input */
	virtual void OnBasicAttackPressed();
	/** Called on special attack input */
	virtual void OnSpecialAttackPressed();

	/** Called on ability1 input */
	virtual void OnAbility1Pressed();
	/** Called on ability2 input */
	virtual void OnAbility2Pressed();
	/** Called on ability3 input */
	virtual void OnAbility3Pressed();
	/** Called on ability4 input */
	virtual void OnAbility4Pressed();

private:
	FTimerHandle UnusedHandle;
	void ResetBasicAttack();
};
