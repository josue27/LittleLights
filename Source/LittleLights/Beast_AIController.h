// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Beast_AIController.generated.h"

/**
 * 
 */
UCLASS()
class LITTLELIGHTS_API ABeast_AIController : public AAIController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay()override;
public:
	virtual void Tick(float DeltaSeconds)override;
private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;
};
