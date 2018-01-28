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

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CharacterBase"));
	}

	controller_ = UGameplayStatics::GetPlayerController(GetWorld(), 0);

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
	camera_->SetWorldRotation(FRotator(-40.f, 0.f, 0.f));
	camera_->SetupAttachment(camera_boom_, USpringArmComponent::SocketName);

	health_ = .4f;

	casting1h_ = false;
	basic_attacking1h_ = false;
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

	if (health_ < 1.f)
	{
		health_ += 0.01 * DeltaTime;
	}
	if (health_ > 1.f)
	{
		health_ = 1.f;
	}
}

// Called to bind functionality to input
void ADungeonCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis(TEXT("MoveStraight"), this, &ADungeonCharacter::MoveStraight);
	InputComponent->BindAxis(TEXT("MoveSideways"), this, &ADungeonCharacter::MoveSideways);

	InputComponent->BindAction(TEXT("BasicAttack"), IE_Pressed, this, &ADungeonCharacter::OnBasicAttackPressed);
	InputComponent->BindAction(TEXT("SpecialAttack"), IE_Pressed, this, &ADungeonCharacter::OnSpecialAttackPressed);

	InputComponent->BindAction(TEXT("Ability1"), IE_Pressed, this, &ADungeonCharacter::OnAbility1Pressed);
	InputComponent->BindAction(TEXT("Ability2"), IE_Pressed, this, &ADungeonCharacter::OnAbility2Pressed);
	InputComponent->BindAction(TEXT("Ability3"), IE_Pressed, this, &ADungeonCharacter::OnAbility3Pressed);
	InputComponent->BindAction(TEXT("Ability4"), IE_Pressed, this, &ADungeonCharacter::OnAbility4Pressed);
}

// called when forward movement input happens
void ADungeonCharacter::MoveStraight(float value)
{
	if (value != 0.0f)
	{
		AddMovementInput(FVector(90.f,.0f,.0f), value);
	}
}

// called when sideways movement input happens
void ADungeonCharacter::MoveSideways(float value)
{
	if (value != 0.0f)
	{
		AddMovementInput(FVector(.0f, 90.f, .0f), value);
	}
}

// called for basic attack
void ADungeonCharacter::OnBasicAttackPressed()
{
	if (!basic_attacking1h_ && !casting1h_)
	{
		basic_attacking1h_ = true;
		//GetWorldTimerManager().SetTimer(UnusedHandle, this, &ADungeonCharacter::ResetBasicAttack, 2.f, false);
	}
}

void ADungeonCharacter::ResetBasicAttack()
{
	basic_attacking1h_ = false;
}

// called for special attack
void ADungeonCharacter::OnSpecialAttackPressed()
{

}

// called for ability 1
void ADungeonCharacter::OnAbility1Pressed()
{

}

// called for ability 2
void ADungeonCharacter::OnAbility2Pressed()
{

}

// called for ability 3
void ADungeonCharacter::OnAbility3Pressed()
{

}

// called for ability 4
void ADungeonCharacter::OnAbility4Pressed()
{

}