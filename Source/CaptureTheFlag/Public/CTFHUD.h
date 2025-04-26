// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CTFPlayerController.h"
#include "CTFPlayerState.h"
#include "CTFAttributeSet.h"
#include "CTFAbilitySystemComponent.h"
#include "CTFHUD.generated.h"

class UPlayerDisplay;
/**
 * 
 */
UCLASS()
class CAPTURETHEFLAG_API ACTFHUD : public AHUD
{
	GENERATED_BODY()

public:
	void InitializeHUD(ACTFPlayerController* PlayerController);

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerDisplay> PlayerDisplayClass;
	
private:
	UPROPERTY()
	ACTFPlayerController* CTFPlayerController;

	UPROPERTY()
	ACTFPlayerState* CTFPlayerState;

	UPROPERTY()
	UCTFAttributeSet* CTFAttributeSet;
};
