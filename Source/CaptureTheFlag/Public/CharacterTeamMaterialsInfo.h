// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "CharacterTeamMaterialsInfo.generated.h"

USTRUCT(BlueprintType)
struct FCharacterTeamMaterialsInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag TeamTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UMaterialInstance*> TeamMaterials;
};

/**
 * 
 */
UCLASS()
class CAPTURETHEFLAG_API UCharacterTeamMaterials : public UDataAsset
{
	GENERATED_BODY()
public:
	bool FindCharacterTeamMaterials(FGameplayTag TeamTag, TArray<UMaterialInstance*>& OutCharacterTeamMaterials);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FCharacterTeamMaterialsInfo> CharacterTeamsMaterials;
};
