// Fill out your copyright notice in the Description page of Project Settings.


#include "FlagSpawnPoint.h"
#include "Flag.h"


AFlag* AFlagSpawnPoint::SpawnFlag()
{
	AFlag* Flag = GetWorld()->SpawnActor<AFlag>(FlagClass, GetActorLocation(), GetActorRotation());
	Flag->InSpawnPoint = true;
	return Flag;
}
