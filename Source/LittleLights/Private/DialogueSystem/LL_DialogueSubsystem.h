// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueSystem/LL_DialogueComponent.h"
#include "Engine/DataTable.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LL_DialogueSubsystem.generated.h"


/**
 * 
 */
UCLASS()
class LITTLELIGHTS_API ULL_DialogueSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	//TODO: We set the DT in the GMs we should do it in the base gamemode
	UPROPERTY(BlueprintReadWrite)
	UDataTable* CharactersData;
	/**
	 * @brief For the moment the idea is to get the general data for dialogues
	 */
	UFUNCTION(BlueprintCallable)
	bool GetCharacterData(const ELL_CharacterID SpeakerID, FLL_DialogueSpeakers& CharacterData);

	
};
