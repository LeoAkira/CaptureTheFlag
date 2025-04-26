// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFHUD.h"

#include "CTFAttributeSet.h"
#include "PlayerDisplay.h"

void ACTFHUD::InitializeHUD(ACTFPlayerController* PlayerController)
{
	UPlayerDisplay* PlayerDisplay = CreateWidget<UPlayerDisplay>(GetWorld(), PlayerDisplayClass);
	PlayerDisplay->InitializeDisplay(PlayerController);

	PlayerDisplay->AddToViewport();
}
