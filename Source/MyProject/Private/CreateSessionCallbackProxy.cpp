// Copyright Epic Games, Inc. All Rights Reserved.


#include "CreateSessionCallbackProxyMP.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemBPCallHelperMP.h"


#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CreateSessionCallbackProxyMP)

//////////////////////////////////////////////////////////////////////////
// UCreateSessionCallbackProxy

//////////////////////////////////////////////////////////////////////////
// FOnlineSubsystemBPCallHelper

FOnlineSubsystemBPCallHelperMP::FOnlineSubsystemBPCallHelperMP(const TCHAR* CallFunctionContext, UObject* WorldContextObject, FName SystemName)
	: OnlineSub(Online::GetSubsystem(GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull), SystemName))
	, FunctionContext(CallFunctionContext)
{
	if (OnlineSub == nullptr)
	{
		FFrame::KismetExecutionMessage(*FString::Printf(TEXT("%s - Invalid or uninitialized OnlineSubsystem"), FunctionContext), ELogVerbosity::Warning);
	}
}

void FOnlineSubsystemBPCallHelperMP::QueryIDFromPlayerController(APlayerController* PlayerController)
{
	UserID.Reset();

	APlayerState* PlayerState = nullptr;
	if (PlayerController != nullptr)
	{
		PlayerState = ToRawPtr(PlayerController->PlayerState);
	}

	if (PlayerState != nullptr)
	{
		UserID = PlayerState->GetUniqueId().GetUniqueNetId();
		if (!UserID.IsValid())
		{
			FFrame::KismetExecutionMessage(*FString::Printf(TEXT("%s - Cannot map local player to unique net ID"), FunctionContext), ELogVerbosity::Warning);
		}
	}
	else
	{
		FFrame::KismetExecutionMessage(*FString::Printf(TEXT("%s - Invalid player state"), FunctionContext), ELogVerbosity::Warning);
	}
}

UCreateSessionCallbackProxyMP::UCreateSessionCallbackProxyMP(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, CreateCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateCompleted))
	, StartCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartCompleted))
	, NumPublicConnections(1)
{
}

UCreateSessionCallbackProxyMP* UCreateSessionCallbackProxyMP::CreateSession(UObject* WorldContextObject, class APlayerController* PlayerController, int32 PublicConnections, bool bUseLAN, bool bUseLobbiesIfAvailable, FString SessionName, FString LobbyType, FString LobbyCode)
{
	UCreateSessionCallbackProxyMP* Proxy = NewObject<UCreateSessionCallbackProxyMP>();
	Proxy->PlayerControllerWeakPtr = PlayerController;
	Proxy->NumPublicConnections = PublicConnections;
	Proxy->bUseLAN = bUseLAN;
	Proxy->bUseLobbiesIfAvailable = bUseLobbiesIfAvailable;
	Proxy->WorldContextObject = WorldContextObject;
	Proxy->LobbyName = SessionName;
	Proxy->LobbyType = LobbyType;
	Proxy->LobbyCode = LobbyCode;
	return Proxy;
}

void UCreateSessionCallbackProxyMP::Activate()
{
	FOnlineSubsystemBPCallHelperMP Helper(TEXT("CreateSession"), WorldContextObject);
	Helper.QueryIDFromPlayerController(PlayerControllerWeakPtr.Get());

	if (Helper.IsValid())
	{
		auto Sessions = Helper.OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			CreateCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(CreateCompleteDelegate);

			FOnlineSessionSettings Settings;
			Settings.NumPublicConnections = NumPublicConnections;
			Settings.bShouldAdvertise = true;
			Settings.bAllowJoinInProgress = true;
			Settings.bIsLANMatch = bUseLAN;
			Settings.bUsesPresence = true;
			Settings.bAllowJoinViaPresence = true;
			Settings.bUseLobbiesIfAvailable = bUseLobbiesIfAvailable;

			Settings.Settings.Add( FName(TEXT("ServerName")), FOnlineSessionSetting(FString(LobbyName), EOnlineDataAdvertisementType::ViaOnlineService));
			Settings.Settings.Add( FName(TEXT("LobbyType")), FOnlineSessionSetting(FString(LobbyType), EOnlineDataAdvertisementType::ViaOnlineService));
			Settings.Settings.Add( FName(TEXT("LobbyCode")), FOnlineSessionSetting(FString(LobbyCode), EOnlineDataAdvertisementType::ViaOnlineService));
			Settings.Set( FName(TEXT("ServerYAS")), FOnlineSessionSetting(FString(TEXT("69696TST")), EOnlineDataAdvertisementType::ViaOnlineService));



			Sessions->CreateSession(*Helper.UserID, NAME_GameSession, Settings);

			// OnCreateCompleted will get called, nothing more to do now
			return;
		}
		else
		{
			FFrame::KismetExecutionMessage(TEXT("Sessions not supported by Online Subsystem"), ELogVerbosity::Warning);
		}
	}

	// Fail immediately
	OnFailure.Broadcast();
}

void UCreateSessionCallbackProxyMP::OnCreateCompleted(FName SessionName, bool bWasSuccessful)
{
	FOnlineSubsystemBPCallHelperMP Helper(TEXT("CreateSessionCallback"), WorldContextObject);
	Helper.QueryIDFromPlayerController(PlayerControllerWeakPtr.Get());

	if (Helper.IsValid())
	{
		auto Sessions = Helper.OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(CreateCompleteDelegateHandle);
			
			if (bWasSuccessful)
			{
				StartCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(StartCompleteDelegate);
				Sessions->StartSession(NAME_GameSession);
				UE_LOG(LogTemp, Log, TEXT("🚀 EOS Session / Lobby created - ID = %d"), NAME_GameSession);

				// OnStartCompleted will get called, nothing more to do now
				return;
			}
		}
	}

	if (!bWasSuccessful)
	{
		OnFailure.Broadcast();
	}
}

void UCreateSessionCallbackProxyMP::OnStartCompleted(FName SessionName, bool bWasSuccessful)
{
	FOnlineSubsystemBPCallHelperMP Helper(TEXT("StartSessionCallback"), WorldContextObject);
	Helper.QueryIDFromPlayerController(PlayerControllerWeakPtr.Get());

	if (Helper.IsValid())
	{
		auto Sessions = Helper.OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(StartCompleteDelegateHandle);
		}
	}

	if (bWasSuccessful)
	{
		OnSuccess.Broadcast();
	}
	else
	{
		OnFailure.Broadcast();
	}
}

