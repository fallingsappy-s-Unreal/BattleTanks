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
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) return false;
	
	if (AimDirection.Equals(Barrel->GetForwardVector(), 0.01f))
	{
		return false;
	}

	return true;
}

void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{	if (AmmoCount > 0)
	{
		//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
		bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;

		if (isReloaded)
		{
			if (IsBarrelMoving())
			{
				FiringStatus = EFiringStatus::Aiming;
			}
			else
			{
				FiringStatus = EFiringStatus::Locked;
			}
		}
	}
	else
	{
		FiringStatus = EFiringStatus::OutOfAmmo;
	}
}

void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	LastFireTime = FPlatformTime::Seconds();
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
			AimDirection = LaunchVelocity.GetSafeNormal();

			MoveBarrelTowards(AimDirection);
		}
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector TargetAimDirection)
{
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();

	auto AimAsRotator = TargetAimDirection.Rotation();

	auto DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->Elevate(DeltaRotator.Pitch);

	if (FMath::Abs(DeltaRotator.Yaw) < 180)
	{
		Turret->Rotate(DeltaRotator.Yaw);
	}
	else
	{
		Turret->Rotate(-DeltaRotator.Yaw);
	}
}

void UTankAimingComponent::Fire()
{
	if (!ensure(Barrel && ProjectileBlueprint)) return;

	if (FiringStatus != EFiringStatus::Reloading && AmmoCount > 0)
	{		
		auto ProjectileStartLocation = Barrel->GetSocketLocation(FName("Projectile"));
		auto ProjectileStartRotation = Barrel->GetSocketRotation(FName("Projectile"));

		auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, ProjectileStartLocation, ProjectileStartRotation);

		Projectile->Launch(LaunchSpeed);

		FiringStatus = EFiringStatus::Reloading;

		AmmoCount--;
		
		LastFireTime = FPlatformTime::Seconds();
	}
}

EFiringStatus UTankAimingComponent::GetFiringStatus() const
{
	return FiringStatus;
}

int32 UTankAimingComponent::GetAmmoLeft() const
{
	return AmmoCount;
}
