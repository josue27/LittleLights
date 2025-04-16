// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tottem.generated.h"

class APersonaje;
class ATottem_Piece;

USTRUCT(BlueprintType)
struct FTottemPieceSettings
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Tottem Settigs")
	FName PieceMeshName;
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Tottem Settigs")
	bool bColocated;
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Tottem Settigs")
	FTransform MeshPostion;
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Tottem Settigs")
	ATottem_Piece* TottemPiece = nullptr;

	FTottemPieceSettings()
	{
		bColocated = false;
	}
};

UCLASS()
class LITTLELIGHTS_API ATottem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATottem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


//VARS
public:
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Tottem Settigs")
		int32 FragmentsRequired;
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Tottem Settigs")
		int32 FragmentsInserted;
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Tottem Settigs")
		APersonaje* Player = nullptr;
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Tottem Settigs")
		FVector OffsetPiecesLocation;
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Tottem Settigs")
		float AcumulatedDelay = 0.0f;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
