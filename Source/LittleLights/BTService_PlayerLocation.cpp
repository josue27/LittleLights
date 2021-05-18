// Fill out your copyright notice in the Description page of Project Settings.
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "BTService_PlayerLocation.h"
#include "AIController.h"


UBTService_PlayerLocation::UBTService_PlayerLocation() 
{
    NodeName ="UpdatePlayerLocationIfSeen";
    
}

void UBTService_PlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) 
{
    Super::TickNode(OwnerComp,NodeMemory,DeltaSeconds);

    // if(OwnerComp.GetAIOwner() == nullptr)
    //     return;
    
    // APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
    // if(PlayerPawn)
    // {
    //     OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(),PlayerPawn);
    // }else
    // {
    //     OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());

    // }

    // if (OwnerComp.GetAIOwner() == nullptr)
    //{
    //    return;
    //}
    //AAIController* Controller = OwnerComp.GetAIOwner();
    //if(Controller ==nullptr)
    //{
    //    return;
    //}
    //APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
    //if(PlayerPawn == nullptr)
    //{
    //    return;
    //}
    //if(Controller->LineOfSightTo(PlayerPawn))
    //{

    //    // OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(),PlayerPawn->GetActorLocation());
    //    OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(),PlayerPawn);
    //}else{
    //    OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());

    //}
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn == nullptr)
    {
        return;
    }

    OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerPawn->GetActorLocation());
}

