// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CTFAbilitySystemComponent.h"
#include "CTFAttributeSet.h"
#include "CTFGameMode.h"
#include "CTFPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


void ACTFPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	SetShowMouseCursor(false);
}

void ACTFPlayerController::SetupDelegates()
{
	const UCTFAttributeSet* AttributeSet = Cast<UCTFAttributeSet>(GetAbilitySystemComponent()->GetAttributeSet(UCTFAttributeSet::StaticClass()));

	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddLambda
	(
		[this](const FOnAttributeChangeData& Data)
		{
			if (Data.NewValue <= 0)
			{
				OnPlayerDied.Broadcast(this);
			}
		}
	);
}

void ACTFPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACTFPlayerController::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACTFPlayerController::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACTFPlayerController::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACTFPlayerController::Look);

		//Shooting
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ACTFPlayerController::Shoot);
	}
}

void ACTFPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	SetupDelegates();
}

void ACTFPlayerController::Jump(const FInputActionValue& Value)
{
	GetCharacter()->Jump();
}

void ACTFPlayerController::StopJumping(const FInputActionValue& Value)
{
	GetCharacter()->StopJumping();
}

void ACTFPlayerController::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	GetCharacter()->AddMovementInput(GetCharacter()->GetActorForwardVector(), MovementVector.Y);
	GetCharacter()->AddMovementInput(GetCharacter()->GetActorRightVector(), MovementVector.X);
}

void ACTFPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
}

void ACTFPlayerController::Shoot(const FInputActionValue& Value)
{
	GetAbilitySystemComponent()->ActivateShootAbility();
}

UCTFAbilitySystemComponent* ACTFPlayerController::GetAbilitySystemComponent()
{
	if (AbilitySystemComponent == nullptr || !IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent = Cast<UCTFAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetCharacter()));
	}
	return AbilitySystemComponent;
}