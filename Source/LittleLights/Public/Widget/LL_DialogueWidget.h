// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LL_DialogueWidget.generated.h"

/**
 * 
 */
UCLASS()
class LITTLELIGHTS_API ULL_DialogueWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	
	virtual  void NativeConstruct() override;

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="LL_Widget")
	void NextButton();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="LL_Widget")
	void EndDialog();
	
};
