// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "WeaponUserInterface.h"
#include "CTFPlayerCharacter.generated.h"

class UCTFAttributeSet;
class UCTFAbilitySystemComponent;
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
class ACTFPlayerCharacter : public ACharacter, public IWeaponUserInterface
{
	GENERATED_BODY()

public:
	ACTFPlayerCharacter();

	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponComponent;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Config | Input", meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
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

	virtual void PossessedBy(AController* NewController) override;
	
	virtual void OnRep_PlayerState() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Config")
	TSubclassOf<UGameplayAbility> FireAbility;

	UPROPERTY(EditDefaultsOnly, Category="Config | Weapon")
	FName WeaponGripSocketName;
	
	UPROPERTY(EditDefaultsOnly, Category="Config | Weapon")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, Category="Config | Weapon")
	UAnimMontage* FirstPersonFireMontage;
	
	UPROPERTY(EditDefaultsOnly, Category="Config | Weapon")
	UAnimMontage* ThirdPersonFireMontage;
	/*
	 * IWeaponUserInterface
	 */
	virtual FVector GetMuzzleLocation() override;
	
	virtual FRotator GetMuzzleRotation() override;

	virtual void PlayFireMontage() override;
	/*
	 * end IWeaponUserInterface
	 */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastFireMontage();
protected:
	virtual void BeginPlay() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Shoot(const FInputActionValue& Value);

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	UPROPERTY()
	TObjectPtr<UCTFAttributeSet> AttributeSet;
	
	UPROPERTY()
	TObjectPtr<UCTFAbilitySystemComponent> AbilitySystemComponent;
private:

	TArray<TSubclassOf<UGameplayAbility>> Abilities;

	void InitAbilityActorInfo();
	void InitializeAttributes();
};

