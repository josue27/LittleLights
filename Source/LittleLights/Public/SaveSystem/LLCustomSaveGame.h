// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FLLGameSaveData.h"
#include "GameFramework/SaveGame.h"
#include "LLCustomSaveGame.generated.h"

/**
 * 
 */

UCLASS()
class LITTLELIGHTS_API ULLCustomSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, SaveGame, Category = "Little Lights")
	int SaveID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, SaveGame, Category = "Little Lights")
	FDateTime DateWhenSaveLastModified = FDateTime();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, SaveGame, Category = "Little Lights")
	int UserIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, SaveGame, Category = "Little Lights")
	FLLGameSaveData GameData;

	
	static bool CompareByDateDescending(const ULLCustomSaveGame& A, const ULLCustomSaveGame& B)
	{
		return A.DateWhenSaveLastModified > B.DateWhenSaveLastModified;
	}
};
