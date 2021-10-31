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

	UPROPERTY(VisibleAnywhere,Category="AI")
	FBlackboardKeySelector TargetKeyName ;	

	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
