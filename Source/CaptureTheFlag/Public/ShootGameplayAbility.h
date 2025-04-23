// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "ShootGameplayAbility.generated.h"

class AProjectile;
/**
 * 
 */
UCLASS()
class CAPTURETHEFLAG_API UShootGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

	void SpawnProjectile(AActor* AvatarActor);
	void PlaySound(AActor* AvatarActor);
	void PlayFireMontage(AActor* AvatarActor);

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> OnHitGameplayEffect;
};
