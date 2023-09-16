// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
	GloblaStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);

}

void AMovingPlatform::AddActiveTrigger()
{
	ActiveTrigger++;
}

void AMovingPlatform::RemoveActiveTrigger()
{
	if (ActiveTrigger > 0)
	{
		ActiveTrigger--;

	}
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ActiveTrigger > 0)
	{
		if (HasAuthority())
		{
			FVector Location = GetActorLocation();

			float JourneyLength = (float)(GlobalTargetLocation - GloblaStartLocation).Size();
			float JourneyTravelled = (float)(Location - GloblaStartLocation).Size();

			if (JourneyTravelled >= JourneyLength)
			{
				FVector SwapTemp = GlobalTargetLocation;
				GlobalTargetLocation = GloblaStartLocation;
				GloblaStartLocation = SwapTemp;
			}

			FVector Direction = (GlobalTargetLocation - GloblaStartLocation).GetSafeNormal();
			Location += Direction * Speed * DeltaTime;
			SetActorLocation(Location);
		}
	}
	
}
