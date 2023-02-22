// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LL_PlayerControllerBase.generated.h"

class UUserWidget;
class ALLGame_HUD;
class ULL_WorldUserWidget;
/**
 * 
 */
UCLASS()
class LITTLELIGHTS_API ALL_PlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

protected:
		UPROPERTY(EditDefaultsOnly, Category = "LL | UI")
			ALLGame_HUD* LL_GameHUD;

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

	UFUNCTION(BlueprintCallable, Category = " LL | Player Controller")
		void UpdateHUDBeastDistance(float deltaDistance);

	UPROPERTY(EditDefaultsOnly, Category = "LL | UI")
		TSubclassOf<ULL_WorldUserWidget> DefaultWidgetClass;
	UPROPERTY()
		ULL_WorldUserWidget* InteractionWidgetInstance = nullptr;
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool UsingGamePad;

	UFUNCTION(BlueprintCallable)
		void ShowKeyToPressUI(FString keymsg,AActor* ActorToAttach);
	UFUNCTION(BlueprintCallable)
		void RemoveKeyToPressUI();

	
};
