// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FlagController.h"
#include "GameplayTagContainer.h"
#include "TeamBase.h"
#include "GameFramework/GameMode.h"
#include "CTFGameMode.generated.h"

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
	
protected:
	virtual void BeginPlay() override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	
	UPROPERTY()
	TMap<FGameplayTag, ATeamBase*> TeamBases;

	UPROPERTY()
	TMap<FGameplayTag, int> TeamsPlayerCount;

	UPROPERTY(EditDefaultsOnly, Category = "Config | Player")
	TSubclassOf<ACharacter> PlayerCharacterClass ;

	UPROPERTY(EditDefaultsOnly, Category = "Config | Player")
	TArray<FGameplayTag> TeamTags;

	UPROPERTY(EditDefaultsOnly, Category = "Config | Player")
	float PlayerRespawnTime = 3.f;

	UPROPERTY(EditDefaultsOnly, Category = "Config | Flag")
	TSubclassOf<AFlagController> FlagControllerClass;
private:
	FGameplayTag GetTeamWithLessPlayers();

	UPROPERTY()
	TObjectPtr<AFlagController> FlagController;

	UFUNCTION()
	void HandlePlayerDeath(ACTFPlayerController* PlayerController);

	TArray<ACTFPlayerController*> RespawningPlayers;
};



