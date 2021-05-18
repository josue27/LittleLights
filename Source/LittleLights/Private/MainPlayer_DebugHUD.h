// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DebugHUD.h"
#include "MainPlayer_DebugHUD.generated.h"

/**
 * 
 */
UCLASS()
class LITTLELIGHTS_API AMainPlayer_DebugHUD : public ADebugHUD
{
	GENERATED_BODY()
protected:
	virtual void DrawHUD() override;

};
