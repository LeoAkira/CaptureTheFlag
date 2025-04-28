// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "TeamData.generated.h"

USTRUCT(BlueprintType)
struct FTeamDataInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TeamTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName TeamName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor TeamColor;
	
};

/**
 * 
 */
UCLASS(Blueprintable)
class CAPTURETHEFLAG_API UTeamData : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	bool FindDataTeamInfo(FGameplayTag TeamTag, FTeamDataInfo& OutDataTeamInfo);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FTeamDataInfo> TeamsInfos;
};
