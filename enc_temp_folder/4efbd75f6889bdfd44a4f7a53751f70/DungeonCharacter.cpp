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

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false;
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

	// Create a decal in the world to show the cursor's location
	cursor_to_world_ = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	cursor_to_world_->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> decal_material_asset(TEXT("Material'/Game/Materials/M_CursorDecal.M_CursorDecal'"));
	if (decal_material_asset.Succeeded())
	{
		cursor_to_world_->SetDecalMaterial(decal_material_asset.Object);
	}
	cursor_to_world_->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	cursor_to_world_->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
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
	//Set cursor location every tick
	if (cursor_to_world_ != nullptr)
	{
		if (controller_)
		{
			FVector cursor_forward_vec = trace_result.ImpactNormal;
			FRotator cursor_rot = cursor_forward_vec.Rotation();
			cursor_to_world_->SetWorldLocation(trace_result.Location);
			cursor_to_world_->SetWorldRotation(cursor_rot);
		}
	}
	FVector hit_location = trace_result.Location;
	float new_yaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), hit_location).Yaw;
	//new_rotation.Pitch = GetMesh()->RelativeRotation.Pitch;
	//new_rotation.Roll = GetMesh()->RelativeRotation.Roll;
	//GetMesh()->SetRelativeRotation(new_rotation);
	controller_->SetControlRotation(FRotator(.0f,.0f,new_yaw));

	// OLD FUNCTIONALITY
	// Move the player to the location clicked if there is a change of location
	/*if (FVector::Dist(TargetLocation, GetActorLocation()) > 2.5f)
	{
		FVector Diff = TargetLocation - GetActorLocation();
		Diff.Normalize();

		GetMovementComponent()->AddInputVector(Diff);
	}*/
}

// Called to bind functionality to input
void ADungeonCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	// bind callbacks for input events
	//InputComponent->BindAction(TEXT("ClickToInteract"), IE_Pressed, this, &ADungeonCharacter::OnClickToInteractPressed);
	//InputComponent->BindAction(TEXT("ClickToInteract"), IE_Released, this, &ADungeonCharacter::OnClickToInteractReleased);

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

	// OLD INPUT FUNCTION
	/*FVector WorldLocation;
	FVector WorldDirection;

	// get playercontroller
	APlayerController* player_controller = Cast<APlayerController>(GetController());
	if (player_controller)
	{
		if (player_controller->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
		{
			//where the trace should stop
			FVector TraceEndLocation = Camera->GetForwardVector() + 12000 * WorldDirection;

			// setup trace parameters
			FCollisionQueryParams TraceParams(FName(TEXT("CameraTrace")), true, this);
			TraceParams.bTraceAsyncScene = true;
			TraceParams.bReturnPhysicalMaterial = true;
			TraceParams.AddIgnoredActor(this);
			TraceParams.AddIgnoredActor(GetOwner());

			FHitResult Hit(ForceInit);

			// trace
			if (GetWorld()->LineTraceSingleByChannel(Hit, WorldLocation, TraceEndLocation, ECC_Camera, TraceParams))
			{
				TargetLocation = Hit.ImpactPoint;
				// set to actors Z that it doesnt get stuck in terrain
				TargetLocation.Z = GetActorLocation().Z;


				FRotator NewRotation = FRotationMatrix::MakeFromX(TargetLocation - GetActorLocation()).Rotator();

				NewRotation.Yaw -= 90.f;
				GetMesh()->SetRelativeRotation(NewRotation);
			}
		}

	}*/
}

void ADungeonCharacter::OnClickToInteractReleased()
{
}

