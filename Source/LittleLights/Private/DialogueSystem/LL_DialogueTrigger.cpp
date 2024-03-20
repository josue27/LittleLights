// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueSystem/LL_DialogueTrigger.h"

#include "LLGamePlayFunctionLibrary.h"
#include "Components/BoxComponent.h"


// Sets default values
ALL_DialogueTrigger::ALL_DialogueTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	ActivateUsing = ELLDialogueActivationType::ShowNextDialogue;
	LevelSequence= nullptr;
}

void ALL_DialogueTrigger::ActivateObject_Implementation()
{
	ILL_GameplayInterface::ActivateObject_Implementation();
	bEnabled = true;
}

void ALL_DialogueTrigger::ActivateObjectsOnList_Implementation()
{
	ILL_GameplayInterface::ActivateObjectsOnList_Implementation();
	
	for (auto ActorItem: ActivableItems)
	{
		ILL_GameplayInterface::Execute_ActivateObject(ActorItem);
	}
}

// Called when the game starts or when spawned
void ALL_DialogueTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALL_DialogueTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

