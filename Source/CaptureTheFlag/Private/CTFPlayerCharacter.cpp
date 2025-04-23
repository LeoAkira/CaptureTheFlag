// Copyright Epic Games, Inc. All Rights Reserved.

#include "CTFPlayerCharacter.h"

#include "CTFAbilitySystemComponent.h"
#include "CTFAttributeSet.h"
#include "CTFHUD.h"
#include "CTFPlayerController.h"
#include "CTFPlayerState.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

//////////////////////////////////////////////////////////////////////////
// ACaptureTheFlagCharacter

ACTFPlayerCharacter::ACTFPlayerCharacter()
{
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->SetupAttachment(FirstPersonCameraComponent);
	FirstPersonMesh->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	GetMesh()->SetOwnerNoSee(true);
	
	WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	WeaponComponent->SetupAttachment(GetCapsuleComponent());
	WeaponComponent->SetRelativeLocation(FVector::ZeroVector);
}

void ACTFPlayerCharacter::PossessedBy(AController* NewController)
{
	//Server Init
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	if (HasAuthority())
	{
		Cast<UCTFAbilitySystemComponent>(AbilitySystemComponent)->AddFireAbility(FireAbility);
	}
}

void ACTFPlayerCharacter::OnRep_PlayerState()
{
	//Client Init
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
}

FVector ACTFPlayerCharacter::GetMuzzleLocation()
{
	return WeaponComponent->GetSocketLocation(MuzzleSocketName);
}

FRotator ACTFPlayerCharacter::GetMuzzleRotation()
{
	return FirstPersonCameraComponent->GetComponentRotation();
}

void ACTFPlayerCharacter::PlayFireMontage()
{
	if (IsLocallyControlled())
	{
		FirstPersonMesh->GetAnimInstance()->Montage_Play(FirstPersonFireMontage);
	}
	else
	{
		MulticastFireMontage();
	}
}

UAbilitySystemComponent* ACTFPlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACTFPlayerCharacter::MulticastFireMontage_Implementation()
{
	GetMesh()->GetAnimInstance()->Montage_Play(ThirdPersonFireMontage);
}

void ACTFPlayerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	if (IsLocallyControlled())
	{
		WeaponComponent->AttachToComponent(FirstPersonMesh, AttachmentRules, WeaponGripSocketName);
	}
	else
	{
		WeaponComponent->AttachToComponent(GetMesh(), AttachmentRules, WeaponGripSocketName);
	}
	
}

//////////////////////////////////////////////////////////////////////////// Input

void ACTFPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACTFPlayerCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACTFPlayerCharacter::Look);

		//Shooting
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ACTFPlayerCharacter::Shoot);
	}
}

void ACTFPlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ACTFPlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACTFPlayerCharacter::Shoot(const FInputActionValue& Value)
{
	Cast<UCTFAbilitySystemComponent>(AbilitySystemComponent)->ActivateFireAbility();
}

void ACTFPlayerCharacter::InitAbilityActorInfo()
{
	ACTFPlayerState* CTFPlayerState = GetPlayerState<ACTFPlayerState>();
	AbilitySystemComponent = CTFPlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(CTFPlayerState, this);
	AttributeSet = CTFPlayerState->GetAttributeSet();
	if (ACTFPlayerController* PlayerController = Cast<ACTFPlayerController>(GetController()))
	{
		if (ACTFHUD* HUD = Cast<ACTFHUD>(PlayerController->GetHUD()))
		{
			//Initialize HUD
		}
	}
	InitializeAttributes();
}

void ACTFPlayerCharacter::InitializeAttributes()
{
	Cast<UCTFAttributeSet>(AttributeSet)->SetHealth(100);
}
