// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerState.h"
#include "CTFPlayerState.generated.h"

class UCTFAbilitySystemComponent;
class UCTFAttributeSet;
class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 
 */
UCLASS()
class CAPTURETHEFLAG_API ACTFPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACTFPlayerState();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UAttributeSet* GetAttributeSet() const;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCTFAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UCTFAttributeSet> AttributeSet;
};
