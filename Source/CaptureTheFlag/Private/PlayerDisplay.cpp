// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDisplay.h"

void UPlayerDisplay::InitializeDisplay(ACTFPlayerController* PlayerController)
{
	PlayerController->OnHealthChanged.AddDynamic(this, &UPlayerDisplay::OnHealthChanged);
}
