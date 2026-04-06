// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LittleLights/LL_GameModeBase.h"
#include "LLGamePlayFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class LITTLELIGHTS_API ULLGamePlayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	public:
	UFUNCTION(BlueprintCallable,Category="LLGameplay")
	static bool IsPlayerAlive(AActor* PlayerActor);

	UFUNCTION(BlueprintCallable,Category="LLGameplay")
	static float TimeToLocation(float Velocity, FVector Start,FVector End);

	UFUNCTION(BlueprintCallable,Category="LLGameplay")
	static  bool IsCloserToEnd(FVector PlayerPos,USplineComponent* Spline);

	UFUNCTION(BlueprintCallable,Category="LLGameplay")
	static  bool IsTotemPieceGrabable(AActor* Totempiece);

	UFUNCTION()
	static void ScreenLog(FString text,FColor Color = FColor::White);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetAppVersion"), Category = "Game Config")
	static FString GetAppVersion();
	
#pragma region Build Type info getter
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Build Info")
	static bool IsShippingBuild()
	{
#if UE_BUILD_SHIPPING
		return true;
#else
		return false;
#endif
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Build Info")
	static bool IsDevelopmentBuild()
	{
#if UE_BUILD_DEVELOPMENT
		return true;
#else
		return false;
#endif
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Build Info")
	static bool IsDebugBuild()
	{
#if UE_BUILD_DEBUG
		return true;
#else
		return false;
#endif
	}
#pragma endregion

};
