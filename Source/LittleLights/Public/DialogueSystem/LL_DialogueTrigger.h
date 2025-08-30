// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LL_GameplayInterface.h"
#include "GameFramework/Actor.h"
#include "LL_DialogueTrigger.generated.h"

UENUM(BlueprintType)
enum class ELLDialogueActivationType : uint8
{
	None = 0,
	Timeline = 1 UMETA(DisplayName = "Time Line"),
	ShowPanel = 2 UMETA(DisplayName = "Show Panel"),
	JumpToTime = 3 UMETA (DisplayName = "Jump to Time"),
	ShowNextDialogue = 4 UMETA (DisplayName = "Show Next Dialogue"),
	ShowSpecificDialogue = 5 UMETA (DisplayName = "Show Specific Dialogue"),
	BeaconCompletion = 6 UMETA(DisplayName ="Beacon completion"),
	MAX
};

class ALevelSequenceActor;
class UBoxComponent;

UCLASS()
class LITTLELIGHTS_API ALL_DialogueTrigger : public AActor, public ILL_GameplayInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALL_DialogueTrigger();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UBoxComponent* TriggerCollider;
	
	//Should this react?
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="LL Dialogue")
	bool bEnabled;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="LL Dialogue")
	ALevelSequenceActor* LevelSequence = nullptr;

	//Text to display when interacting, do we need this?
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="LL Dialogue")
	FText TextToDisplay;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="LL Dialogue")
	ELLDialogueActivationType ActivateUsing;

	//Should we try to keep playing the level sequence after closing dialogue
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="LL Dialogue")
	bool bShouldPlayAfterClose;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="LL Dialogue")
	bool bPauseTimeLine;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="LL Dialogue")
	bool bDestroyAfterActivation;
	
	//if using ELLDialogueActivationType::JumpToTime this is the time we jump in the sequence
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="LL Dialogue")
	float TimeInSequence = 0.f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="LL Dialogue")
	int32 DialogueToDisplay = -1;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="LL Dialogue")
	int32 PanelToDisplay = 0;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="LL Dialogue")
	TArray<AActor*> ActivableItems;

	/////////-------------------------------------

	virtual void ActivateObject_Implementation() override;
	virtual void ActivateObjectsOnList_Implementation() override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
