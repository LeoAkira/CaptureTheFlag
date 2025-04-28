// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyDisplay.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLobbyButtonClickedSignature);

/**
 * 
 */
UCLASS()
class CAPTURETHEFLAG_API ULobbyDisplay : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintCallable)
	FOnLobbyButtonClickedSignature OnHostButtonClicked;

	UPROPERTY(BlueprintCallable)
	FOnLobbyButtonClickedSignature OnJoinButtonClicked;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void JoiningGame();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HostGameFailed(const FString& Reason);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void JoinGameFailed(const FString& Reason);
};
