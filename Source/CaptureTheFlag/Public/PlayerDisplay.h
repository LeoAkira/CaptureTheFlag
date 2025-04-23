// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTFPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "PlayerDisplay.generated.h"

/**
 * 
 */
UCLASS()
class CAPTURETHEFLAG_API UPlayerDisplay : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeDisplay(ACTFPlayerController* PlayerController);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float NewHealth);
};
