// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/LL_AIBeast.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ALL_AIBeast::ALL_AIBeast()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
void ALL_AIBeast::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensingComp->OnSeePawn.AddDynamic(this,&ALL_AIBeast::PlayerSeen);
	
}
// Called when the game starts or when spawned
void ALL_AIBeast::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALL_AIBeast::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if(GetController()->LineOfSightTo())
	if(TargetActorTemp && !PawnSensingComp->HasLineOfSightTo(TargetActorTemp))
	{
		ResetTarget(TargetActorTemp);
	}
}

void ALL_AIBeast::PlayerSeen(APawn* PlayerPawn)
{
	if(!PlayerPawn)
	{
		return;
	}
	if(PawnSensingComp->HasLineOfSightTo(PlayerPawn))
	{
		SetTarget(PlayerPawn);
		DrawDebugString(GetWorld(),PlayerPawn->GetActorLocation(),"Player Seen");
	}
	
}

void ALL_AIBeast::SetTarget(AActor* Actor)
{
	
	AAIController* AIC = Cast<AAIController>(GetController());
	if(AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject(TargetKeyName,Actor);
		AIC->GetBlackboardComponent()->SetValueAsVector("MoveToLoc",Actor->GetActorLocation());

	}
	TargetActorTemp = Actor;
}

void ALL_AIBeast::ResetTarget(AActor* Actor)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if(AIC)
	{
		AIC->GetBlackboardComponent()->ClearValue(TargetKeyName);
		//AIC->GetBlackboardComponent()->SetValueAsVector("MoveToLoc",Actor->GetActorLocation());

	}
	TargetActorTemp = nullptr;
}