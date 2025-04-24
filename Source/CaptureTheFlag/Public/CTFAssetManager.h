// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "CTFAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class CAPTURETHEFLAG_API UCTFAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UCTFAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
