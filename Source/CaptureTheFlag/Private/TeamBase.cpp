// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CTFGameplayTags.h"

ATeamBase::ATeamBase()
{
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(GetRootComponent());
	Box->OnComponentBeginOverlap.AddDynamic(this, &ATeamBase::OnOverlap);
	SetRootComponent(Box);
}

FTransform ATeamBase::GetRandomSpawnPoint()
{
	int Points = SpawnPoints.Num();
	if (Points > 0)
	{
		return SpawnPoints[FMath::RandRange(0, Points-1)];
	}
	else
	{
		FTransform Transform = GetTransform();
		Transform.SetLocation(FMath::RandPointInBox(Box->Bounds.GetBox()));
		return Transform;
	}
}

void ATeamBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATeamBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UAbilitySystemComponent* ActorASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		FGameplayTag HasFlag = FCTFGameplayTags::Get().Player_HasFlag;
		FGameplayTagContainer MatchingTags;
		MatchingTags.AddTag(TeamTag);
		MatchingTags.AddTag(HasFlag);
		if (ActorASC->HasAllMatchingGameplayTags(MatchingTags))
		{
			ActorASC->RemoveLooseGameplayTag(HasFlag);
			OnFlagDelivered.Broadcast(TeamTag);
		}
	}
}

