// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSGameInstance.h"

#include "FindSessionsCallbackProxy.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Engine/LocalPlayer.h"



#define SETTING_SERVER_NAME FName(TEXT("SERVER_NAME"))
void UEOS_C_GI::CreateSessionWithName(const FString& LobbyName, int32 MaxConnections)
{
	const IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	if (!Subsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("No OnlineSubsystem found"));
		OnLobbyCreated.Broadcast(NAME_None);
		return;
	}

	IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface invalid"));
		OnLobbyCreated.Broadcast(NAME_None);
		return;
	}

	FOnlineSessionSettings Settings;
	Settings.bIsLANMatch = false;
	Settings.NumPublicConnections = MaxConnections;
	Settings.bShouldAdvertise = true;
	Settings.bUsesPresence = true;
	Settings.bAllowJoinInProgress = true;
	Settings.bAllowJoinViaPresence = true;
	Settings.bUseLobbiesIfAvailable = true;

	Settings.Settings.Add(FName("LobbyName"), FOnlineSessionSetting(LobbyName, EOnlineDataAdvertisementType::ViaOnlineService));
	
	SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(
	FOnCreateSessionCompleteDelegate::CreateLambda([this, SessionInterface](FName SessionName, bool bWasSuccessful)
	{
		if (bWasSuccessful)
		{
			UE_LOG(LogTemp, Log, TEXT("Session_EOS created: %s"), *SessionName.ToString());

			// ✅ Must call StartSession only after successful session creation
			SessionInterface->AddOnStartSessionCompleteDelegate_Handle(
				FOnStartSessionCompleteDelegate::CreateLambda([this](FName InSessionName, bool bStartSuccess)
				{
					if (bStartSuccess)
					{
						UE_LOG(LogTemp, Log, TEXT("Session_EOS started: %s"), *InSessionName.ToString());
						OnLobbyCreated.Broadcast(InSessionName);
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Session_EOS start failed"));
						OnLobbyCreated.Broadcast(NAME_None);
					}
				})
			);

			SessionInterface->StartSession(SessionName);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Session_EOS creation failed"));
			OnLobbyCreated.Broadcast(NAME_None);
		}
	})
);

	SessionInterface->CreateSession(0, NAME_GameSession, Settings);
	

	
}

FString UEOS_C_GI::GetLobbyNameFromResult(const FBlueprintSessionResult& Result)
{
	FString OutName;
	if (Result.OnlineResult.Session.SessionSettings.Get(FName(TEXT("ServerName")), OutName))
	{
		return OutName;
	}
	return TEXT("Unknown");
}

FString UEOS_C_GI::GetLobbyTypeFromResult(const FBlueprintSessionResult& Result)
{
	FString OutName;
	if (Result.OnlineResult.Session.SessionSettings.Get(FName(TEXT("LobbyType")), OutName))
	{
		return OutName;
	}
	return TEXT("Unknown");
}

FString UEOS_C_GI::GetLobbyCodeFromResult(const FBlueprintSessionResult& Result)
{
	FString OutName;
	if (Result.OnlineResult.Session.SessionSettings.Get(FName(TEXT("LobbyCode")), OutName))
	{
		return OutName;
	}
	return TEXT("Unknown");
}


