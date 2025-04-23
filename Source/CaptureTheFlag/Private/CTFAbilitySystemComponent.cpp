// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFAbilitySystemComponent.h"

#include "ShootGameplayAbility.h"

void UCTFAbilitySystemComponent::AddShootAbility(const TSubclassOf<UGameplayAbility> Ability)
{
	GiveAbility(FGameplayAbilitySpec(Ability, 1));
}

void UCTFAbilitySystemComponent::ActivateShootAbility()
{
	FGameplayAbilitySpec ShootAbilitySpec = GetActivatableAbilities()[0];
	AbilitySpecInputPressed(ShootAbilitySpec);
	if (!ShootAbilitySpec.IsActive())
	{
		TryActivateAbility(ShootAbilitySpec.Handle);
	}
}
