// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tottem_Piece.generated.h"

UCLASS()
class LITTLELIGHTS_API ATottem_Piece : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATottem_Piece();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bTaken = false;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
