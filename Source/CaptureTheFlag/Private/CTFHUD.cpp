// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFHUD.h"
#include "PlayerDisplay.h"

void ACTFHUD::InitializeHUD(ACTFPlayerController* PlayerController, ACTFPlayerState* PlayerState)
{
	CTFPlayerController = PlayerController;
	CTFPlayerState = PlayerState;

	UPlayerDisplay* PlayerDisplay = CreateWidget<UPlayerDisplay>(GetWorld(), PlayerDisplayClass);
	PlayerDisplay->InitializeDisplay(PlayerController, PlayerState);

	PlayerDisplay->AddToViewport();
}
