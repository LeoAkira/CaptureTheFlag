// Fill out your copyright notice in the Description page of Project Settings.


#include "Flag.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CTFGameplayTags.h"
#include "Components/CapsuleComponent.h"

AFlag::AFlag()
{
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AFlag::OnOverlap);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Capsule);

	bReplicates = true;
	ReplicatedComponents.Add(Capsule);
	ReplicatedComponents.Add(Mesh);
}

void AFlag::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
}

void AFlag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!InSpawnPoint)
	{
		CurrTimeToDestroy += DeltaTime;
		if (CurrTimeToDestroy >= TimeToAutoDestroy)
		{
			HideFlag();
			OnFlagAutoDestroyed.Broadcast();
		}
	}
}

void AFlag::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UAbilitySystemComponent* ActorASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		ActorASC->AddLooseGameplayTag(FCTFGameplayTags::Get().Player_HasFlag);
		ActorASC->AddReplicatedLooseGameplayTag(FCTFGameplayTags::Get().Player_HasFlag);
		HideFlag();
	}
}

void AFlag::ShowFlag(FVector Location, bool bInSpawnPoint)
{
	//Try to reposition the flag on the floor
	FHitResult Hit;
	FVector TraceStart = Location + FVector::UpVector * 100;
	FVector TraceEnd = Location - FVector::UpVector * 100;
	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_WorldStatic))
	{
		SetActorLocation(Hit.Location);
	}
	
	Capsule->SetGenerateOverlapEvents(true);
	Mesh->SetVisibility(true);
	SetHidden(false);
	
	InSpawnPoint = bInSpawnPoint;
	CurrTimeToDestroy = 0;
}

void AFlag::HideFlag()
{
	Capsule->SetGenerateOverlapEvents(false);
	Mesh->SetVisibility(false);
	SetHidden(true);
}
