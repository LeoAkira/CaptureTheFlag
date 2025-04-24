// Fill out your copyright notice in the Description page of Project Settings.


#include "Flag.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CTFGameplayTags.h"
#include "Components/CapsuleComponent.h"

AFlag::AFlag()
{
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Capsule);
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AFlag::OnOverlap);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
}

void AFlag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!InSpawnPoint)
	{
		TimeToAutoDestroy -= DeltaTime;
		if (TimeToAutoDestroy <= 0.f)
		{
			OnFlagAutoDestroyed.Broadcast();
			Destroy();
		}
	}
}

void AFlag::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UAbilitySystemComponent* ActorASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		ActorASC->AddLooseGameplayTag(FCTFGameplayTags::Get().Player_HasFlag);
		Destroy();
	}
}
