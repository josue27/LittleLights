// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/LL_MoveToPlayer.h"
#include "PlayerCharacter.h"
#include "AIController.h"
#include "LLComponents/LL_ToolsComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void ULL_MoveToPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BBComp))
	{
		
		APlayerCharacter* LLPlayer = Cast<APlayerCharacter>(BBComp->GetValueAsObject(TargetKeyName.SelectedKeyName));
		if (LLPlayer && LLPlayer->IsPlayerAlive())
		{
			//for the moment this pawn should be the beast
			APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
			if (ensure(AIPawn))
			{
				if (LLPlayer->ToolsComponent)
				{
					AcceptableRadius = LLPlayer->ToolsComponent->GetDeltaRemainOrb() <= 0.f? 200.f : 1300.f ;
				}
				
			}
		}
	}
}