// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

class UTankAimingComponent;

UCLASS()
class BATTLETANKS_API ATankAIController : public AAIController
{
	GENERATED_BODY()

public:
	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;

	virtual void SetPawn(APawn* InPawn) override;
	
	UPROPERTY(EditAnywhere, Category = Setup)
	float AcceptanceRadius = 10000;

	UFUNCTION()
	void OnTankDeath();
};
