// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeamBase.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "FlagController.generated.h"

class AFlagSpawnPoint;

UCLASS()
class CAPTURETHEFLAG_API AFlagController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlagController();

	UPROPERTY(BlueprintAssignable)
	FOnFlagDeliveredSignature OnFlagDelivered;

	UFUNCTION(BlueprintCallable)
	AFlag* SpawnFlag(FVector Location);

	UFUNCTION(BlueprintCallable)
	void StartFlagRespawn();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ATeamBase*> DeliveryPoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AFlagSpawnPoint* SpawnPoint;

	UFUNCTION()
	void FlagDelivered(FGameplayTag TeamTag);

	UPROPERTY()
	float FlagRespawnTime = 5.f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFlag> FlagClass;
	
private:
	bool RespawnFlag = false;
	float CurrentFlagRespawnTime = 0.f;
};
