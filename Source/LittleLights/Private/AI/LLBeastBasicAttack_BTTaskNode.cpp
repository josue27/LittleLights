// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/LLBeastBasicAttack_BTTaskNode.h"

#include "AIController.h"
#include "LLGamePlayFunctionLibrary.h"
#include "PlayerCharacter.h"
#include "AI/LL_AIBeast.h"
#include "BehaviorTree/BlackboardComponent.h"
ULLBeastBasicAttack_BTTaskNode::ULLBeastBasicAttack_BTTaskNode()
{
}

EBTNodeResult::Type ULLBeastBasicAttack_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Get the AI controller of the beast
	AAIController* AIController = OwnerComp.GetAIOwner();

	if(ensure(AIController))
	{
		AActor* TargetActor = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if(TargetActor == nullptr)
		{
			EBTNodeResult::Failed;
		}
		
		if(!ULLGamePlayFunctionLibrary::IsPlayerAlive(TargetActor))
		{
			EBTNodeResult::Failed;
		}
		
		ALL_AIBeast* BeastAI = Cast<ALL_AIBeast>(AIController->GetPawn());
		if(BeastAI)
		{
			BeastAI->BasicAttackSequence();
			UE_LOG(LogTemp,Warning,TEXT("Beast attacked player"));

		}
		return  EBTNodeResult::Succeeded;
	}
	
	//Get the target that he is chasing and is in range

	
	return EBTNodeResult::Failed;
}

