// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EOSGameInstance.generated.h"

/** BP delegate: true on success, false on failure */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyCreated, FName, SessionName);

/**
 * 
 */
UCLASS()
class MYPROJECT_API UEOS_C_GI : public UGameInstance
{
 GENERATED_BODY()

public:
 UFUNCTION(BlueprintCallable, Category = "EOS|Session")
 void CreateSessionWithName(const FString& LobbyName, int32 MaxConnections = 4);

 UPROPERTY(BlueprintAssignable, Category = "EOS|Session")
 FOnLobbyCreated OnLobbyCreated;

 UFUNCTION(BlueprintCallable, Category = "EOS|Session")
 FString GetLobbyNameFromResult(const FBlueprintSessionResult& Result);

 UFUNCTION(BlueprintCallable, Category = "EOS|Session")
 FString GetLobbyTypeFromResult(const FBlueprintSessionResult& Result);

 UFUNCTION(BlueprintCallable, Category = "EOS|Session")
 FString GetLobbyCodeFromResult(const FBlueprintSessionResult& Result);
};
