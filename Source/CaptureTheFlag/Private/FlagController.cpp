// Fill out your copyright notice in the Description page of Project Settings.


#include "FlagController.h"
#include "Flag.h"
#include "FlagSpawnPoint.h"
#include "Kismet/GameplayStatics.h"

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

AFlag* AFlagController::GetFlag()
{
	if (!Flag)
	{
		Flag = GetWorld()->SpawnActor<AFlag>(FlagClass, FTransform());
		Flag->HideFlag();
		Flag->OnFlagAutoDestroyed.AddDynamic(this, &AFlagController::StartFlagRespawn);
	}
	return Flag;
}

void AFlagController::StartFlagRespawn()
{
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this]{
		GetFlag()->ShowFlag(SpawnPoint->GetActorLocation(), true);
	});
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, FlagRespawnTime, false);
}

void AFlagController::SpawnFlagAt(FVector Location)
{
	GetFlag()->ShowFlag(Location, false);
}

void AFlagController::RemoveFlag()
{
	GetFlag()->HideFlag();
}


