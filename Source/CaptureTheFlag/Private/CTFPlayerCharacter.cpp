// Copyright Epic Games, Inc. All Rights Reserved.

#include "CTFPlayerCharacter.h"

#include "CTFAbilitySystemComponent.h"
#include "CTFAttributeSet.h"
#include "CTFGameplayTags.h"
#include "CTFHUD.h"
#include "CTFPlayerController.h"
#include "CTFPlayerState.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"

//////////////////////////////////////////////////////////////////////////
// ACaptureTheFlagCharacter

ACTFPlayerCharacter::ACTFPlayerCharacter()
{
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetMesh(), WeaponGripSocketName);
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.0f, 10.0f, 160.0f));
	FirstPersonCameraComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

	WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));

	FlagComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Flag"));
	FlagComponent->SetupAttachment(RootComponent);
	FlagComponent->SetVisibility(false);
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
	FRotator Rotation = FirstPersonCameraComponent->GetComponentRotation();
	Rotation.Pitch = GetCameraPitch();
	return Rotation;
}

void ACTFPlayerCharacter::PlayShootMontage()
{
	GetMesh()->GetAnimInstance()->Montage_Play(ShootMontage);
}

UAbilitySystemComponent* ACTFPlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

float ACTFPlayerCharacter::GetCameraPitch()
{
	return CameraPitch;
}

void ACTFPlayerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	WeaponComponent->AttachToComponent(GetMesh(), AttachmentRules, WeaponGripSocketName);
}

void ACTFPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (IsLocallyControlled())
	{
		SetCameraPitch(Controller->GetControlRotation().Pitch);
	}
}

void ACTFPlayerCharacter::OnGameplayTagCountChanged(FGameplayTag Tag, int32 Count)
{
	if (FCTFGameplayTags::Get().Player_HasFlag.MatchesTag(Tag))
	{
		FlagComponent->SetVisibility(Count > 0);
	}
}

void ACTFPlayerCharacter::InitAbilityActorInfo()
{
	if (IsLocallyControlled())
	{
		for (FName BoneName : BonesToHideInFirstPerson)
		{
			GetMesh()->HideBoneByName(BoneName, PBO_MAX);
		}
	}
	
	ACTFPlayerState* CTFPlayerState = GetPlayerState<ACTFPlayerState>();
	AbilitySystemComponent = CTFPlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(CTFPlayerState, this);
	AttributeSet = CTFPlayerState->GetAttributeSet();
	Cast<UCTFAttributeSet>(AttributeSet)->SetHealth(MaxHealth);
	
	if (ACTFPlayerController* CTFPlayerController = Cast<ACTFPlayerController>(GetController()))
	{
		if (ACTFHUD* HUD = Cast<ACTFHUD>(CTFPlayerController->GetHUD()))
		{
			HUD->InitializeHUD(CTFPlayerController, CTFPlayerState);
		}
		AbilitySystemComponent->AddLooseGameplayTag(CTFPlayerController->TeamTag);
		AbilitySystemComponent->AddReplicatedLooseGameplayTag(CTFPlayerController->TeamTag);
	}
	FGameplayTag HasTag = FCTFGameplayTags::Get().Player_HasFlag;
	AbilitySystemComponent->RegisterGameplayTagEvent(HasTag).AddUObject(this, &ACTFPlayerCharacter::OnGameplayTagCountChanged);
}

void ACTFPlayerCharacter::SetCameraPitch_Implementation(float NewValue)
{
	SetCameraPitchMulticast(NewValue);
}

void ACTFPlayerCharacter::SetCameraPitchMulticast_Implementation(float NewValue)
{
	CameraPitch = NewValue > 180 ? NewValue - 360 : NewValue;
}
