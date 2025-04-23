// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AbilitySystemComponent.h"
#include "WeaponUserInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponUserInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CAPTURETHEFLAG_API IWeaponUserInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual FVector GetMuzzleLocation() = 0;
	virtual FRotator GetMuzzleRotation() = 0;

	virtual void PlayShootMontage() = 0;
};
