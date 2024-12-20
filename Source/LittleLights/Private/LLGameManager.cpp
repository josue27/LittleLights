﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "LLGameManager.h"
#include "Kismet/GameplayStatics.h"


void ULLGameManager::LevelCompleted(ELLMapsIndexEntry InLevel)
{
	 GameSave.LevelsCompleted = InLevel;
}

void ULLGameManager::LevelChanged(ELLMapsIndexEntry InLevel)
{
	GameSave.InLevel = InLevel;
}

void ULLGameManager::SaveGame()
{
	if(ULLCustomSaveGame* LLSaveGame = Cast<ULLCustomSaveGame>(UGameplayStatics::CreateSaveGameObject(ULLCustomSaveGame::StaticClass())))
	{
		//We should equal the data I guess?
		//LLSaveGame->GameData = GameSave;
		
		LLSaveGame->GameData.LevelsCompleted = GameSave.LevelsCompleted;
		LLSaveGame->GameData.InLevel = GameSave.InLevel;
		FAsyncSaveGameToSlotDelegate SaveDelegate;
		SaveDelegate.BindLambda([this , LLSaveGame](const FString& SlotName, const int32 UserIndex, bool bSuccess)
		{
			if(bSuccess)
			{
				UE_LOG(LogTemp,Warning,TEXT("Saved Game Successfull"));
				OnSaveGameCompleted.Broadcast();
			}
			else
			{
				UE_LOG(LogTemp,Warning,TEXT("Saved Game Error"));

			}
		});

		UGameplayStatics::AsyncSaveGameToSlot(LLSaveGame,"LLSaveGame",0,SaveDelegate);
	}
}

void ULLGameManager::LoadGame()
{
	if (USaveGame* LoadedSaveGame = UGameplayStatics::LoadGameFromSlot("LLSaveGame", 0))
	{
		if (ULLCustomSaveGame* LoadedGenericSave = Cast<ULLCustomSaveGame>(LoadedSaveGame))
		{
			GameSave = LoadedGenericSave->GameData;
			InLevelCompleted = LoadedGenericSave->GameData.LevelsCompleted;
			CurrentLevel = LoadedGenericSave->GameData.InLevel;
			OnLoadGameCompleted.Broadcast(true);
		}
	}
	else // If load not succeeded
	{
		OnLoadGameCompleted.Broadcast(false);
		UE_LOG(LogTemp,Warning,TEXT("Load Game Error"));

	}
	
}
