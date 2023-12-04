// Fill out your copyright notice in the Description page of Project Settings.


#include "LL_DialogueSubsystem.h"

#include "DialogueSystem/LL_DialogueComponent.h"

void ULL_DialogueSubsystem::GetCharacterData(const ELL_CharacterID SpeakerID, FLL_DialogueSpeakers& CharacterData)
{
	FLL_DialogueSpeakers RetrievedData;
	if(CharactersData)
	{
		TArray<FName> RowNames = CharactersData->GetRowNames();
		for(auto Row : RowNames)
		{
			FLL_DialogueSpeakers* Data = CharactersData->FindRow<FLL_DialogueSpeakers>(Row,"",true);
			if(Data->CharacterID == SpeakerID)
			{
				CharacterData = *Data;
				break;
			}
		}
	}
	
}
