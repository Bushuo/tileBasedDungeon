// Fill out your copyright notice in the Description page of Project Settings.

#include "tileBasedDungeon.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "DungeonCharacter.h"


// Sets default values
ADungeonCharacter::ADungeonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	controller_ = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create camera_boom_ to Attach the Camera to
	camera_boom_ = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	camera_boom_->SetupAttachment(RootComponent);
	camera_boom_->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	camera_boom_->SetRelativeRotation(FRotator(-40.f,0.f,0.f));
	camera_boom_->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Attach Camera to camera_boom_
	camera_ = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera_->SetupAttachment(camera_boom_, USpringArmComponent::SocketName);
	camera_->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
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

	FHitResult trace_result(ForceInit);
	controller_->GetHitResultUnderCursor(ECC_Visibility, false, trace_result);
	FVector hit_location = trace_result.Location;
	float new_yaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), hit_location).Yaw;
	GetMesh()->SetWorldRotation(FRotator(.0f, new_yaw - 90.f, .0f));
}

// Called to bind functionality to input
void ADungeonCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis(TEXT("MoveStraight"), this, &ADungeonCharacter::MoveStraight);
	InputComponent->BindAxis(TEXT("MoveSideways"), this, &ADungeonCharacter::MoveSideways);
}

void ADungeonCharacter::MoveStraight(float value)
{
	if (value != 0.0f)
	{
		AddMovementInput(FVector(90.f,.0f,.0f), value);
	}
}

void ADungeonCharacter::MoveSideways(float value)
{
	if (value != 0.0f)
	{
		AddMovementInput(FVector(.0f, 90.f, .0f), value);
	}
}



// Called when Click input happens
void ADungeonCharacter::OnClickToInteractPressed()
{
}

void ADungeonCharacter::OnClickToInteractReleased()
{
}

