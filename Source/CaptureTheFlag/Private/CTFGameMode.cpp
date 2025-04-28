// Copyright Epic Games, Inc. All Rights Reserved.

#include "CTFGameMode.h"

#include "CTFAbilitySystemComponent.h"
#include "CTFGameplayTags.h"
#include "CTFPlayerController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "OnlineSessionController.h"

void ACTFGameMode::SpawnPlayer(ACTFPlayerController* Controller)
{
	APawn* Pawn = Controller->GetPawn();
	if (Pawn) Pawn->DisableInput(Controller);

	ATeamBase* TeamBase = *TeamBases.Find(Controller->TeamTag);
	ACharacter* Character = GetWorld()->SpawnActor<ACharacter>(PlayerCharacterClass, TeamBase->GetRandomSpawnPoint());
	Character->SetActorHiddenInGame(true);

	//Adding a delay before Possess so character can be properly initialized before possessing
	FTimerDelegate TimerDelegate;

	TimerDelegate.BindLambda([Controller, Character, Pawn]{
		if (Pawn) Pawn->Destroy();
		Controller->Possess(Character);
		Character->SetActorHiddenInGame(false);
	});
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, .2f, false);
}

void ACTFGameMode::RespawnPlayer(ACTFPlayerController* Controller, bool SkipTimer)
{
	//Ignore Respawn after while ending match
	if (MatchState != MatchState::WaitingToStart && MatchState != MatchState::InProgress) return;
	if (SkipTimer)
	{
		SpawnPlayer(Controller);
		return;
	}
	
	const float RespawnTime = MatchState == MatchState::InProgress ? PlayerRespawnTime : 0.2f;

	//Avoids double respawning
	RespawningPlayers.Add(Controller);
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this, Controller]{
		if (RespawningPlayers.Contains(Controller))
		{
			SpawnPlayer(Controller);
			RespawningPlayers.Remove(Controller);
		}
	});
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, RespawnTime, false);
}

void ACTFGameMode::RespawnFlag()
{
	FlagController->StartFlagRespawn();
}

void ACTFGameMode::EndGame()
{
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this]{
		GetWorld()->SpawnActor<AOnlineSessionController>(AOnlineSessionController::StaticClass())->EndSession();
		UGameplayStatics::OpenLevel(GetWorld(), "Lobby");
	});
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, TimeToEnd, false);
}

void ACTFGameMode::BeginPlay()
{
	Super::BeginPlay();
	FlagController = GetWorld()->SpawnActor<AFlagController>(FlagControllerClass);
}

void ACTFGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	for (FGameplayTag Team : TeamTags)
	{
		TeamsPlayerCount.Add(Team, 0);
	}
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATeamBase::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		ATeamBase* Base = Cast<ATeamBase>(Actor);
		TeamBases.Add(Base->GetTeamTag(), Base);
	}
}

void ACTFGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (ACTFPlayerController* PlayerController = Cast<ACTFPlayerController>(NewPlayer))
	{
		PlayerController->OnPlayerDied.AddDynamic(this, &ACTFGameMode::HandlePlayerDeath);
		FGameplayTag TeamTag = GetTeamWithLessPlayers();
		TeamsPlayerCount[TeamTag]++;
		PlayerController->TeamTag = TeamTag;
		SpawnPlayer(PlayerController);
	}

	if (GetMatchState() == MatchState::WaitingToStart)
	{
		if (NumPlayers >= MinPlayersToStart)
		{
			if (!GetWorld()->GetTimerManager().TimerExists(StartMatchTimerHandle))
			{
				FTimerDelegate MatchStartTimerDelegate;
				MatchStartTimerDelegate.BindLambda([this]{
					StartMatch();
				});
				GetWorld()->GetTimerManager().SetTimer(StartMatchTimerHandle, MatchStartTimerDelegate, TimeToStart, false);
			}
		}
	}
}

void ACTFGameMode::Logout(AController* Exiting)
{
	if (ACTFPlayerController* PlayerController = Cast<ACTFPlayerController>(Exiting))
	{
		FGameplayTag TeamTag = PlayerController->TeamTag;
		TeamsPlayerCount[TeamTag]--;
	}

	if (GetMatchState() == MatchState::WaitingToStart)
	{
		if (NumPlayers <= MinPlayersToStart)
		{
			if (GetWorld()->GetTimerManager().TimerExists(StartMatchTimerHandle))
			{
				GetWorld()->GetTimerManager().ClearTimer(StartMatchTimerHandle);
			}
		}
	}
	
	Super::Logout(Exiting);
}

void ACTFGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACTFPlayerController::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		RespawnPlayer(Cast<ACTFPlayerController>(Actor), true);
	}
	RespawnFlag();
}

void ACTFGameMode::HandleMatchHasEnded()
{
	EndGame();
	Super::HandleMatchHasEnded();
}

bool ACTFGameMode::ReadyToStartMatch_Implementation()
{
	return false;
}

void ACTFGameMode::HandlePlayerDeath(ACTFPlayerController* PlayerController)
{
	if (PlayerController->GetAbilitySystemComponent()->HasMatchingGameplayTag(FCTFGameplayTags::Get().Player_HasFlag))
	{
		FlagController->SpawnFlagAt(PlayerController->GetPawn()->GetActorLocation());
	}
	
	RespawnPlayer(PlayerController);
}


FGameplayTag ACTFGameMode::GetTeamWithLessPlayers()
{
	FGameplayTag Team = TeamTags[0];
	int Count = TeamsPlayerCount[Team];
	for (int i = 1; i < TeamTags.Num(); i++)
	{
		int TeamCount = TeamsPlayerCount[TeamTags[i]];
		if (TeamCount < Count)
		{
			Team = TeamTags[i];
			Count = TeamCount;
		}
	}
	return Team;
}
