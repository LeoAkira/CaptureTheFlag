// Copyright Epic Games, Inc. All Rights Reserved.

#include "CTFGameMode.h"

#include "CTFGameState.h"
#include "CTFHUD.h"
#include "CTFPlayerCharacter.h"
#include "CTFPlayerController.h"
#include "CTFPlayerState.h"
#include "UObject/ConstructorHelpers.h"

ACTFGameMode::ACTFGameMode(): Super()
{
	DefaultPawnClass = ACTFPlayerCharacter::StaticClass();
	HUDClass = ACTFHUD::StaticClass();
	PlayerControllerClass = ACTFPlayerController::StaticClass();
	GameStateClass = ACTFGameState::StaticClass();
	PlayerStateClass = ACTFPlayerState::StaticClass();
}
