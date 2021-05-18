// Fill out your copyright notice in the Description page of Project Settings.


#include "Beast_AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void ABeast_AIController::BeginPlay()
{
    Super::BeginPlay();
    if (AIBehavior != nullptr) {
        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

        RunBehaviorTree(AIBehavior);
        GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
      /*  GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());*/
    }

}

void ABeast_AIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
