// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFGameState.h"

#include "AbilitySystemComponent.h"
#include "CTFGameMode.h"
#include "TeamBase.h"
#include "Kismet/GameplayStatics.h"


void ACTFGameState::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATeamBase::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		ATeamBase* Base = Cast<ATeamBase>(Actor);
		Base->OnFlagDelivered.AddDynamic(this, &ACTFGameState::OnTeamScored);
		TeamPoints.Add(Base->GetTeamTag());
		TeamBases.Add(Base);
		OnScoreUpdated.Broadcast(Base->GetTeamTag(), 0);
	}
}

void ACTFGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
}

void ACTFGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
}

void ACTFGameState::OnRep_MatchState()
{
	Super::OnRep_MatchState();
	if (MatchState == MatchState::InProgress)
	{
		if (HasAuthority())
		{
			if (ACTFGameMode* GameMode = Cast<ACTFGameMode>(AuthorityGameMode))
			{
				MulticastOnMatchStarted(GameMode->GetTeamTags());
			}
		}
	}
}

void ACTFGameState::OnTeamScored(FGameplayTag TeamTag)
{
	TeamPoints[TeamTag]++;
	OnScoreUpdated.Broadcast(TeamTag, TeamPoints[TeamTag]);

	if (HasAuthority())
	{
		ACTFGameMode* GameMode = Cast<ACTFGameMode>(AuthorityGameMode);
		if (TeamPoints[TeamTag] >= GameMode->GetPointsToWin())
		{
			SetMatchState(MatchState::WaitingPostMatch);
			MulticastOnGameOver(TeamTag);
		}
		else
		{
			GameMode->RespawnFlag();
		}
	}
}

void ACTFGameState::MulticastOnGameOver_Implementation(FGameplayTag WinningTeam)
{
	OnGameOver.Broadcast(WinningTeam);
}

void ACTFGameState::MulticastOnMatchStarted_Implementation(const TArray<FGameplayTag>& TeamsTags)
{
	OnMatchStarted.Broadcast(TeamsTags);
}
