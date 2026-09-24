// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveSystem/LLCustomSaveGame.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LLGameManager.generated.h"

struct FLLGameSaveData;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadGameCompleted,bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSaveGameCompleted);
UCLASS()
class LITTLELIGHTS_API ULLGameManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category="LL | SaveSystem")
	FLLGameSaveData GameSave;

	UPROPERTY(BlueprintAssignable)
	FOnLoadGameCompleted OnLoadGameCompleted;

	UPROPERTY(BlueprintAssignable)
	FOnSaveGameCompleted OnSaveGameCompleted;

	UFUNCTION(BlueprintCallable)
	void LevelCompleted(ELLMapsIndexEntry InLevel);

	UFUNCTION(BlueprintCallable)
	void LevelChanged(ELLMapsIndexEntry InLevel);
	
	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame();

	UFUNCTION(BlueprintCallable)
	void DeleteSaveGame();

	UFUNCTION(BlueprintCallable)
	void AddDeliveredPiece(TottemPieceType PieceType);

	UFUNCTION(BlueprintCallable)
	void ClearLevelProgress();

	UFUNCTION(BlueprintCallable)
	void SetPlayerLocation(const FVector& Location);

	UFUNCTION(BlueprintCallable)
	void SetInitialEntranceCompleted(bool bCompleted);

	UFUNCTION(BlueprintCallable)
	void SetOrbState(float RemainingDelta);

	UFUNCTION(BlueprintCallable)
	void SetTorchState(bool bLit);

	UPROPERTY(BlueprintReadOnly)
	ELLMapsIndexEntry InLevelCompleted;

	UPROPERTY(BlueprintReadOnly)
	ELLMapsIndexEntry CurrentLevel;

	
};
