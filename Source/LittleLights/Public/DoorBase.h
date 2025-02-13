// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoorBase.generated.h"

enum class ELLMapsIndexEntry : uint8;
class ATargetPoint;
class UBoxComponent;



UCLASS()
class LITTLELIGHTS_API ADoorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorBase();
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Door Base")
	ATargetPoint* EndWalkPoint;

	UPROPERTY( BlueprintReadWrite)
	UBoxComponent* EndTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Door Base")
	ELLMapsIndexEntry LevelID;//Level that corresponds to this door
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool CanOpenDoor();

//PROPERTIES
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDoorOpened = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDoorUnlocked = false;

};
