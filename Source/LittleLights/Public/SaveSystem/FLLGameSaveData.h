#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Tottem_Piece.h"
#include "FLLGameSaveData.generated.h"


UENUM(BlueprintType)
enum class ELLMapsIndexEntry : uint8
{
	TutorialA,
	TutorialB,
	TutorialC,//Entrance golem base
	Lobby,//Golem base
	Level1,//Totem A
	Level2,//Totem B
	Level3,//Totem C
	LAST
};

USTRUCT(BlueprintType)
struct FLLMapListInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	ELLMapsIndexEntry MapID = ELLMapsIndexEntry::TutorialA;
	
	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	TSoftObjectPtr<UWorld> Map;

	
};

ENUM_RANGE_BY_VALUES(ELLMapsIndexEntry,ELLMapsIndexEntry::TutorialA,ELLMapsIndexEntry::Level3);

USTRUCT(BlueprintType)
struct FLLGameSaveData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	ELLMapsIndexEntry LevelsCompleted ;//Witch level user has completed
	
	UPROPERTY(BlueprintReadWrite)
	ELLMapsIndexEntry InLevel = ELLMapsIndexEntry::TutorialA;//Witch level should we load;

	//The initial movement the character makes, if completed we should not trigger this and tell the GM of the level
	UPROPERTY(BlueprintReadWrite)
	bool InitialEntranceCompleted;

	UPROPERTY(BlueprintReadWrite)
	FVector PlayerLocation;

	UPROPERTY(BlueprintReadWrite)
	FVector Totem;

	//Progreso medio nivel: piezas entregadas en el nivel en curso
	UPROPERTY(BlueprintReadWrite)
	TArray<TottemPieceType> DeliveredPieces;

	//true cuando hay un nivel en curso que reanudar
	UPROPERTY(BlueprintReadWrite)
	bool bHasMidLevelProgress = false;

	//Orbe: luz restante normalizada 0..1 (para reanudar)
	UPROPERTY(BlueprintReadWrite)
	float OrbRemainingDelta = 1.0f;

	//Antorcha encendida
	UPROPERTY(BlueprintReadWrite)
	bool bTorchLit = false;

	FLLGameSaveData()
	{
		LevelsCompleted = ELLMapsIndexEntry::TutorialA;
		InLevel = ELLMapsIndexEntry::TutorialA;
		InitialEntranceCompleted = false;
		PlayerLocation = FVector::Zero();
		Totem = FVector::Zero();
		DeliveredPieces.Empty();
		bHasMidLevelProgress = false;
		OrbRemainingDelta = 1.0f;
		bTorchLit = false;
	}
};