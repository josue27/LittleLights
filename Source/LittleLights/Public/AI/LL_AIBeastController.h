// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "LL_AIBeastController.generated.h"

class UBehaviorTree;
/**
 * 
 */
UCLASS()
class LITTLELIGHTS_API ALL_AIBeastController : public AAIController
{
	GENERATED_BODY()
	protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,Category="AI")
	UBehaviorTree* BehaviorTree;
	
};
