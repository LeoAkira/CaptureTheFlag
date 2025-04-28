// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "PlayerDisplay.generated.h"

class ACTFPlayerController;
class ACTFPlayerState;
class UCTFAttributeSet;
/**
 * 
 */
UCLASS()
class CAPTURETHEFLAG_API UPlayerDisplay : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void InitializeDisplay(ACTFPlayerController* PlayerController);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float NewHealth);

	UFUNCTION(BlueprintImplementableEvent)
	void OnScoreUpdated(FGameplayTag Team, int NewScore);

	UFUNCTION(BlueprintImplementableEvent)
	void OnGameStarted(const TArray<FGameplayTag>& Teams);

	UFUNCTION(BlueprintImplementableEvent)
	void OnGameOver(FGameplayTag WinningTeam);
	
	UPROPERTY(BlueprintReadOnly)
	float MaxHealth;
};
