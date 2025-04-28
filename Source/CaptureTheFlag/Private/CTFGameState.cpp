// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFGameState.h"

#include "AbilitySystemComponent.h"
#include "CTFGameMode.h"
#include "Kismet/GameplayStatics.h"


void ACTFGameState::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
}

void ACTFGameState::AddPlayerState(APlayerState* PlayerState)
{
	Initialize();
	Super::AddPlayerState(PlayerState);
	
	//Player may have entered after first broadcast
	if (HasMatchStarted()) OnMatchStarted.Broadcast(TeamTags);
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
		OnMatchStarted.Broadcast(TeamTags);
	}
}

void ACTFGameState::OnTeamScored(FGameplayTag TeamTag)
{
	TeamPoints[TeamTag]++;
	OnScoreUpdated.Broadcast(TeamTag, TeamPoints[TeamTag]);

	if (ACTFGameMode* GameMode = Cast<ACTFGameMode>(AuthorityGameMode))
	{
		if (TeamPoints[TeamTag] >= GameMode->GetPointsToWin())
		{
			MulticastOnGameOver(TeamTag);
			GameMode->EndGame();
		}
		else
		{
			GameMode->RespawnFlag();
		}
	}
}

void ACTFGameState::Initialize()
{
	if (!Initialized)
	{
		Initialized = true;

		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATeamBase::StaticClass(), FoundActors);

		for (AActor* Actor : FoundActors)
		{
			ATeamBase* Base = Cast<ATeamBase>(Actor);
			Base->OnFlagDelivered.AddDynamic(this, &ACTFGameState::OnTeamScored);
			TeamPoints.Add(Base->GetTeamTag());
			OnScoreUpdated.Broadcast(Base->GetTeamTag(), 0);
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
