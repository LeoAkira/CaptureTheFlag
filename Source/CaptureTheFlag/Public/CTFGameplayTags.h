// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FCTFGameplayTags
{
public:
	static FCTFGameplayTags Get() { return GameplayTags; }
	static void InitializeGameplayTags();

	FGameplayTag Player_HasFlag;

	FGameplayTag Team;
	FGameplayTag Team_Red;
	FGameplayTag Team_Blue;
private:
	static FCTFGameplayTags GameplayTags;
};