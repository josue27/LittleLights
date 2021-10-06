// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LittleLights/LL_GameModeBase.h"
#include "Level_Manager_Base.generated.h"


UENUM(BlueprintType,Blueprintable)
enum class EGameState : uint8
{
	GameInit UMETA(DisplaName="GameInit"),
	Playing  UMETA(DisplaName="Playing"),
	LevelCompleted UMETA(DisplaName="Level Completed"),
	LevelEnded UMETA(DisplaName="Level Ended")
};
UCLASS()
class LITTLELIGHTS_API ALevel_Manager_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevel_Manager_Base();
protected:

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

//PROPERTIES
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float TorchLightUpTime = 0.0f;

//FUNCTIONS
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Level Manager")
	EGameState CurrentGameState;


};
