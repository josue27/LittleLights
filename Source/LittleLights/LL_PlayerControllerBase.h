// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/LL_CrouchCross_Ability.h"
#include "GameFramework/PlayerController.h"
#include "LL_PlayerControllerBase.generated.h"

class UUserWidget;
class ALLGame_HUD;
class ULL_WorldUserWidget;
class ULL_ArrowInputWidget;
class UInputMappingContext;
class UInputAction;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueOver);
UCLASS()
class LITTLELIGHTS_API ALL_PlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

protected:
		UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "LL | UI")
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
	void TogglePauseMenu(const FInputActionValue& Value);
	
	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable, Category = " LL | Player Controller")
		void UpdateHUDBeastDistance(float deltaDistance);

	UPROPERTY(EditDefaultsOnly, Category = "LL | UI")
		TSubclassOf<ULL_WorldUserWidget> DefaultWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "LL | UI")
		TSubclassOf<ULL_ArrowInputWidget> ArrowPressWidgetClass;
	UPROPERTY()
		ULL_WorldUserWidget* InteractionWidgetInstance = nullptr;
	UPROPERTY()
		ULL_ArrowInputWidget* ArrowWidgetInstance = nullptr;
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool UsingGamePad;

	UFUNCTION(BlueprintCallable)
		void ShowKeyToPressUI(FString keymsg,AActor* ActorToAttach);
	UFUNCTION(BlueprintCallable)
		void RemoveKeyToPressUI();
	UFUNCTION(BlueprintCallable)
		void ShowKeyWithTimeToPressUI(FString keymsg,AActor* ActorToAttach,float TimeRemainng);

	UFUNCTION(BlueprintCallable)
		void ShowArrowToPressUI(LLEInputDirection KeyToPress, AActor* ActorToAttach);
	UFUNCTION(BlueprintCallable)
		void RemoveArrowToPressUI();
	UFUNCTION(BlueprintCallable)
		void ShowArrowWithTimeToPressUI(LLEInputDirection KeyPressed, AActor* ActorToAttach, float TimeRemainng);
		
	UFUNCTION(BlueprintCallable)
		void ShowTotemPiecesHUD(bool bShow);

	UFUNCTION(BlueprintCallable)
		void TotemPiecesDeliveredHUD();

	UFUNCTION(BlueprintCallable)
	void ShowDialogue(FLL_DialogueLineStruct DialogueStruct, bool bShow);
	UFUNCTION(BlueprintCallable)
	void ShowNextDialogue();
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void ShowBeastPresenceImg(bool bShow);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void DialogueEnded();


	UPROPERTY(BlueprintAssignable,BlueprintCallable)
	FOnDialogueOver OnDialogueLinesOver;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LLPLayer | EnhancedInput")
	UInputMappingContext* InputMapping;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,  Category = "LLPLayer | EnhancedInput")
	UInputAction* Pause_IA;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,  Category = "LLPLayer | EnhancedInput")
	UInputAction* AdvanceDialogue_IA;
};
