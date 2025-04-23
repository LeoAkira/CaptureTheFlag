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
		Cast<UCTFAbilitySystemComponent>(AbilitySystemComponent)->AddShootAbility(ShootAbility);
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

void ACTFPlayerCharacter::PlayShootMontage()
{
	if (IsLocallyControlled())
	{
		FirstPersonMesh->GetAnimInstance()->Montage_Play(FirstPersonShootMontage);
	}
	else
	{
		MulticastShootMontage();
	}
}

UAbilitySystemComponent* ACTFPlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACTFPlayerCharacter::MulticastShootMontage_Implementation()
{
	GetMesh()->GetAnimInstance()->Montage_Play(ThirdPersonShootMontage);
}

void ACTFPlayerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
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

void ACTFPlayerCharacter::InitAbilityActorInfo()
{
	ACTFPlayerState* CTFPlayerState = GetPlayerState<ACTFPlayerState>();
	AbilitySystemComponent = CTFPlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(CTFPlayerState, this);
	AttributeSet = CTFPlayerState->GetAttributeSet();
	Cast<UCTFAttributeSet>(AttributeSet)->SetHealth(100);
	
	if (ACTFPlayerController* PlayerController = Cast<ACTFPlayerController>(GetController()))
	{
		if (ACTFHUD* HUD = Cast<ACTFHUD>(PlayerController->GetHUD()))
		{
			//Initialize HUD
		}
	}
}