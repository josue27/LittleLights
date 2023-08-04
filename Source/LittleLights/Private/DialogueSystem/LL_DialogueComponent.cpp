// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueSystem/LL_DialogueComponent.h"

#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "LittleLights/LL_PlayerControllerBase.h"

// Sets default values for this component's properties
ULL_DialogueComponent::ULL_DialogueComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	
	DialogueStatus = LL_DialogueState::D_NONE;
}


// Called when the game starts
void ULL_DialogueComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULL_DialogueComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULL_DialogueComponent::StartDialogue()
{
	ALL_PlayerControllerBase* PC = Cast<ALL_PlayerControllerBase>(UGameplayStatics::GetPlayerController(GetWorld(),0));

	if(DialogueStatus == LL_DialogueState::D_Started)
	{
		
		if(PC)
		{
			//Necessary?
			if(InDialogueIndex < TempDialogues.Num() )
			{
				PC->ShowDialogue(TempDialogues[InDialogueIndex], true);
				InDialogueIndex++;
				return;
			}
			
			PC->ShowDialogue(FText::FromString("[Closing....]"), false);
			
			
		}
		else
		{
			PC->ShowDialogue(FText::FromString("[Closing....]"), false);

		}
		InDialogueIndex = 0;
		DialogueStatus = LL_DialogueState::D_Ended;
		TempDialogues.Empty();
		return;
	}
	
	DialogueStatus = LL_DialogueState::D_Started;
	
	if(PC)
	{
		
		if(DialoguesDT && !Dialogue_ID.IsEmpty())
		{
			TArray<FName> DialogueRows = DialoguesDT->GetRowNames();
			
			for(auto& Row : DialogueRows)
			{
				FLL_Dialogue* DialogueData = DialoguesDT->FindRow<FLL_Dialogue>(Row,"",true);
				if(DialogueData->ID == Dialogue_ID)
				{
					TempDialogues = DialogueData->DialogueLines;
				}
			}
			//Check if we hava something in the TempDialogues
			if(TempDialogues.Num() > 0)
			{
				PC->ShowDialogue(TempDialogues[InDialogueIndex], true);
				InDialogueIndex++;
			}
			else
			{
				
				PC->ShowDialogue(FText::FromString("[DUMMY DIALOGUE]"), true);
			}
		}
	}
	
	
}

