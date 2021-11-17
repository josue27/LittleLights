// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LL_PlayerControllerBase.generated.h"

class UUserWidget;
/**
 * 
 */
UCLASS()
class LITTLELIGHTS_API ALL_PlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

	protected:


	UPROPERTY(EditDefaultsOnly,Category="UI")
	TSubclassOf<UUserWidget> PauseMenuClass;

	UPROPERTY()
	UUserWidget* PauseMenuInstance;
	///////////	//////////////////
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category="Player Controller")
	void DisableAllInput();

	UFUNCTION(BlueprintCallable, Category="Player Controller")
	void ChangeToGameInput();

	UFUNCTION(BlueprintCallable, Category="Player Controller")
	void ChangeToUIInput();

	UFUNCTION(BlueprintCallable,Category="Player Controller")
	void TogglePauseMenu();
	
	virtual void SetupInputComponent() override;
};
