// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LLGamePlayFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class LITTLELIGHTS_API ULLGamePlayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	public:
	UFUNCTION(BlueprintCallable,Category="Gameplay")
	static bool IsPlayerAlive(AActor* PlayerActor);
	
};