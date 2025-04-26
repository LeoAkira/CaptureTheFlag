// Copyright Epic Games, Inc. All Rights Reserved.

#include "CTFGameMode.h"

#include "CTFAbilitySystemComponent.h"
#include "CTFGameplayTags.h"
#include "CTFPlayerController.h"
#include "FlagSpawnPoint.h"
#include "Flag.h"
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

	//Adding a delay before Possess so character can be properly initialized first
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([Controller, Character]{
		Controller->Possess(Character);
	});
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.1f, false);
}

void ACTFGameMode::RespawnPlayer(ACTFPlayerController* Controller)
{
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
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, PlayerRespawnTime, false);
}

void ACTFGameMode::BeginPlay()
{
	Super::BeginPlay();
	FlagController = GetWorld()->SpawnActor<AFlagController>(FlagControllerClass);
	FlagController->StartFlagRespawn();
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