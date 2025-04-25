// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "CTFPlayerController.generated.h"

class UCTFAbilitySystemComponent;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
/**
 * 
 */
UCLASS()
class CAPTURETHEFLAG_API ACTFPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
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

	UPROPERTY()
	FOnHealthChangedSignature OnHealthChanged;

	void SetupDelegates();

	UPROPERTY()
	FGameplayTag TeamTag;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void Jump(const FInputActionValue& Value);

	UFUNCTION()
	void StopJumping(const FInputActionValue& Value);

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void Shoot(const FInputActionValue& Value);

private:
	UPROPERTY()
	TObjectPtr<UCTFAbilitySystemComponent> AbilitySystemComponent;

	UCTFAbilitySystemComponent* GetAbilitySystemComponent();
};
