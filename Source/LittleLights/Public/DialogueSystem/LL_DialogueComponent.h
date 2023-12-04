// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "LL_DialogueComponent.generated.h"


class UDataTable;

UENUM(BlueprintType)
enum class LL_DialogueState : uint8
{
	D_Started,
	D_Ended,
	D_NONE
};
UENUM(BlueprintType)
enum class ELL_CharacterID : uint8
{
	CI_Lucas = 0 UMETA(DisplayName = "Lucas"),
	CI_Golem1 = 1 UMETA(DisplayName = "Golem1"),
	CI_Golem2 = 2 UMETA(DisplayName = "Golem2"),
	CI_Bestia = 3 UMETA(DisplayName = "Bestia"),
};


USTRUCT(BlueprintType)
struct FLL_DialogueLineStruct  
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite,  meta = (MultiLine="true"))
	FText Dialogue;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UTexture2D* SpeakerA_Portrait;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ESlateVisibility SpeakerA_Visibility;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UTexture2D* SpeakerB_Portrait;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ESlateVisibility SpeakerB_Visibility;
	
};

USTRUCT(BlueprintType)
struct FLL_Dialogue : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString ID;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FLL_DialogueLineStruct> DialogueLines;
	
};

USTRUCT(BlueprintType)
struct FLL_DialogueSpeakers : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ELL_CharacterID CharacterID;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText DisplayName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UTexture2D* Portrait;
	
	
	
};




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LITTLELIGHTS_API ULL_DialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULL_DialogueComponent();

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="LL Dialogue")
	FString Dialogue_ID;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UDataTable* DialoguesDT;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	LL_DialogueState DialogueStatus;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int32 InDialogueIndex = 0;

	UPROPERTY(BlueprintReadOnly)
	TArray<FLL_DialogueLineStruct> TempDialogues;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void StartDialogue();


	
		
};
