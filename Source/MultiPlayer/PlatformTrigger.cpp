// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformTrigger.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "MovingPlatform.h"

// Sets default values
APlatformTrigger::APlatformTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("TriggerVolume"));
	if (!ensure(TriggerVolume != nullptr))
		return;
	RootComponent = TriggerVolume;


}

// Called when the game starts or when spawned
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &APlatformTrigger::OnOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddUniqueDynamic(this, &APlatformTrigger::OnOverlapEnd);

	
}

// Called every frame
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlatformTrigger::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	for (auto Platform : PlatformsToTrigger)
	{
		Platform->AddActiveTrigger();
		UE_LOG(LogTemp, Warning, TEXT("Activate"));
	}
}

void APlatformTrigger::OnOverlapEnd(
	UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	for (auto Platform : PlatformsToTrigger)
	{
		Platform->RemoveActiveTrigger();
		UE_LOG(LogTemp, Warning, TEXT("Deativate"));

	}
}

