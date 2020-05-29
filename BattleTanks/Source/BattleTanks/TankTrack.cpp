// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTrack.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;	
}

void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	ApplySideWaysForce();
	DriveTrack();
	CurrentThrottle = 0.f;
}

void UTankTrack::ApplySideWaysForce()
{
	auto SlippageSpeed = FVector::DotProduct(GetComponentVelocity(), GetRightVector());

	auto DeltaTime = GetWorld()->GetDeltaSeconds();

	auto CorrectionAcceleration =  - SlippageSpeed / DeltaTime * GetRightVector();

	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());

	auto CorrectionForce = TankRoot->GetMass() * CorrectionAcceleration / 2;

	TankRoot->AddForce(CorrectionForce);
}

void UTankTrack::BeginPlay()
{
	Super::BeginPlay();
	
	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

void UTankTrack::DriveTrack()
{
	auto ForceApplied = GetForwardVector() * CurrentThrottle * TrackMaxDrivingForce;
	auto ForceLocation = GetComponentLocation();
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}

void UTankTrack::SetThrottle(float Throttle)
{
	CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, -1,1);	
}
