// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFAssetManager.h"

#include "CTFAbilitySystemComponent.h"
#include "CTFGameplayTags.h"

UCTFAssetManager& UCTFAssetManager::Get()
{
	UCTFAssetManager* CTFAssetManager = Cast<UCTFAssetManager>(GEngine->AssetManager);
	return *CTFAssetManager;
}

void UCTFAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FCTFGameplayTags::InitializeGameplayTags();
}
