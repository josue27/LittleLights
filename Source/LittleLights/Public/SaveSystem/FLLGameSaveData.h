#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FLLGameSaveData.generated.h"


UENUM(BlueprintType)
enum class ELLMapsIndexEntry : uint8
{
	TutorialA,
	TutorialB,
	Lobby,
	Level1,
	Level2,
	Level3,
	LAST
};

USTRUCT(BlueprintType)
struct FLLMapListInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	ELLMapsIndexEntry MapID;
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	TSoftObjectPtr<UWorld> Map;

	
};

ENUM_RANGE_BY_VALUES(ELLMapsIndexEntry,ELLMapsIndexEntry::TutorialA,ELLMapsIndexEntry::Level3);

USTRUCT(BlueprintType)
struct FLLGameSaveData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	ELLMapsIndexEntry LevelsCompleted;//Witch level user has completed
	
	UPROPERTY(BlueprintReadWrite)
	ELLMapsIndexEntry InLevel;//Witch level should we load;

	//The initial movement the character makes, if completed we should not trigger this and tell the GM of the level
	UPROPERTY(BlueprintReadWrite)
	bool InitialEntranceCompleted;

	UPROPERTY(BlueprintReadWrite)
	FVector PlayerLocation;

	UPROPERTY(BlueprintReadWrite)
	FVector Totem;
	
};