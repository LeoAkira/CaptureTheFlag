// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "TeamBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFlagDeliveredSignature, FGameplayTag, Team);

UCLASS()
class CAPTURETHEFLAG_API ATeamBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeamBase();

	UPROPERTY(BlueprintAssignable)
	FOnFlagDeliveredSignature OnFlagDelivered;

	UFUNCTION(BlueprintCallable)
	FTransform GetRandomSpawnPoint();

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetTeamTag() { return TeamTag; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* Box;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FTransform> SpawnPoints;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag TeamTag;
private:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);	
};
