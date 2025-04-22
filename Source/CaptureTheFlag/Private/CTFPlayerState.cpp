// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFPlayerState.h"

#include "CTFAbilitySystemComponent.h"
#include "CTFAttributeSet.h"

ACTFPlayerState::ACTFPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCTFAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UCTFAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* ACTFPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* ACTFPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}
