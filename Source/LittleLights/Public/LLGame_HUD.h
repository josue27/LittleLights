// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LLGame_HUD.generated.h"

class UUserWidget;
class ULL_PlayerOverlay;
/**
 * 
 */
UCLASS()
class LITTLELIGHTS_API ALLGame_HUD : public AHUD
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "LL PlayerHUD")
		TSubclassOf<UUserWidget> PlayerOverlayClass;

	ULL_PlayerOverlay* PlayerOverlay;

	UFUNCTION(BlueprintCallable)
		void AddPlayerOverlay();


protected:
	virtual void BeginPlay() override;
};
