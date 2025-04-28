// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDisplay.h"

#include "CTFAbilitySystemComponent.h"
#include "CTFPlayerController.h"
#include "CTFAttributeSet.h"
#include "CTFGameState.h"
#include "CTFPlayerState.h"

void UPlayerDisplay::InitializeDisplay_Implementation(ACTFPlayerController* PlayerController)
{
	if (const ACTFPlayerState* PlayerState = PlayerController->GetPlayerState<ACTFPlayerState>())
	{
		if (const UCTFAttributeSet* AttributeSet = Cast<UCTFAttributeSet>(PlayerState->GetAttributeSet()))
		{
			MaxHealth = AttributeSet->GetHealth();
			if (UCTFAbilitySystemComponent* AbilitySystemComponent = Cast<UCTFAbilitySystemComponent>(PlayerState->GetAbilitySystemComponent()))
			{
				AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddLambda(
					[this](const FOnAttributeChangeData& Data)
					{
						OnHealthChanged(Data.NewValue);
					}
				);
			}
		}
	}

	if (ACTFGameState* GameState = Cast<ACTFGameState>(GetWorld()->GetGameState()))
	{
		GameState->OnMatchStarted.AddDynamic(this, &UPlayerDisplay::OnGameStarted);
		GameState->OnScoreUpdated.AddDynamic(this, &UPlayerDisplay::OnScoreUpdated);
		GameState->OnGameOver.AddDynamic(this, &UPlayerDisplay::OnGameOver);
	}
}
