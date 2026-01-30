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
	/**Level that corresponds to this door*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Door Base")
	ELLMapsIndexEntry LevelID;
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
	/**
	 * If it is a door in the golem base set it to true, otherwise is false to avoid the user to be able to finish the level
	 * by accident
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDoorUnlocked = false;

};
