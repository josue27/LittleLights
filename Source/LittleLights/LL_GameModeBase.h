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

	UPROPERTY(BlueprintReadWrite,Category="LLGameMode|FinalDoor")
	ADoorBase* FinalDoor;

	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="LLGameMode|AIBeast")
	TSubclassOf<UUserWidget> BeastState_WidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "LLGameMode|AIBeast")
	UUserWidget* BeastState_Instance;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="LLGameMode|AIBeast")
	UEnvQuery* SpawnBeastLocationQuery;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="LLGameMode|Time")
	float TimeToBeast = 20.0f;//time until the beast is spawned since level started

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="LLGameMode|LightOrb")
	bool StartWithDecayLight;
	
	

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="LLGameMode|UI")
	TSubclassOf<UUserWidget> TotemCompleted_WidgetClass;

	
	UPROPERTY(BlueprintReadWrite, Category = "LLGameMode|UI")
	UUserWidget* TotemCompleted_WidgetInstance;

	UPROPERTY(BlueprintReadWrite, Category = "LLGameMode|Player")
	APlayerCharacter* Player;
	FTimerHandle SpawnBeastTimerHandler;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLGameMode|Player")
		bool EnableIntroMovement;
	
	
	///////////

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "LLGameMode")
	void StartSequence();

	virtual void StartPlay() override;
	

	
	UFUNCTION()
	void StartBeastTimer();

	UFUNCTION()
	void OnLocationQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION(BlueprintCallable)
		void GameStart();

	UFUNCTION(BlueprintCallable)
		void PlayerEndedIntroMovement();
	UFUNCTION(BlueprintCallable)
		void PlayerIntroMovement();
	

	UFUNCTION()
	void RemoveWidget();
	
	virtual void BeaconCompleted_Implementation() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="LLGameMode | Player")
		float TorchLightUpTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLGameMode | Level Manager")
		EGameState CurrentGameState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* PlayerIntroDestiny;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialParameterCollection* FogMaterialParameter;

	UFUNCTION(Exec)
	void SpawnBeast_Debug();
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "LLGameMode|AIBeast")
	TSubclassOf<AActor> BeastAI_Class;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "LLGameMode|AIBeast")
	void SpawnBeast();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LLGameMode|AIBeast")
	AActor* BeastAI;

	UFUNCTION(BlueprintCallable)
		void LevelCompleted();

	UFUNCTION(BlueprintImplementableEvent)
		void LightUpMoon();




	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="LLGameMode|GameMode")
	void TottemCompleted();


	UFUNCTION(BlueprintCallable,Category="LLGameMode|AIBeast")
	float DeltaDistanceToBeast();

	UFUNCTION(Exec)
		void StartOrbDecay();
};
