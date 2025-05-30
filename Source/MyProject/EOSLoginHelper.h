#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EOSLoginHelper.generated.h"

UCLASS()
class UEOSLoginHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Logs in to EOS using Device ID (anonymous login).
	 * @param bCreateNew - Whether to create a new account if one doesn't exist.
	 */
	UFUNCTION(BlueprintCallable, Category = "EOS|Login")
	static void LoginWithDeviceId(bool bCreateNew);
};
