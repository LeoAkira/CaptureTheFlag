// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlagSpawnPoint.generated.h"

UCLASS()
class CAPTURETHEFLAG_API AFlagSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	UFUNCTION(BlueprintCallable)
	AFlag* SpawnFlag();

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AFlag> FlagClass;
};
