// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LL_PlayerOverlay.generated.h"


class UProgressBar;
class UImage;
/**
 * 
 */
UCLASS()
class LITTLELIGHTS_API ULL_PlayerOverlay : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DeltaStamine;

	UPROPERTY(meta =(BindWidget))
		UImage* BeastAlertImg;


	UFUNCTION(BlueprintNativeEvent)
		void ShowTotemPieceHUD(bool bShow);
	
	UFUNCTION(BlueprintNativeEvent)
		void TotemPieceDeliveredHUD();
	UFUNCTION(BlueprintNativeEvent)
	void ShowBeastPresenceHUD(bool bShow);

	UFUNCTION(BlueprintNativeEvent)
	void ShowDialogue(bool bShow, FLL_DialogueLineStruct DialogueStruct);
};
