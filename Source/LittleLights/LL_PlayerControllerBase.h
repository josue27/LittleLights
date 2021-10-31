// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LL_PlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class LITTLELIGHTS_API ALL_PlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

	protected:
	
	virtual void BeginPlay() override;
	
};
