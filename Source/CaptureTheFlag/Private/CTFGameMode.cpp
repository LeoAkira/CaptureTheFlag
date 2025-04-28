// Copyright Epic Games, Inc. All Rights Reserved.

#include "CTFGameMode.h"

#include "CTFAbilitySystemComponent.h"
#include "CTFGameplayTags.h"
#include "CTFPlayerController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

void ACTFGameMode::SpawnPlayer(ACTFPlayerController* Controller)
{
	if (APawn* Pawn = Controller->GetPawn())
	{
		Pawn->Destroy();
	}
	
	ATeamBase* TeamBase = *TeamBases.Find(Controller->TeamTag);
	ACharacter* Character = GetWorld()->SpawnActor<ACharacter>(PlayerCharacterClass, TeamBase->GetRandomSpawnPoint());
	Character->SetActorHiddenInGame(true);

	//Adding a delay before Possess so character can be properly initialized before possessing
	FTimerDelegate TimerDelegate;

	TimerDelegate.BindLambda([Controller, Character]{
		Controller->Possess(Character);
		Character->SetActorHiddenInGame(false);
	});
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, .5f, false);
}

void ACTFGameMode::RespawnPlayer(ACTFPlayerController* Controller)
{
	//Ignore Respawn after while ending match
	if (MatchState != MatchState::WaitingToStart && MatchState != MatchState::InProgress) return;

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
	EndMatch();
}

void ACTFGameMode::BeginPlay()
{
	Super::BeginPlay();
	FlagController = GetWorld()->SpawnActor<AFlagController>(FlagControllerClass);
}

void ACTFGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	for (FGameplayTag Tag : TeamTags)
	{
		TeamsPlayerCount.Add(Tag, 0);
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
		PlayerController->SetInputMode(FInputModeGameOnly());
		FGameplayTag TeamTag = GetTeamWithLessPlayers();
		PlayerController->TeamTag = TeamTag;
		TeamsPlayerCount[TeamTag]++;
		SpawnPlayer(PlayerController);

		PlayerController->OnPlayerDied.AddDynamic(this, &ACTFGameMode::HandlePlayerDeath);
	}
}

void ACTFGameMode::Logout(AController* Exiting)
{
	if (ACTFPlayerController* PC = Cast<ACTFPlayerController>(Exiting))
	{
		FGameplayTag TeamTag = PC->TeamTag;
		TeamsPlayerCount[TeamTag]--;
	}
	Super::Logout(Exiting);
}

void ACTFGameMode::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();
}

void ACTFGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACTFPlayerController::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		RespawnPlayer(Cast<ACTFPlayerController>(Actor));
	}
	RespawnFlag();
}

void ACTFGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
	
}

void ACTFGameMode::HandleMatchAborted()
{
	Super::HandleMatchAborted();
	
}

bool ACTFGameMode::ReadyToStartMatch_Implementation()
{
	if (GetMatchState() == MatchState::WaitingToStart)
	{
		if (NumPlayers >= MinPlayersToStart)
		{
			return true;
		}
	}
	return false;
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

void ACTFGameMode::HandlePlayerDeath(ACTFPlayerController* PlayerController)
{
	if (PlayerController->GetAbilitySystemComponent()->HasMatchingGameplayTag(FCTFGameplayTags::Get().Player_HasFlag))
	{
		FlagController->SpawnFlagAt(PlayerController->GetPawn()->GetActorLocation());
	}
	
	RespawnPlayer(PlayerController);
}