// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "LL_BTService_CheckPlayerRange.generated.h"

/**
 * 
 */
UCLASS()
class LITTLELIGHTS_API ULL_BTService_CheckPlayerRange : public UBTService
{
	GENERATED_BODY()

	protected:
	
	UPROPERTY(EditAnywhere,Category="AI")
	FBlackboardKeySelector InRangeKey;
	UPROPERTY(EditAnywhere,Category="AI")
	float DistanceAttackRange = 500.0f;
	UPROPERTY(EditAnywhere,Category="AI")
	float DistanceToTriggerTeleport = 1000.0f;
	//Distance from player we want to look a place to teleport
	UPROPERTY(EditAnywhere,Category="AI")
	float DistanceToTeleport = 700.0f;
	//Radius that you will look for the point to teleport
	UPROPERTY(EditAnywhere,Category="AI")
	float RadiusToSpawn = 800.0f;
	UPROPERTY(VisibleAnywhere,Category="AI")
	FBlackboardKeySelector TargetKeyName ;	
	UPROPERTY(VisibleAnywhere,Category="AI")
	FBlackboardKeySelector TeleportTriggerKeyName ;	
	FTimerHandle TriggerTeleporTimer;
	

	UFUNCTION()
	//Target actor should be the player and AIPawn welp, the beast or Pawn that we control
	void TeleportToRandLocation(AActor* TargetActor,APawn* AIPawn);
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
