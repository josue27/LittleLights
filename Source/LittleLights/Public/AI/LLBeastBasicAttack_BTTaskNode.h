// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "LLBeastBasicAttack_BTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class LITTLELIGHTS_API ULLBeastBasicAttack_BTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
	protected:
	virtual  EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
public:
	
	ULLBeastBasicAttack_BTTaskNode();
	
};
