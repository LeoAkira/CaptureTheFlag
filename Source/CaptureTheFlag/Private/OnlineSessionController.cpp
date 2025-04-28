// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineSessionController.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"

bool AOnlineSessionController::HostGame() const
{
	if (const IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld()))
	{
		if (const IOnlineSessionPtr OnlineSession = OnlineSubsystem->GetSessionInterface())
		{
			ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
			FOnlineSessionSettings SessionSettings = FOnlineSessionSettings();
			SessionSettings.bAllowJoinInProgress = true;
			SessionSettings.bShouldAdvertise = true;
			SessionSettings.bIsLANMatch = true;
			SessionSettings.NumPublicConnections = 8;
			if (OnlineSession->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionSettings))
			{
				return true;
			}
			else
			{
				OnHostGameFailed.Broadcast("Host Session Failed: CreateSession failed");
			}
		}
		else
		{
			OnHostGameFailed.Broadcast("Host Session Failed: Invalid OnlineSession");
		}
	}
	else
	{
		OnHostGameFailed.Broadcast("Host Session Failed: Invalid OnlineSubsystem");
	}
	return false;
}

bool AOnlineSessionController::JoinGame()
{
	if (const IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld()))
	{
		if (const IOnlineSessionPtr OnlineSession = OnlineSubsystem->GetSessionInterface())
		{
			ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
			TSharedRef<FOnlineSessionSearch> SessionSearch = MakeShareable(new FOnlineSessionSearch());
			SessionSearch->bIsLanQuery = true;
			SessionSearch->TimeoutInSeconds = 30.f;
			OnlineSession->OnFindSessionsCompleteDelegates.AddLambda([this, SessionSearch, OnlineSubsystem, LocalPlayer](const bool bWasSuccessful)
			{
				if (bWasSuccessful)
				{
					for (FOnlineSessionSearchResult Result : SessionSearch->SearchResults)
					{
						if (Result.Session.NumOpenPublicConnections > 0)
						{
							if (OnlineSubsystem->GetSessionInterface()->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, Result))
							{
								OnSessionJoined.Broadcast();
							}
							else
							{
								OnJoinGameFailed.Broadcast("Join Session Failed");
							}
						}
						else
						{
							OnJoinGameFailed.Broadcast("Join Session Failed: All Sessions full");
						}
					}
				}
				else
				{
					OnJoinGameFailed.Broadcast("Join Session Failed: Search Failed");
				}	
			});
			
			if (OnlineSession->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch))
			{
				return true;
			}
			else
			{
				OnJoinGameFailed.Broadcast("Join Session Failed: No Session Found");
			}
		}
		else
		{
			OnJoinGameFailed.Broadcast("Join Session Failed: Invalid OnlineSession");
		}
	}
	else
	{
		OnJoinGameFailed.Broadcast("Join Session Failed: Invalid OnlineSubsystem");
	}
	return false;
}

void AOnlineSessionController::EndSession()
{
	Online::GetSubsystem(GetWorld())->GetSessionInterface()->EndSession(NAME_GameSession);
}
