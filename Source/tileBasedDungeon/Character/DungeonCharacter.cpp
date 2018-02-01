// Fill out your copyright notice in the Description page of Project Settings.

#include "tileBasedDungeon.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "DungeonCharacter.h"

// insert in weapon
#include "Projectiles/Projectile.h"

void FDungeonCharacterInput::Sanitize()
{
	MovementInput = RawMovementInput.ClampAxes(-1.f, 1.f);
	MovementInput = MovementInput.GetSafeNormal();

	RawMovementInput.Set(0.f, 0.f);
}

void FDungeonCharacterInput::MoveStraight(float axis_value)
{
	RawMovementInput.X += axis_value;
}

void FDungeonCharacterInput::MoveSideways(float axis_value)
{
	RawMovementInput.Y += axis_value;
}

void FDungeonCharacterInput::BasicAttack(bool b_pressed)
{
	b_basic_attacking1h_ = b_pressed;
}

void FDungeonCharacterInput::SpecialAttack(bool b_pressed)
{
}

// Sets default values
ADungeonCharacter::ADungeonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CharacterBase"));
	}

	player_id_ = 0;
	controller_ = UGameplayStatics::GetPlayerController(GetWorld(), player_id_);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create camera_boom_ to Attach the Camera to
	USpringArmComponent* camera_boom_ = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	camera_boom_->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	camera_boom_->bUsePawnControlRotation = false;
	camera_boom_->bEnableCameraRotationLag = false;
	camera_boom_->TargetArmLength = 700.f;
	camera_boom_->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
	camera_boom_->SetupAttachment(RootComponent);
	camera_boom_->SetWorldRotation(FRotator(-40.f, 0.f, 0.f));

	// Attach Camera to camera_boom_
	camera_ = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	
	camera_->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	camera_->SetupAttachment(camera_boom_, USpringArmComponent::SocketName);

	health_ = .4f;
	health_regeneration_ = .01f;

	movement_speed_ = 400.f;
	this->GetCharacterMovement()->MaxWalkSpeed = movement_speed_;

	basic_attack_cooldown_ = 2.f;
}

// Called when the game starts or when spawned
void ADungeonCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADungeonCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
	// sanitize the movement input
	character_input_.Sanitize();

	// move the tank
	{
		FVector desired_movement_direction = FVector(character_input_.MovementInput.X, character_input_.MovementInput.Y, 0.f);
		if (!desired_movement_direction.IsNearlyZero())
		{
			AddMovementInput(desired_movement_direction);
		}
	}

	// Turn char to mouse location
	{
		FHitResult trace_result(ForceInit);
		controller_->GetHitResultUnderCursor(ECC_Visibility, false, trace_result);
		FVector hit_location = trace_result.Location;
		float new_yaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), hit_location).Add(0.f,-90.f,0.f).Yaw;
		current_yaw_ = new_yaw + 90.f;
		GetCapsuleComponent()->SetWorldRotation(FRotator(.0f, new_yaw, .0f));
	}
	// Regenerate health
	{
		if (health_ < 1.f)
		{
			health_ += health_regeneration_ * DeltaTime;
		}
		if (health_ > 1.f)
		{
			health_ = 1.f;
		}
	}
	// Handle input - code to put in the weapons
	{
		const FDungeonCharacterInput& current = this->GetCurrectInput();
		if (current.b_basic_attacking1h_)
		{
			if (UWorld* world = GetWorld())
			{
				UE_LOG(LogTemp, Warning, TEXT("spawn a basic attack"));
				
				if (AProjectile* new_projectile = Cast<AProjectile>(world->SpawnActor(BasicAttackProjectile)))
				{
					FVector location = GetMesh()->GetSocketLocation("RightHand");
					FRotator rotation = FRotator(0.f, this->GetCharacterYaw(), 0.f);
					new_projectile->SetActorLocation(location);
					new_projectile->SetActorRotation(rotation);
				}
			}
		}
	}

}

// Called to bind functionality to input
void ADungeonCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis(TEXT("MoveStraight"), this, &ADungeonCharacter::MoveStraight);
	InputComponent->BindAxis(TEXT("MoveSideways"), this, &ADungeonCharacter::MoveSideways);

	InputComponent->BindAction(TEXT("BasicAttack"), IE_Pressed, this, &ADungeonCharacter::OnBasicAttackPressed);
	InputComponent->BindAction(TEXT("BasicAttack"), IE_Released, this, &ADungeonCharacter::OnBasicAttackReleased);
	InputComponent->BindAction(TEXT("SpecialAttack"), IE_Pressed, this, &ADungeonCharacter::OnSpecialAttackPressed);
	InputComponent->BindAction(TEXT("SpecialAttack"), IE_Released, this, &ADungeonCharacter::OnSpecialAttackReleased);

	InputComponent->BindAction(TEXT("Ability1"), IE_Pressed, this, &ADungeonCharacter::OnAbility1Pressed);
	InputComponent->BindAction(TEXT("Ability1"), IE_Released, this, &ADungeonCharacter::OnAbility1Released);
	InputComponent->BindAction(TEXT("Ability2"), IE_Pressed, this, &ADungeonCharacter::OnAbility2Pressed);
	InputComponent->BindAction(TEXT("Ability2"), IE_Released, this, &ADungeonCharacter::OnAbility2Released);
	InputComponent->BindAction(TEXT("Ability3"), IE_Pressed, this, &ADungeonCharacter::OnAbility3Pressed);
	InputComponent->BindAction(TEXT("Ability3"), IE_Released, this, &ADungeonCharacter::OnAbility3Released);
	InputComponent->BindAction(TEXT("Ability4"), IE_Pressed, this, &ADungeonCharacter::OnAbility4Pressed);
	InputComponent->BindAction(TEXT("Ability4"), IE_Released, this, &ADungeonCharacter::OnAbility4Released);
}

// called when forward movement input happens
void ADungeonCharacter::MoveStraight(float axis_value)
{
	character_input_.MoveStraight(axis_value);
}

// called when sideways movement input happens
void ADungeonCharacter::MoveSideways(float axis_value)
{
	character_input_.MoveSideways(axis_value);
}

// called for basic attack
void ADungeonCharacter::OnBasicAttackPressed()
{
	character_input_.BasicAttack(true);
}

void ADungeonCharacter::OnBasicAttackReleased()
{
	character_input_.BasicAttack(false);
}

// called for special attack
void ADungeonCharacter::OnSpecialAttackPressed()
{
	character_input_.SpecialAttack(true);
}
void ADungeonCharacter::OnSpecialAttackReleased()
{
	character_input_.SpecialAttack(false);
}

// called for ability 1
void ADungeonCharacter::OnAbility1Pressed()
{
}

void ADungeonCharacter::OnAbility1Released()
{
}

// called for ability 2
void ADungeonCharacter::OnAbility2Pressed()
{
}

void ADungeonCharacter::OnAbility2Released()
{
}

// called for ability 3
void ADungeonCharacter::OnAbility3Pressed()
{
}

void ADungeonCharacter::OnAbility3Released()
{
}

// called for ability 4
void ADungeonCharacter::OnAbility4Pressed()
{
}

void ADungeonCharacter::OnAbility4Released()
{
}

const float ADungeonCharacter::GetCharacterYaw()
{
	return current_yaw_;
}