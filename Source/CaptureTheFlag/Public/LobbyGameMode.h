// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LobbyDisplay.h"
#include "GameFramework/GameMode.h"
#include "OnlineSessionController.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CAPTURETHEFLAG_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULobbyDisplay> LobbyDisplayClass;

	UPROPERTY(EditDefaultsOnly)
	FString GameLevelName;
private:
	virtual void BeginPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;

	UFUNCTION()
	void HostGame();

	UFUNCTION()
	void JoinGame();

	UFUNCTION()
	void OnHostGameFailed(FString Reason);

	UFUNCTION()
	void OnJoinGameFailed(FString Reason);

	UFUNCTION()
	void OnSessionJoined();
	
	UPROPERTY()
	ULobbyDisplay* LobbyDisplay;

	UPROPERTY()
	TObjectPtr<AOnlineSessionController> SessionController;
};
