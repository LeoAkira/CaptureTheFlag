// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDisplay.h"

#include "CTFAbilitySystemComponent.h"
#include "CTFPlayerController.h"
#include "CTFAttributeSet.h"
#include "CTFPlayerState.h"

void UPlayerDisplay::InitializeDisplay_Implementation(ACTFPlayerController* PlayerController)
{
	ACTFPlayerState* PlayerState = PlayerController->GetPlayerState<ACTFPlayerState>();
	UCTFAttributeSet* AttributeSet = Cast<UCTFAttributeSet>(PlayerState->GetAttributeSet());
	UCTFAbilitySystemComponent* AbilitySystemComponent = Cast<UCTFAbilitySystemComponent>(PlayerState->GetAbilitySystemComponent());
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged(Data.NewValue);
		}
	);
	
	MaxHealth = AttributeSet->GetHealth();
}
