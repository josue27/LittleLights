// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "LL_TargetPoint.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class LocationType:uint8
{
	StartPoint = 0 UMETA(DisplayName = "Start Point"),
	ExitPoint = 1 UMETA(DisplayName = "Exit Point"),
	SpecialPoint = 2 UMETA(DisplayName = "Special point")
	
};
UCLASS()
class LITTLELIGHTS_API ALL_TargetPoint : public ATargetPoint
{
	GENERATED_BODY()

	protected:
	ALL_TargetPoint();

	
	
};
