// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CTFAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class CAPTURETHEFLAG_API UCTFAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AddShootAbility(const TSubclassOf<UGameplayAbility> Ability);
	
	void ActivateShootAbility();
};
