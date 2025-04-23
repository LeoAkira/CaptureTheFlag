// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Projectile.h"
#include "WeaponUserInterface.h"
#include "Kismet/GameplayStatics.h"

void UShootGameplayAbility::SpawnProjectile(AActor* AvatarActor)
{
	// Try and fire a projectile
	if (ProjectileClass == nullptr) return;
		
	IWeaponUserInterface* WeaponInterface = Cast<IWeaponUserInterface>(AvatarActor);
	FTransform SpawnTransform;
		
	SpawnTransform.SetLocation(WeaponInterface->GetMuzzleLocation());
	SpawnTransform.SetRotation(WeaponInterface->GetMuzzleRotation().Quaternion());

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	// Spawn the projectile at the muzzle
	AProjectile* Projectile = GetWorld()->SpawnActorDeferred<AProjectile>(
		ProjectileClass,
		SpawnTransform,
		AvatarActor,
		Cast<APawn>(AvatarActor),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AvatarActor);

	Projectile->OnHitEffect = SourceASC->MakeOutgoingSpec(OnHitGameplayEffect, 1.f, SourceASC->MakeEffectContext());
		
	Projectile->FinishSpawning(SpawnTransform);
}

void UShootGameplayAbility::PlaySound(AActor* AvatarActor)
{
	// Try and play the sound if specified
	if (FireSound == nullptr) return;
	
	UGameplayStatics::PlaySoundAtLocation(this, FireSound, AvatarActor->GetActorLocation());
}

void UShootGameplayAbility::PlayFireMontage(AActor* AvatarActor)
{
	Cast<IWeaponUserInterface>(AvatarActor)->PlayShootMontage();
}

void UShootGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (AvatarActor->HasAuthority())
	{
		SpawnProjectile(AvatarActor);
	}
	PlaySound(AvatarActor);
	PlayFireMontage(AvatarActor);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}
