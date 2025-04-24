// Fill out your copyright notice in the Description page of Project Settings.


#include "FlagController.h"
#include "Flag.h"
#include "FlagSpawnPoint.h"
#include "Kismet/GameplayStatics.h"

AFlagController::AFlagController()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AFlagController::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFlagDeliveryPoint::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		AFlagDeliveryPoint* DeliveryPoint = Cast<AFlagDeliveryPoint>(Actor);
		DeliveryPoint->OnFlagDelivered.AddDynamic(this, &AFlagController::FlagDelivered);
		DeliveryPoints.Add(DeliveryPoint);
	}

	SpawnPoint = Cast<AFlagSpawnPoint>(UGameplayStatics::GetActorOfClass(GetWorld(), AFlagSpawnPoint::StaticClass()));
}

void AFlagController::FlagDelivered(FGameplayTag TeamTag)
{
	OnFlagDelivered.Broadcast(TeamTag);
}

void AFlagController::StartFlagRespawn()
{
	CurrentFlagRespawnTime = 0.f;
	RespawnFlag = true;
}

// Called every frame
void AFlagController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RespawnFlag)
	{
		CurrentFlagRespawnTime += DeltaTime;
		if (CurrentFlagRespawnTime >= FlagRespawnTime)
		{
			SpawnPoint->SpawnFlag()->OnFlagAutoDestroyed.AddDynamic(this, &AFlagController::StartFlagRespawn);
			RespawnFlag = false;
		}
	}
}

