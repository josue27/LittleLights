// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/LL_BTService_CheckPlayerRange.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"

void ULL_BTService_CheckPlayerRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if(ensure(BBComp))
	{
		AActor* TargetActor = Cast<AActor>( BBComp->GetValueAsObject(TargetKeyName.SelectedKeyName));
		if(TargetActor)
		{
			AAIController* AIController = OwnerComp.GetAIOwner();
			if(ensure(AIController))
			{
				APawn* AIPawn = AIController->GetPawn();
				if(ensure(AIPawn))
				{
					float Distance = FVector::Distance(TargetActor->GetActorLocation(),AIPawn->GetActorLocation());
					bool InRange = Distance < DistanceAttackRange;
					bool InLineOfSight = false;
					if(InRange)
					{
						InLineOfSight = AIController->LineOfSightTo(TargetActor);
					}
					BBComp->SetValueAsBool(InRangeKey.SelectedKeyName,(InRange && InLineOfSight));
			
					DrawDebugString(OwnerComp.GetWorld(),TargetActor->GetActorLocation(),FString::Printf(TEXT("InRange: %s | InLOS: %s"),InRange?TEXT("true"):TEXT("false"),InLineOfSight?TEXT("true"):TEXT("false")),nullptr,FColor::Red);
					GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::White,FString::Printf(TEXT("InRange: %s | InLOS: %s"),InRange?TEXT("true"):TEXT("false"),InLineOfSight?TEXT("true"):TEXT("false")));
				//if(!InLineOfSight || !InRange)
				//{
				//	BBComp->ClearValue(TargetKeyName.SelectedKeyName);
				//	DrawDebugString(GetWorld(),TargetActor->GetActorLocation(),"Lost Sight of Player");
				//}else if(InRange && InLineOfSight)
				//{
				//	BBComp->SetValueAsVector("MoveToLoc",TargetActor->GetActorLocation());
				//}

				}
			}
		}
	}
}
