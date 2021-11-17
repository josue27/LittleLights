// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/LL_BTService_CheckPlayerRange.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "LLGamePlayFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

static TAutoConsoleVariable<bool> CVarDebugAI(TEXT("ll.DebugAI"),false,TEXT("Enable spawning of bots via timer"),ECVF_Cheat);

void ULL_BTService_CheckPlayerRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if(ensure(BBComp))
	{
		AActor* TargetActor = Cast<AActor>( BBComp->GetValueAsObject(TargetKeyName.SelectedKeyName));
		if(TargetActor && ULLGamePlayFunctionLibrary::IsPlayerAlive(TargetActor))
		{
			AAIController* AIController = OwnerComp.GetAIOwner();
			if(ensure(AIController))
			{
				APawn* AIPawn = AIController->GetPawn();
				if(ensure(AIPawn))
				{
					float Distance = FVector::Distance(TargetActor->GetActorLocation(),AIPawn->GetActorLocation());
					bool InRange = Distance <= DistanceAttackRange;
					bool InLineOfSight = false;
					if(InRange)
					{
						InLineOfSight = AIController->LineOfSightTo(TargetActor);
					}
					BBComp->SetValueAsBool(InRangeKey.SelectedKeyName,(InRange && InLineOfSight));

					if(CVarDebugAI.GetValueOnGameThread())
					{
						
					DrawDebugString(OwnerComp.GetWorld(),TargetActor->GetActorLocation(),FString::Printf(TEXT("InRange: %s | InLOS: %s"),InRange?TEXT("true"):TEXT("false"),InLineOfSight?TEXT("true"):TEXT("false")),nullptr,FColor::Red);
					GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::White,FString::Printf(TEXT("InRange: %s | InLOS: %s"),InRange?TEXT("true"):TEXT("false"),InLineOfSight?TEXT("true"):TEXT("false")));
					}

			

				}
			}
		}else
		{
			BBComp->SetValueAsBool(InRangeKey.SelectedKeyName,false);

		}
	}
}
