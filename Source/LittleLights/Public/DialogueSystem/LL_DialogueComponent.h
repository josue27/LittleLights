// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SlateWrapperTypes.h"
#include "Engine/DataTable.h"
#include "LL_DialogueComponent.generated.h"


class UDataTable;
enum class  ESlateVisibility :  uint8;

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
	CI_None = 0 UMETA(DisplayName = "None"),
	CI_Lucas = 1 UMETA(DisplayName = "Lucas"),
	CI_Golem1 = 2 UMETA(DisplayName = "Golem1"),
	CI_Golem2 = 3 UMETA(DisplayName = "Golem2"),
	CI_Bestia = 4 UMETA(DisplayName = "Bestia"),
	CI_OldGolem = 5 UMETA(DisplayName = "Old Golem"),
	CI_MAX
};

UENUM(BlueprintType)
enum class LL_SpeakerTurn : uint8
{
	Speaker_A = 0 UMETA(DisplayName = "Speaker A"),
	Speaker_B = 1 UMETA(DisplayName = "Speaker B")
};

USTRUCT(BlueprintType)
struct FLL_DialogueLine : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly,  meta = (MultiLine="true"))
	FText Dialogue;
	UPROPERTY(EditAnywhere,BlueprintReadOnly);
	LL_SpeakerTurn Speaker = LL_SpeakerTurn();
	
};

USTRUCT(BlueprintType)
struct FLL_DialogueSpeakers : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ELL_CharacterID CharacterID = ELL_CharacterID::CI_None;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText DisplayName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UTexture2D* Portrait;
	
	
	
};

USTRUCT(BlueprintType)
struct FLL_DialogueLineStruct  
{
	GENERATED_BODY()

	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ELL_CharacterID SpeakerA_Info;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ELL_CharacterID SpeakerB_Info;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,  meta = (MultiLine="true"))
	TArray<FLL_DialogueLine> DialogueLines;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ESlateVisibility SpeakerA_Visibility;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ESlateVisibility SpeakerB_Visibility;
	
	FLL_DialogueLineStruct(): SpeakerA_Info(ELL_CharacterID::CI_None),
		  SpeakerB_Info(ELL_CharacterID::CI_None),
		  SpeakerA_Visibility(ESlateVisibility::Collapsed),
		  SpeakerB_Visibility(ESlateVisibility::Collapsed)
	{
		
	}
	
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
