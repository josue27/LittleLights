// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoorBase.h"
#include "Level_Manager_Base.h"
#include "AI/LL_AIBeast.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/GameModeBase.h"
#include "LL_GameModeBase.generated.h"

/**
 * 
 */


UCLASS()
class LITTLELIGHTS_API ALL_GameModeBase : public AGameModeBase, public ILL_GameplayInterface
{
	GENERATED_BODY()

protected:
	ALL_GameModeBase();

	UPROPERTY()
	ALevel_Manager_Base* LevelManager;

	UPROPERTY(BlueprintReadWrite,Category="Door")
	ADoorBase* FinalDoor;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Test")
	float TestB;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="AIBeast")
	TSubclassOf<UUserWidget> BeastState_WidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* BeastState_Instance;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Time")
	float TimeToBeast = 20.0f;//time until the beast is spawned since level started

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	bool StartWithDecayLight;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="AIBeast")
	UEnvQuery* SpawnBeastLocationQuery;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="UI")
	TSubclassOf<UUserWidget> TotemCompleted_WidgetClass;

	
	UPROPERTY(BlueprintReadWrite,Category="UI")
	UUserWidget* TotemCompleted_WidgetInstance;

	UPROPERTY(BlueprintReadWrite,Category="Player")
	APlayerCharacter* Player;
	FTimerHandle SpawnBeastTimerHandler;
	///////////

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void StartSequence();

	virtual void StartPlay() override;
	

	
	UFUNCTION()
	void StartBeastTimer();

	UFUNCTION()
	void OnLocationQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	

	UFUNCTION()
	void RemoveWidget();
	
	virtual void BeaconCompleted_Implementation() override;

	public:
	UFUNCTION(Exec)
	void SpawnBeast_Debug();
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="AIBeast")
	TSubclassOf<AActor> BeastAI_Class;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AIBeast")
	void SpawnBeast();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category="AIBeast")
	AActor* BeastAI;

	


	


	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="GameMode")
	void TottemCompleted();


	UFUNCTION(BlueprintCallable,Category="AIBeast")
	float DeltaDistanceToBeast();

	
};
