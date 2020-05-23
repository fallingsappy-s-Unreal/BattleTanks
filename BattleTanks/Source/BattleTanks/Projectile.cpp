// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "HeavyProjectileMovementComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HeavyProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(FName("HeavyProjectileMovementComponent"));
	HeavyProjectileMovementComponent->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::Launch(float Speed)
{
	HeavyProjectileMovementComponent->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
	HeavyProjectileMovementComponent->Activate();
}

