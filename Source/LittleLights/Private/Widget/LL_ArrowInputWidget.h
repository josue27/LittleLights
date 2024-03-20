// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/LL_WorldUserWidget.h"
#include "LL_ArrowInputWidget.generated.h"

/**
 * 
 */
UCLASS()
class LITTLELIGHTS_API ULL_ArrowInputWidget : public ULL_WorldUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetArrow(FKey KeyPressed);
};
