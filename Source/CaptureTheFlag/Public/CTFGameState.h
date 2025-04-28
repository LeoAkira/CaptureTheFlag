// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/GameState.h"
#include "CTFGameState.generated.h"

class ACTFPlayerState;
class ATeamBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchStartedSignature, const TArray<FGameplayTag>&, Teams);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScoreUpdatedSignature, FGameplayTag, Team, int, NewScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameOverSignature, FGameplayTag, WinningTeam);

/**
 * 
 */
UCLASS()
class CAPTURETHEFLAG_API ACTFGameState : public AGameState
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FOnMatchStartedSignature OnMatchStarted;
	
	UPROPERTY()
	FOnScoreUpdatedSignature OnScoreUpdated;

	UPROPERTY()
	FOnGameOverSignature OnGameOver;

protected:
	virtual void BeginPlay() override;
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;
	virtual void OnRep_MatchState() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, int> TeamPoints;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Player")
	TArray<FGameplayTag> TeamTags;

private:
	UFUNCTION()
	void OnTeamScored(FGameplayTag TeamTag);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnGameOver(FGameplayTag WinningTeam);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnMatchStarted(const TArray<FGameplayTag>& TeamsTags);

	void Initialize();
	bool Initialized = false;
};
