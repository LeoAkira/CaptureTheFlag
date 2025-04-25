// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TeamBase.h"
#include "GameFramework/GameMode.h"
#include "CTFGameMode.generated.h"

UCLASS(minimalapi)
class ACTFGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ACTFGameMode();

	UFUNCTION(BlueprintCallable)
	void RespawnPlayer(APlayerController* Controller, FGameplayTag TeamTag);
	
protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	
	UPROPERTY()
	TMap<FGameplayTag, ATeamBase*> TeamBases;

	UPROPERTY()
	TMap<FGameplayTag, int> TeamsPlayerCount;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACharacter> PlayerCharacterClass ;

	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> TeamTags;

private:
	FGameplayTag GetTeamWithLessPlayers();
};



