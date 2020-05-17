// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "TankPlayerController.h"
#include "Engine/World.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	const auto ControlledTank = GetControlledTank();
	const auto PlayerTank = GetPlayerTank();
	
	if (!ControlledTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController not possesing a tank"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController possesing a tank: %s"), *ControlledTank->GetName());
	}

	if (!PlayerTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController not possesing a tank"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController aware of Player Tank: %s"), *PlayerTank->GetName());
	}
}

ATank* ATankAIController::GetPlayerTank() const
{
	return Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}
