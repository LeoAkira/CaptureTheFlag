// Fill out your copyright notice in the Description page of Project Settings.


#include "FireGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "CTFProjectile.h"
#include "WeaponUserInterface.h"
#include "Kismet/GameplayStatics.h"

void UFireGameplayAbility::SpawnProjectile(AActor* AvatarActor)
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
	ACTFProjectile* Projectile = GetWorld()->SpawnActorDeferred<ACTFProjectile>(
		ProjectileClass,
		SpawnTransform,
		AvatarActor,
		Cast<APawn>(AvatarActor),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	//TODO: Damage Effect
		
	Projectile->FinishSpawning(SpawnTransform);
}

void UFireGameplayAbility::PlaySound(AActor* AvatarActor)
{
	// Try and play the sound if specified
	if (FireSound == nullptr) return;
	
	UGameplayStatics::PlaySoundAtLocation(this, FireSound, AvatarActor->GetActorLocation());
}

void UFireGameplayAbility::PlayFireMontage(AActor* AvatarActor)
{
	Cast<IWeaponUserInterface>(AvatarActor)->PlayFireMontage();
}

void UFireGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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
