// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"

#include "Kismet/GameplayStatics.h"

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	LobbyDisplay = CreateWidget<ULobbyDisplay>(GetWorld(), LobbyDisplayClass);
	LobbyDisplay->OnHostButtonClicked.AddDynamic(this, &ALobbyGameMode::HostGame);
	LobbyDisplay->OnJoinButtonClicked.AddDynamic(this, &ALobbyGameMode::JoinGame);
	LobbyDisplay->AddToViewport();

	SessionController = GetWorld()->SpawnActor<AOnlineSessionController>(AOnlineSessionController::StaticClass());
	SessionController->OnHostGameFailed.AddDynamic(this, &ALobbyGameMode::OnHostGameFailed);
	SessionController->OnJoinGameFailed.AddDynamic(this, &ALobbyGameMode::OnJoinGameFailed);
	SessionController->OnSessionJoined.AddDynamic(this, &ALobbyGameMode::OnSessionJoined);
}

void ALobbyGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	Cast<APlayerController>(NewPlayer)->SetInputMode(FInputModeUIOnly());
}

void ALobbyGameMode::HostGame()
{
	if (SessionController->HostGame())
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(GameLevelName), true, "listen");
	}
}

void ALobbyGameMode::JoinGame()
{
	if (SessionController->JoinGame())
	{
		LobbyDisplay->JoiningGame();
	}
}

void ALobbyGameMode::OnHostGameFailed(FString Reason)
{
	LobbyDisplay->HostGameFailed(Reason);
}

void ALobbyGameMode::OnJoinGameFailed(FString Reason)
{
	LobbyDisplay->JoinGameFailed(Reason);
}

void ALobbyGameMode::OnSessionJoined()
{
	GetGameInstance()->ClientTravelToSession(0, NAME_GameSession);
}
