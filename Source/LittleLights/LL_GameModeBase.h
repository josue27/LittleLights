// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LL_GameModeBase.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EGameState : uint8
{
	GameInit,
	Playing,
	LevelCompleted,
	LevelEnded
};
UCLASS()
class LITTLELIGHTS_API ALL_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:
	ALL_GameModeBase();
	
};
