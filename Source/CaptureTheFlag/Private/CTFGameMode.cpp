// Copyright Epic Games, Inc. All Rights Reserved.

#include "CTFGameMode.h"

/*
#include "CTFGameState.h"
#include "CTFHUD.h"
#include "CTFPlayerCharacter.h"
#include "CTFPlayerController.h"
#include "CTFPlayerState.h"
*/
#include "CTFPlayerController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ACTFGameMode::ACTFGameMode(): Super()
{
	/*
	DefaultPawnClass = ACTFPlayerCharacter::StaticClass();
	HUDClass = ACTFHUD::StaticClass();
	PlayerControllerClass = ACTFPlayerController::StaticClass();
	GameStateClass = ACTFGameState::StaticClass();
	PlayerStateClass = ACTFPlayerState::StaticClass();
	*/
}

void ACTFGameMode::RespawnPlayer(APlayerController* Controller, FGameplayTag TeamTag)
{
	ATeamBase* Base = *TeamBases.Find(TeamTag);
	ACharacter* Character = GetWorld()->SpawnActor<ACharacter>(PlayerCharacterClass, Base->GetRandomSpawnPoint());

	//Adding a delay before Possess so character can be properly initialized first
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([Controller, Character]{
		Controller->Possess(Character);
	});
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.1f, false);
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

	if (ACTFPlayerController* PC = Cast<ACTFPlayerController>(NewPlayer))
	{
		FGameplayTag TeamTag = GetTeamWithLessPlayers();
		PC->TeamTag = TeamTag;
		TeamsPlayerCount[TeamTag]++;
		RespawnPlayer(PC, TeamTag);
	}

	UE_LOG(LogTemp, Warning, TEXT("New Player, Teams:"));
	for (FGameplayTag Tag : TeamTags)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: %d"), *Tag.ToString(), TeamsPlayerCount[Tag]);
	}
}

void ACTFGameMode::Logout(AController* Exiting)
{
	if (ACTFPlayerController* PC = Cast<ACTFPlayerController>(Exiting))
	{
		FGameplayTag TeamTag = PC->TeamTag;
		TeamsPlayerCount[TeamTag]--;
	}

	UE_LOG(LogTemp, Warning, TEXT("Player Exited, Teams:"));
	for (FGameplayTag Tag : TeamTags)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: %d"), *Tag.ToString(), TeamsPlayerCount[Tag]);
	}
	
	Super::Logout(Exiting);
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
