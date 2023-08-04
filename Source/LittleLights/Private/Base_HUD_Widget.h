// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Base_HUD_Widget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class UBase_HUD_Widget : public UUserWidget
{
	GENERATED_BODY()

public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DeltaStamine;
	
	/**
	 * @brief 
	 */
	UPROPERTY(EditAnywhere, meta = (Bindwidget))
		UProgressBar* BeastAlertImg;

	////

	UFUNCTION(BlueprintCallable)
		void UpdateBeastDistance(float distance);
};
