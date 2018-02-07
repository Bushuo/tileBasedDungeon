// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "DungeonCharacter.generated.h"

class AProjectile;

USTRUCT(BlueprintType)
struct FDungeonCharacterInput
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InputCharacter")
	FVector2D MovementInput;

	void Sanitize();
	void MoveStraight(float axis_value);
	void MoveSideways(float axis_value);

	void BasicAttack(bool b_pressed);
	void SpecialAttack(bool b_pressed);

	//Bools

	/** Tells wether the character is casting */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	uint32 b_casting1h_ : 1;

	/** Tells wether the character is basic attacking */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	uint32 b_basic_attacking1h_ : 1;

private:
	// raw data. Game code should not see this.
	FVector2D RawMovementInput;
};


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

	/** get the yaw of the current character rotation */
	UFUNCTION(BlueprintCallable)
	const float GetCharacterYaw();

	UFUNCTION(BlueprintCallable, Category = "Character")
	const FDungeonCharacterInput& GetCurrectInput() { return character_input_; }

protected:
	/** the characters controller */
	APlayerController* controller_;

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	UCameraComponent* camera_;

	/** The base characters health */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float health_;

	/** The healthvalue a character restores each second */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	float health_regeneration_;

	/** Input structure */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	FDungeonCharacterInput character_input_;

	/** The characters base movement speed */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (ClampMin = "0.0", ClampMax = "400.0"))
	float movement_speed_;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (ClampMin = "0.0"))
	float basic_attack_cooldown_;

	int player_id_;

	//FUNCTIONS//

	/** Called on Forward/Backward input*/
	void MoveStraight(float axis_value);

	/** Called on left/right input */
	void MoveSideways(float axis_value);

	/** Called on basic attack input */
	virtual void OnBasicAttackPressed();
	virtual void OnBasicAttackReleased();
	/** Called on special attack input */
	virtual void OnSpecialAttackPressed();
	virtual void OnSpecialAttackReleased();

	/** Called on ability1 input */
	virtual void OnAbility1Pressed();
	virtual void OnAbility1Released();
	/** Called on ability2 input */
	virtual void OnAbility2Pressed();
	virtual void OnAbility2Released();
	/** Called on ability3 input */
	virtual void OnAbility3Pressed();
	virtual void OnAbility3Released();
	/** Called on ability4 input */
	virtual void OnAbility4Pressed();
	virtual void OnAbility4Released();

	// ______ Code to put in Weapon _______ //
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AProjectile> BasicAttackProjectile;

private:
	float current_yaw_;
	FTimerHandle UnusedHandle;
};


// IDEAS FOR WEAPON
/*
	#include "AProjectile"
	
	UENUM() EWeaponType
	{
		ranged,
		melee
	};

	have owning character ADungeonCharacter*
	have basic attack projectile if ranged weapon TSubclassOf<AProjectile>

*/
