// Fill out your copyright notice in the Description page of Project Settings.

#include "CTFGameplayTags.h"
#include "GameplayTagsManager.h"

FCTFGameplayTags FCTFGameplayTags::GameplayTags;

void FCTFGameplayTags::InitializeGameplayTags()
{
	GameplayTags.Player_HasFlag = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Player.HasFlag")
	);
	
	GameplayTags.Team_Red = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Team.Red")
	);
	
	GameplayTags.Team_Blue = UGameplayTagsManager::Get().AddNativeGameplayTag
	(
		FName("Team.Blue")
	);
}
