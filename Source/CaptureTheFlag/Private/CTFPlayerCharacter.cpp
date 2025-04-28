// Copyright Epic Games, Inc. All Rights Reserved.

#include "CTFPlayerCharacter.h"

#include "CTFAbilitySystemComponent.h"
#include "CTFAttributeSet.h"
#include "CTFGameplayTags.h"
#include "CTFHUD.h"
#include "CTFPlayerController.h"
#include "CTFPlayerState.h"
#include "FlagController.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

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
	WeaponComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FlagComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Flag"));
	FlagComponent->SetupAttachment(RootComponent);
	FlagComponent->SetVisibility(false);

	bReplicates = true;
	ReplicatedComponents.Add(GetMesh());
}

void ACTFPlayerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	WeaponComponent->AttachToComponent(GetMesh(), AttachmentRules, WeaponGripSocketName);
}

void ACTFPlayerCharacter::PossessedBy(AController* NewController)
{
	//Server Init
	Super::PossessedBy(NewController);
	InitializeCharacter();
	InitializeController();

	if (HasAuthority())
	{
		Cast<UCTFAbilitySystemComponent>(AbilitySystemComponent)->AddShootAbility(ShootAbility);
	}
}

void ACTFPlayerCharacter::OnRep_PlayerState()
{
	//Client Init
	Super::OnRep_PlayerState();

	if (GetPlayerState() != nullptr)
	{
		InitializeCharacter();
		InitializeController();
	}
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
	if (Tag.MatchesTagExact(FCTFGameplayTags::Get().Player_HasFlag))
	{
		FlagComponent->SetVisibility(Count > 0);
	}
}

void ACTFPlayerCharacter::CharacterDeath_Implementation()
{
	if (Died) return;
	Died = true;
	//Ragdoll
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	WeaponComponent->SetSimulatePhysics(true);
	WeaponComponent->SetEnableGravity(true);
	WeaponComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	FlagComponent->SetVisibility(false);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACTFPlayerCharacter::InitializeCharacter()
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
	AbilitySystemComponent->RegisterGenericGameplayTagEvent().AddUObject(this, &ACTFPlayerCharacter::OnGameplayTagCountChanged);
	AttributeSet = CTFPlayerState->GetAttributeSet();
	UCTFAttributeSet* CTFAttributeSet = Cast<UCTFAttributeSet>(AttributeSet);
	CTFAttributeSet->SetHealth(MaxHealth);
	
	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(CTFAttributeSet->GetHealthAttribute()).AddLambda
	(
		[this](const FOnAttributeChangeData& Data)
		{
			if (Data.NewValue <= 0)
			{
				CharacterDeath();
			}
		}
	);
}

void ACTFPlayerCharacter::InitializeController()
{
	if (ACTFPlayerController* CTFPlayerController = Cast<ACTFPlayerController>(GetController()))
	{
		if (ACTFHUD* HUD = Cast<ACTFHUD>(CTFPlayerController->GetHUD()))
		{
			HUD->InitializeHUD(CTFPlayerController);
		}
		AbilitySystemComponent->AddLooseGameplayTag(CTFPlayerController->TeamTag);
		AbilitySystemComponent->AddReplicatedLooseGameplayTag(CTFPlayerController->TeamTag);
		SetTeamMaterials(CTFPlayerController->TeamTag);
	}
}

void ACTFPlayerCharacter::SetTeamMaterials_Implementation(FGameplayTag TeamTag)
{
	TArray<UMaterialInstance*> Materials;
	if (TeamMaterials->FindCharacterTeamMaterials(TeamTag, Materials))
	{
		for (int i = 0; i < Materials.Num(); i++)
		{
			GetMesh()->SetMaterial(i, Materials[i]);
		}
	}
}

void ACTFPlayerCharacter::OnHealthChanged_Implementation(float NewValue)
{
	if (NewValue <= 0)
	{
		if (IsLocallyControlled())
		{
			DisableInput(Cast<APlayerController>(GetController()));
		}
		
		CharacterDeath();
	}
}

void ACTFPlayerCharacter::SetCameraPitch_Implementation(float NewValue)
{
	SetCameraPitchMulticast(NewValue);
}

void ACTFPlayerCharacter::SetCameraPitchMulticast_Implementation(float NewValue)
{
	CameraPitch = NewValue > 180 ? NewValue - 360 : NewValue;
}
