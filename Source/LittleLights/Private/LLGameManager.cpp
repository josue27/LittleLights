// Fill out your copyright notice in the Description page of Project Settings.


#include "LLGameManager.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/DateTime.h"
#include "PlayerCharacter.h"
#include "../Torch.h"
#include "Tottem_Piece.h"
#include "LLComponents/LL_ToolsComponent.h"
#include "Engine/GameInstance.h"
#include "GameFramework/PlayerController.h"


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
		LLSaveGame->GameData = GameSave;
		LLSaveGame->SaveID = 0;
		LLSaveGame->UserIndex = 0;
		LLSaveGame->DateWhenSaveLastModified = FDateTime::Now();

		FAsyncSaveGameToSlotDelegate SaveDelegate;
		SaveDelegate.BindLambda([this , LLSaveGame](const FString& SlotName, const int32 UserIndex, bool bSuccess)
		{
			if(bSuccess)
			{
				UE_LOG(LogTemp,Warning,TEXT("Saved Game Successfull"));
				OnSaveGameCompleted.Broadcast(true);
			}
			else
			{
				UE_LOG(LogTemp,Warning,TEXT("Saved Game Error"));
				OnSaveGameCompleted.Broadcast(false);
			}
		});

		UGameplayStatics::AsyncSaveGameToSlot(LLSaveGame,"LLSaveGame",0,SaveDelegate);
	}
}

void ULLGameManager::AddDeliveredPiece(TottemPieceType PieceType)
{
	GameSave.DeliveredPieces.AddUnique(PieceType);
	GameSave.bHasMidLevelProgress = true;
}

void ULLGameManager::ClearLevelProgress()
{
	GameSave.DeliveredPieces.Empty();
	GameSave.CarriedPieces.Empty();
	GameSave.bHasMidLevelProgress = false;
	GameSave.OrbRemainingDelta = 1.0f;
	GameSave.bOrbDecaying = false;
	GameSave.bTorchLit = false;
	GameSave.bTotemDiscovered = false;
}

void ULLGameManager::SetPlayerLocation(const FVector& Location)
{
	GameSave.PlayerLocation = Location;
}

void ULLGameManager::SetInitialEntranceCompleted(bool bCompleted)
{
	GameSave.InitialEntranceCompleted = bCompleted;
}

void ULLGameManager::SetOrbState(float RemainingDelta)
{
	GameSave.OrbRemainingDelta = RemainingDelta;
}

void ULLGameManager::SetTorchState(bool bLit)
{
	GameSave.bTorchLit = bLit;
}

void ULLGameManager::SetTotemDiscovered(bool bDiscovered)
{
	GameSave.bTotemDiscovered = bDiscovered;
}

void ULLGameManager::SetCarriedPieces(const TArray<TottemPieceType>& Pieces)
{
	GameSave.CarriedPieces = Pieces;
}

void ULLGameManager::CapturePlayerProgress(APlayerCharacter* Player)
{
	if (!Player)
	{
		return;
	}

	GameSave.PlayerLocation = Player->GetActorLocation();

	GameSave.CarriedPieces.Empty();
	for (ATottem_Piece* Piece : Player->TottemPieces)
	{
		if (Piece)
		{
			GameSave.CarriedPieces.AddUnique(Piece->PieceType);
		}
	}

	if (Player->ToolsComponent)
	{
		GameSave.OrbRemainingDelta = Player->ToolsComponent->GetOrbRemainingDelta();
		GameSave.bOrbDecaying = Player->ToolsComponent->IsOrbDecaying();
	}

	if (Player->Torch)
	{
		GameSave.bTorchLit = !Player->Torch->bLightOver;
	}

	GameSave.bHasMidLevelProgress = (GameSave.DeliveredPieces.Num() > 0)
		|| (GameSave.CarriedPieces.Num() > 0)
		|| (GameSave.OrbRemainingDelta < 1.0f)
		|| GameSave.bOrbDecaying
		|| GameSave.bTorchLit
		|| GameSave.bTotemDiscovered;
}

void ULLGameManager::SaveGameAndCapturePlayer()
{
	APlayerCharacter* Player = nullptr;
	if (UGameInstance* GI = GetGameInstance())
	{
		if (APlayerController* PC = GI->GetFirstLocalPlayerController())
		{
			Player = Cast<APlayerCharacter>(PC->GetPawn());
		}
	}

	CapturePlayerProgress(Player);
	SaveGame();
}

void ULLGameManager::DeleteSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist("LLSaveGame", 0))
	{
		UGameplayStatics::DeleteGameInSlot("LLSaveGame", 0);
		UE_LOG(LogTemp, Warning, TEXT("Save game deleted."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No save game found to delete."));
	}

	//Reset in-memory state. Deleting only the disk slot leaves the stale GameSave
	//struct (this subsystem lives for the whole process) which would be re-saved
	//on the next SaveGame() call and resurrect the deleted file.
	GameSave = FLLGameSaveData();
	InLevelCompleted = ELLMapsIndexEntry::TutorialA;
	CurrentLevel = ELLMapsIndexEntry::TutorialA;
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
