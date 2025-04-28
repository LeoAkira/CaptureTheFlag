// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionFailedSignature, FString, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSessionJoinedSignature);

UCLASS()
class CAPTURETHEFLAG_API AOnlineSessionController : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnSessionFailedSignature OnHostGameFailed;
	
	UPROPERTY(BlueprintAssignable)
	FOnSessionFailedSignature OnJoinGameFailed;

	UPROPERTY(BlueprintAssignable)
	FOnSessionJoinedSignature OnSessionJoined;
	
	bool HostGame() const;
	bool JoinGame();
};
