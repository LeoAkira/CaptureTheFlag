// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY(BlueprintReadOnly)
	float MaxHealth;
};
