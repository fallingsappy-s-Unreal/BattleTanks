// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "TankBarrel.h"
#include "TankTurret.h"

void UTankAimingComponent::Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (ensure(Barrel && Turret))
	{
		auto StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
		FVector LaunchVelocity = FVector(0);

		bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
			this,
			LaunchVelocity,
			StartLocation,
			HitLocation,
			LaunchSpeed,
			false,
			0,
			0,
			ESuggestProjVelocityTraceOption::DoNotTrace
		);
		
		if (bHaveAimSolution)
		{
			auto AimDirection = LaunchVelocity.GetSafeNormal();

			MoveBarrelTowards(AimDirection);
		}
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();

	auto AimAsRotator = AimDirection.Rotation();

	auto DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->Elevate(DeltaRotator.Pitch);
	Turret->Rotate(DeltaRotator.Yaw);
}

void UTankAimingComponent::Fire()
{
	if (!ensure(Barrel && ProjectileBlueprint)) return;

	bool IsReloaded = FPlatformTime::Seconds() - LastFireTime > ReloadTimeInSeconds;

	if (IsReloaded)
	{
		auto ProjectileStartLocation = Barrel->GetSocketLocation(FName("Projectile"));
		auto ProjectileStartRotation = Barrel->GetSocketRotation(FName("Projectile"));

		auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, ProjectileStartLocation, ProjectileStartRotation);

		Projectile->Launch(LaunchSpeed);

		LastFireTime = FPlatformTime::Seconds();
	}
}