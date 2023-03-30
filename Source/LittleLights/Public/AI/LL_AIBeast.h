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
	
	UPROPERTY()
	AActor* TargetActorTemp;

	virtual void PostInitializeComponents() override;

	UPROPERTY()
		APlayerCharacter* LLPlayer;
	
	UPROPERTY()
		AAIController* AIC;
public:	

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "LLBeast")
		bool bIsAttacking;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void BasicAttackSequence();



};

