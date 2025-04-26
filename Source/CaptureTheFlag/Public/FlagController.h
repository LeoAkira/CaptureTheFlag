// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Flag.h"
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
	UPROPERTY(BlueprintAssignable)
	FOnFlagDeliveredSignature OnFlagDelivered;

	UFUNCTION(BlueprintCallable)
	void StartFlagRespawn();

	UFUNCTION(BlueprintCallable)
	void SpawnFlagAt(FVector Location);

	UFUNCTION(BlueprintCallable)
	void RemoveFlag();
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ATeamBase*> DeliveryPoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AFlagSpawnPoint* SpawnPoint;

	UFUNCTION()
	void FlagDelivered(FGameplayTag TeamTag);

	UFUNCTION()
	AFlag* GetFlag();

	UPROPERTY(EditDefaultsOnly, Category = "Config | Flag")
	float FlagRespawnTime = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Config | Flag")
	TSubclassOf<AFlag> FlagClass;
private:
	bool RespawnFlag = false;
	float CurrentFlagRespawnTime = 0.f;

	UPROPERTY()
	TObjectPtr<AFlag> Flag;
};
