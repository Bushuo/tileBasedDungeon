// Fill out your copyright notice in the Description page of Project Settings.

#include "tileBasedDungeon.h"
#include "DungeonPlayerController.h"


ADungeonPlayerController::ADungeonPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::None;
}

void ADungeonPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void ADungeonPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction(TEXT("ClickToInteract"), IE_Pressed, this, &ADungeonPlayerController::OnClickToInteractPressed);
	InputComponent->BindAction(TEXT("ClickToInteract"), IE_Released, this, &ADungeonPlayerController::OnClickToInteractReleased);
}

void ADungeonPlayerController::MoveToMouseCursor()
{
	// Trace to see what is under the mouse cursor
	FHitResult hit;
	GetHitResultUnderCursor(ECC_Visibility, false, hit);

	if (hit.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(hit.ImpactPoint);
	}
}

void ADungeonPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const pawn = GetPawn();
	if (pawn)
	{
		UNavigationSystem* const nav_sys = GetWorld()->GetNavigationSystem();
		float const distance = FVector::Dist(DestLocation, pawn->GetActorLocation());

		if (nav_sys && (distance > 120.0f))
		{
			nav_sys->SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void ADungeonPlayerController::OnClickToInteractPressed()
{
	MoveToMouseCursor();
}

void ADungeonPlayerController::OnClickToInteractReleased()
{

}
