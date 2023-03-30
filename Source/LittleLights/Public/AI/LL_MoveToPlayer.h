// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "LL_MoveToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class LITTLELIGHTS_API ULL_MoveToPlayer : public UBTTask_MoveTo
{
	GENERATED_BODY()
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	UPROPERTY(VisibleAnywhere, Category = "AI")
		FBlackboardKeySelector TargetKeyName;
};
