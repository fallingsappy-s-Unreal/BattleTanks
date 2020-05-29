// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

UENUM()
enum class EFiringStatus : uint8
{
	Reloading,
	Aiming,
	Locked
};

class UTankTurret;
class UTankBarrel;
class AProjectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANKS_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = Setup)
	void Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);
	
	void AimAt(FVector HitLocation);
	
	UFUNCTION(BlueprintCallable, Category = Firing)
	void Fire();
	
protected:

	UPROPERTY(BlueprintReadOnly, Category = State)
	EFiringStatus FiringStatus = EFiringStatus::Reloading;
	FVector AimDirection;

private:
	void MoveBarrelTowards(FVector AimDirection);

	// Sets default values for this component's properties
	UTankAimingComponent();

	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Firing)
	float LaunchSpeed = 100000.f;

	UPROPERTY(EditDefaultsOnly, Category = Firing)
	float ReloadTimeInSeconds = 3;

	double LastFireTime = 0;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<AProjectile> ProjectileBlueprint;

	bool IsBarrelMoving();
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void BeginPlay() override;
};
