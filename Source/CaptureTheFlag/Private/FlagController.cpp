// Fill out your copyright notice in the Description page of Project Settings.


#include "FlagController.h"
#include "Flag.h"
#include "FlagSpawnPoint.h"
#include "Kismet/GameplayStatics.h"

AFlagController::AFlagController()
{
	PrimaryActorTick.bCanEverTick = true;
}

AFlag* AFlagController::SpawnFlag(FVector Location)
{
	AFlag* Flag = GetWorld()->SpawnActor<AFlag>(FlagClass, Location, FRotator(0, 0, 0));
	Flag->OnFlagAutoDestroyed.AddDynamic(this, &AFlagController::StartFlagRespawn);
	return Flag; 
}

void AFlagController::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATeamBase::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		ATeamBase* DeliveryPoint = Cast<ATeamBase>(Actor);
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
 			SpawnFlag(SpawnPoint->GetActorLocation())->InSpawnPoint = true;
			RespawnFlag = false;
		}
	}
}

