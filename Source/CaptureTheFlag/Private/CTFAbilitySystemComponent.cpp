// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFAbilitySystemComponent.h"

void UCTFAbilitySystemComponent::AddFireAbility(const TSubclassOf<UGameplayAbility> Ability)
{
	FireAbilitySpec = FGameplayAbilitySpec(Ability, 1);
	GiveAbility(FireAbilitySpec);
}

void UCTFAbilitySystemComponent::ActivateFireAbility()
{
	AbilitySpecInputPressed(FireAbilitySpec);
	if (!FireAbilitySpec.IsActive())
	{
		TryActivateAbility(FireAbilitySpec.Handle);
	}
}
