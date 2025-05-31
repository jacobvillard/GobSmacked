#include "EOSLoginHelper.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystem.h"


void UEOSLoginHelper::LoginWithDeviceId(bool bCreateNew)
{
	UE_LOG(LogTemp, Warning, TEXT("EOS DEBUG: Starting LoginWithDeviceId"));

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get(FName(TEXT("EOS")));
	if (!OnlineSub)
	{
		UE_LOG(LogTemp, Error, TEXT("EOS DEBUG: OnlineSubsystem is NULL! Check if EOS is properly set as DefaultPlatformService."));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("EOS DEBUG: OnlineSubsystem found: %s"), *OnlineSub->GetSubsystemName().ToString());

	IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
	if (!Identity.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("EOS DEBUG: Identity interface is NOT valid. EOS plugin may not be initialized correctly."));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("EOS DEBUG: Identity interface is valid"));

	FOnlineAccountCredentials Credentials;
	Credentials.Type = "DeviceId";

	// Important: "persistent" is required by EOS for DeviceId login
	Credentials.Id = TEXT("persistent");
	Credentials.Token = TEXT("");

	UE_LOG(LogTemp, Log, TEXT("EOS DEBUG: Credentials set -> Type: %s | Id: %s | Token: %s"),
		*Credentials.Type, *Credentials.Id, *Credentials.Token);

	Identity->OnLoginCompleteDelegates->AddLambda([](int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
	{
		if (bWasSuccessful)
		{
			UE_LOG(LogTemp, Log, TEXT("EOS DEBUG: Login successful! LocalUserNum: %d | UserId: %s"), LocalUserNum, *UserId.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("EOS DEBUG: Login FAILED. LocalUserNum: %d | Error: %s"), LocalUserNum, *Error);
		}
	});

	UE_LOG(LogTemp, Log, TEXT("EOS DEBUG: Calling Identity->Login()"));
	const bool bStartedLogin = Identity->Login(0, Credentials);

	if (!bStartedLogin)
	{
		UE_LOG(LogTemp, Error, TEXT("EOS DEBUG: Identity->Login() returned FALSE immediately! Check EOS settings or credentials."));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("EOS DEBUG: Login initiated successfully, waiting for callback..."));
	}
}