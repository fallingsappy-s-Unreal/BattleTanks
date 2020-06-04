// Fill out your copyright notice in the Description page of Project Settings.

#include "SprungWheel.h"
#include "TankTrack.h"

#include "SpawnPoint.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;	
}

TArray<ASprungWheel*> UTankTrack::GetWheels() const
{
	TArray<USceneComponent*> ChildrenComponents;
	GetChildrenComponents(true, ChildrenComponents);

	TArray<ASprungWheel*> Wheels;

	for (auto ChildrenComponent : ChildrenComponents)
	{
		auto SpawnPointChild = Cast<USpawnPoint>(ChildrenComponent);
		
		if (SpawnPointChild)
		{
			AActor* SpawnedChild = SpawnPointChild->GetSpawnedActor();

			if (SpawnedChild)
			{
				auto SprungWheel = Cast<ASprungWheel>(SpawnedChild);

				if (SprungWheel)
					Wheels.Add(SprungWheel);
			}			
		}					
	}

	return Wheels;
}

void UTankTrack::DriveTrack(float CurrentThrottle)
{
	auto ForceApplied = CurrentThrottle * TrackMaxDrivingForce;

	auto Wheels = GetWheels();
	auto ForcePerWheel = ForceApplied / Wheels.Num();

	for (ASprungWheel* Wheel : Wheels)
	{
		Wheel->AddDrivingForce(ForcePerWheel);
		UE_LOG(LogTemp, Warning, TEXT("%f"), CurrentThrottle);
	}
}

void UTankTrack::SetThrottle(float Throttle)
{
	float CurrentThrottle = FMath::Clamp<float>(Throttle, -1,1);
	DriveTrack(CurrentThrottle);
}
