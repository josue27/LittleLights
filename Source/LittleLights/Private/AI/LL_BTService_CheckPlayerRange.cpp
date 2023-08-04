// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/LL_BTService_CheckPlayerRange.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "LLGamePlayFunctionLibrary.h"
#include "AI/NavigationSystemBase.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"


//static TAutoConsoleVariable<bool> CVarDebugAI(TEXT("ll.DebugAI"),false,TEXT("Enable spawning of bots via timer"),ECVF_Cheat);


void ULL_BTService_CheckPlayerRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if(ensure(BBComp))
	{
		AActor* TargetActor = Cast<AActor>( BBComp->GetValueAsObject(TargetKeyName.SelectedKeyName));
		if(TargetActor && ULLGamePlayFunctionLibrary::IsPlayerAlive(TargetActor))//Do we have actor and is Alive?
		{
			AAIController* AIController = OwnerComp.GetAIOwner();
			
			if(ensure(AIController))
			{
				APawn* AIPawn = AIController->GetPawn();
				if(ensure(AIPawn))
				{
					float Distance = FVector::Distance(TargetActor->GetActorLocation(),AIPawn->GetActorLocation());
					bool InRange = Distance <= DistanceAttackRange;
					bool InTeleportRange = Distance > DistanceToTriggerTeleport;
					bool InLineOfSight = false;
					if(InRange)
					{
						InLineOfSight = AIController->LineOfSightTo(TargetActor);
					}
					else if (!InRange)
					{
						
						ALL_AIBeast* BeastAI = Cast<ALL_AIBeast>(AIController->GetPawn());
						if(BeastAI) BeastAI->bIsAttacking = false;
						if(InTeleportRange)
						{
							
						

							// if (BeastAI->CurrentTeleports < BeastAI->MaxTeleports)
							// {
							//
							// 	// //1-Make a timer event so it doesnt trigger that fast
							// 	// FTimerDelegate TimerCallback;
							// 	// TimerCallback.BindUFunction(this,"TeleportToRandLocation",TargetActor,AIPawn);
							// 	// GetWorld()->GetTimerManager().SetTimer(TriggerTeleporTimer,TimerCallback,3.f,false);
							//
							// 	
							// 	
							//
							// }
						}
					}
					BBComp->SetValueAsBool(TeleportTriggerKeyName.SelectedKeyName,(InTeleportRange));
					//BBComp->SetValueAsBool(InRangeKey.SelectedKeyName,(InRange && InLineOfSight));
					BBComp->SetValueAsBool(InRangeKey.SelectedKeyName,(InRange));
					/*
					if(CVarDebugAI.GetValueOnGameThread())
					{
						
					DrawDebugString(OwnerComp.GetWorld(),TargetActor->GetActorLocation(),FString::Printf(TEXT("InRange: %s | InLOS: %s"),InRange?TEXT("true"):TEXT("false"),InLineOfSight?TEXT("true"):TEXT("false")),nullptr,FColor::Red);
					GEngine->AddOnScreenDebugMessage(-1,0.0f,FColor::White,FString::Printf(TEXT("InRange: %s | InLOS: %s"),InRange?TEXT("true"):TEXT("false"),InLineOfSight?TEXT("true"):TEXT("false")));
					}
					*/

			

				}
			}
		}else
		{
			BBComp->SetValueAsBool(InRangeKey.SelectedKeyName,false);
			AAIController* AIController = OwnerComp.GetAIOwner();
			if (ensure(AIController))
			{

				ALL_AIBeast* BeastAI = Cast<ALL_AIBeast>(AIController->GetPawn());
				BeastAI->bIsAttacking = false;
			}
			if (!ULLGamePlayFunctionLibrary::IsPlayerAlive(TargetActor))
			{
				//BBComp->SetValueAsBool("")
			}

		}
	}
}

//TODO:I dont like this being here, should we move it to Beast cpp
void ULL_BTService_CheckPlayerRange::TeleportToRandLocation(AActor* TargetActor,APawn* AIPawn)
{
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSys )
	{
		FNavLocation RandLocation;
		FVector OriginLocationSearch = TargetActor->GetActorLocation() +( TargetActor->GetActorForwardVector() * DistanceToTeleport);
		DrawDebugSphere(GetWorld(),OriginLocationSearch,RadiusToSpawn,16,FColor::Yellow,true,5.f);
		if(NavSys->GetRandomReachablePointInRadius(OriginLocationSearch,RadiusToSpawn,RandLocation))
		{
			AIPawn->SetActorLocation(RandLocation.Location);
			ALL_AIBeast* BeastAI = Cast<ALL_AIBeast>(AIPawn);
			BeastAI->CurrentTeleports++;
		}
		else
		{
			FTimerDelegate TimerCallback;
			TimerCallback.BindUFunction(this,"TeleportToRandLocation",TargetActor,AIPawn);
								
			GetWorld()->GetTimerManager().SetTimer(TriggerTeleporTimer,TimerCallback,3.f,false);
		}
	
		//3-This should not affect the attack so we should spawn it a bit far for the player to be able to know what to do
		//4-Make the randlocation be infront of the player aaaand maybe an angle, but in the mean time in front, next would be right left 60angel search
	
	}
}
