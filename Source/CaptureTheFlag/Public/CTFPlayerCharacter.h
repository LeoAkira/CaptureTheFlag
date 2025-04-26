// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CharacterTeamMaterialsInfo.h"
#include "GameFramework/Character.h"
#include "WeaponUserInterface.h"
#include "CTFPlayerCharacter.generated.h"

class UTP_WeaponComponent;
class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;
class UInputMappingContext;
class UInputAction;
class UAttributeSet;
class UAbilitySystemComponent;
class UGameplayAbility;

UCLASS(BlueprintType, config=Game)
class ACTFPlayerCharacter : public ACharacter, public IWeaponUserInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACTFPlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* FlagComponent;
	
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config | Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Config | Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Config | Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Config | Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* ShootAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Config")
	TSubclassOf<UGameplayAbility> ShootAbility;

	UPROPERTY(EditDefaultsOnly, Category="Config | Weapon")
	FName WeaponGripSocketName;
	
	UPROPERTY(EditDefaultsOnly, Category="Config | Weapon")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, Category="Config | Weapon")
	UAnimMontage* ShootMontage;

	//IWeaponUserInterface
	virtual FVector GetMuzzleLocation() override;
	virtual FRotator GetMuzzleRotation() override;
	virtual void PlayShootMontage() override;
	//end IWeaponUserInterface

	//IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//end IAbilitySystemInterface

	UFUNCTION(BlueprintPure)
	float GetCameraPitch();
protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Config")
	TArray<FName> BonesToHideInFirstPerson;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Config")
	float MaxHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Config")
	UCharacterTeamMaterials* TeamMaterials;

	UFUNCTION()
	void OnGameplayTagCountChanged(FGameplayTag Tag, int32 Count);

	UFUNCTION()
	void CharacterDeath();
	
private:
	void InitializeCharacter();
	void InitializeController();

	UPROPERTY()
	float CameraPitch = 0.f;

	UFUNCTION(Server, Reliable)
	void SetCameraPitch(float NewValue);

	UFUNCTION(NetMulticast, Reliable)
	void SetCameraPitchMulticast(float NewValue);

	UFUNCTION(NetMulticast, Reliable)
	void OnHealthChanged(float NewValue);

	UFUNCTION(NetMulticast, Reliable)
	void SetTeamMaterials(FGameplayTag TeamTag);
};

