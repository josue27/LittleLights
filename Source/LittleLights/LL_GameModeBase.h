// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Level_Manager_Base.h"
#include "GameFramework/GameModeBase.h"
#include "LL_GameModeBase.generated.h"

/**
 * 
 */


UCLASS()
class LITTLELIGHTS_API ALL_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	ALL_GameModeBase();

	UPROPERTY()
	ALevel_Manager_Base* LevelManager;

	UFUNCTION(BlueprintNativeEvent)
	void StartSequence();

	UFUNCTION(BlueprintNativeEvent)
	void TottemCompleted();
	
};
