// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "TankAimingComponent.h"
#include "Engine/World.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	auto TankAimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();

	if (!ensure(TankAimingComponent)) return;

	FoundAimingComponent(TankAimingComponent);
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetPawn()) {return;}
	
	auto TankAimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();

	if (!ensure(TankAimingComponent)) return;

	FVector HitLocation;
	if (GetSightRayHitLocation(HitLocation))
	{
		TankAimingComponent->AimAt(HitLocation);
	}
	
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
	int32 ViewportSizeX, ViewPortSizeY;
	GetViewportSize(ViewportSizeX, ViewPortSizeY);

	FVector2D ScreenLocation = FVector2D(ViewportSizeX * CrossHairXLocation, ViewPortSizeY * CrossHairYLocation);
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection))
	{
		return GetLookVectorHitLocation(LookDirection, OutHitLocation);		
	}
	
	return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector WorldLocation;

	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, WorldLocation, LookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
	FHitResult Hit;
	const auto StartLocation = PlayerCameraManager->GetCameraLocation();
	const auto EndLocation = StartLocation + LookDirection * LineTraceRange;
	
	if( GetWorld()->LineTraceSingleByChannel(
		Hit,
		StartLocation,
		EndLocation,
		ECC_Visibility
	))
	{
		HitLocation = Hit.Location;
		return true;
	}

	HitLocation = FVector(0);
	return false;
}