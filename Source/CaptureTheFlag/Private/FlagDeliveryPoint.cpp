// Fill out your copyright notice in the Description page of Project Settings.


#include "FlagDeliveryPoint.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CTFGameplayTags.h"

AFlagDeliveryPoint::AFlagDeliveryPoint()
{
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(GetRootComponent());
	Box->OnComponentBeginOverlap.AddDynamic(this, &AFlagDeliveryPoint::OnOverlap);
	SetRootComponent(Box);
}

void AFlagDeliveryPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFlagDeliveryPoint::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UAbilitySystemComponent* ActorASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		FGameplayTagContainer MatchingTags;
		MatchingTags.AddTag(TeamTag);
		MatchingTags.AddTag(FCTFGameplayTags::Get().Player_HasFlag);
		if (ActorASC->HasAllMatchingGameplayTags(MatchingTags))
		{
			OnFlagDelivered.Broadcast(TeamTag);
		}
	}
}

