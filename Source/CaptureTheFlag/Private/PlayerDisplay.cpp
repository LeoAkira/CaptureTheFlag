// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDisplay.h"
#include "CTFPlayerController.h"
#include "CTFAttributeSet.h"
#include "CTFPlayerState.h"

void UPlayerDisplay::InitializeDisplay_Implementation(ACTFPlayerController* PlayerController, ACTFPlayerState* PlayerState)
{
	PlayerController->OnHealthChanged.AddDynamic(this, &UPlayerDisplay::OnHealthChanged);

	MaxHealth = Cast<UCTFAttributeSet>(PlayerState->GetAttributeSet())->GetHealth();
}
