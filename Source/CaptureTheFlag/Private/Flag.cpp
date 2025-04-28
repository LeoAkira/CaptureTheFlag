// Fill out your copyright notice in the Description page of Project Settings.


#include "Flag.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CTFAttributeSet.h"
#include "CTFGameplayTags.h"
#include "Components/CapsuleComponent.h"

AFlag::AFlag()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GetRootComponent());
	Capsule->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AFlag::OnOverlap);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Capsule);
	Mesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	bReplicates = true;
	ReplicatedComponents.Add(Mesh);
	ReplicatedComponents.Add(Capsule);
}

void AFlag::BeginPlay()
{
	Super::BeginPlay();
	
	SetReplicateMovement(true);
}

void AFlag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFlag::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UAbilitySystemComponent* ActorASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		FGameplayTag HasFlag = FCTFGameplayTags::Get().Player_HasFlag;
		if (!ActorASC->HasMatchingGameplayTag(HasFlag))
		{
			const UCTFAttributeSet* AttributeSet = Cast<UCTFAttributeSet>(ActorASC->GetAttributeSet(UCTFAttributeSet::StaticClass()));
			if (AttributeSet->GetHealth() > 0)
			{
				ActorASC->AddLooseGameplayTag(HasFlag);
				ActorASC->AddReplicatedLooseGameplayTag(HasFlag);
				HideFlag();
			}
		}
	}
}

void AFlag::ShowFlag_Implementation(FVector Location, bool bInSpawnPoint)
{
	//Try to reposition the flag on the floor
	FHitResult Hit;
	FVector TraceStart = Location + FVector::UpVector * 100;
	FVector TraceEnd = Location - FVector::UpVector * 300;
	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility))
	{
		SetActorLocation(Hit.Location);
	}
	else
	{
		SetActorLocation(Location);
	}
	
	Capsule->SetGenerateOverlapEvents(true);
	SetActorHiddenInGame(false);
}

void AFlag::HideFlag_Implementation()
{
	Capsule->SetGenerateOverlapEvents(false);
	SetActorHiddenInGame(true);
}
