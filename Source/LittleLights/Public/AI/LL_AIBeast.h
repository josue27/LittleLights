// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LL_AIBeast.generated.h"

class UPawnSensingComponent;
class APlayerCharacter;
class AAIController;
UCLASS()
class LITTLELIGHTS_API ALL_AIBeast : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALL_AIBeast();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



	UPROPERTY(EditAnywhere,Category="AI")
	FName TargetKeyName ="TargetPlayer";	

	

	UFUNCTION()
		void PlayerSeen(APawn* PlayerPawn);
	UFUNCTION()
	void PlayerHear(APawn* PlayerPawn,  const FVector& Location, float Volume);

	UFUNCTION()
	void SetTarget(AActor* Actor);

	
	UFUNCTION()
	void ResetTarget(AActor* Actor);

	UFUNCTION(BlueprintNativeEvent)
	void PlayerEvation(APawn* PlayerPawn);
	
	UPROPERTY()
	AActor* TargetActorTemp;

	virtual void PostInitializeComponents() override;

	//Reference of PlayerCharacter, set on BeginPlay
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		APlayerCharacter* LLPlayer;
	
	UPROPERTY()
		AAIController* AIC;

	UFUNCTION(BlueprintCallable)
	void UserStartedInteraction(AActor* ActorInteractor,bool bSlowTime);
	UFUNCTION(BlueprintCallable)
	void UserFinishedInteraction(AActor* ActorInteractor,bool bSlowTime);
public:	

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "LLBeast")
		bool bIsAttacking;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "LLBeast")
	int32 MaxTeleports = 3;
	int32 CurrentTeleports = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "LLBeast")
	float SlowTimeOnInteraction = 0.2f;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void BasicAttackSequence();

	//Resets the CurrentTeleports for the beast to 0, preferably call it when beast is not longer chasing the player
	UFUNCTION(BlueprintCallable)
	void ResetTeleports();

	UFUNCTION()
	void UserObstacleCompletedEvent(bool bSuccess);
	/**
	 * @brief 
	 * @param ActorInteracting  usually it should be the player PlayerCharater
	 * @param bSlowTime should we slow time, usually affects the Beast
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OrbIsEmpty(AActor* ActorInteracting,bool bSlowTime);
	
	virtual void Destroyed() override;
	
};

