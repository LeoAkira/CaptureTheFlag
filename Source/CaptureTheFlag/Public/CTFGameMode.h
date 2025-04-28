// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FlagController.h"
#include "GameplayTagContainer.h"
#include "TeamBase.h"
#include "GameFramework/GameMode.h"
#include "CTFGameMode.generated.h"

class ACTFPlayerController;
class AFlag;

UCLASS(minimalapi)
class ACTFGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SpawnPlayer(ACTFPlayerController* Controller);

	UFUNCTION(BlueprintCallable)
	void RespawnPlayer(ACTFPlayerController* Controller);

	UFUNCTION(BlueprintCallable)
	void RespawnFlag();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetPointsToWin() const { return PointsToWin; }

	UFUNCTION(BlueprintCallable)
	void EndGame();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FGameplayTag> GetTeamTags() { return TeamTags; }
	
protected:
	virtual void BeginPlay() override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void HandleMatchIsWaitingToStart() override;
	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
	virtual void HandleMatchAborted() override;
	virtual bool ReadyToStartMatch_Implementation() override;
	
	UPROPERTY()
	TMap<FGameplayTag, ATeamBase*> TeamBases;

	UPROPERTY()
	TMap<FGameplayTag, int> TeamsPlayerCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Game")
	int PointsToWin = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Game")
	int MinPlayersToStart = 4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Player")
	TSubclassOf<ACharacter> PlayerCharacterClass ;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Player")
	TArray<FGameplayTag> TeamTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Player")
	float PlayerRespawnTime = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config | Flag")
	TSubclassOf<AFlagController> FlagControllerClass;
private:
	FGameplayTag GetTeamWithLessPlayers();

	UPROPERTY()
	TObjectPtr<AFlagController> FlagController;

	UFUNCTION()
	void HandlePlayerDeath(ACTFPlayerController* PlayerController);

	UPROPERTY()
	TArray<ACTFPlayerController*> RespawningPlayers;
};



