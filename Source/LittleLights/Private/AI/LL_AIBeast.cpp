// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/LL_AIBeast.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "LLGamePlayFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

static TAutoConsoleVariable<bool> CVarDebugAI(TEXT("ll.DebugAI"),false,TEXT("Enable spawning of bots via timer"),ECVF_Cheat);

// Sets default values
ALL_AIBeast::ALL_AIBeast()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
void ALL_AIBeast::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	
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

	
	//if(TargetActorTemp && !PawnSensingComp->HasLineOfSightTo(TargetActorTemp))
	//{
	//	ResetTarget(TargetActorTemp);
	//}
}

void ALL_AIBeast::PlayerSeen(APawn* PlayerPawn)
{
	if(!PlayerPawn)
	{
		return;
	}
	

	
}

void ALL_AIBeast::PlayerHear(APawn* PlayerPawn, const FVector& Location, float Volume)
{
	
}

void ALL_AIBeast::SetTarget(AActor* Actor)
{

	
	AAIController* AIC = Cast<AAIController>(GetController());
	if(AIC)
	{
		// if(AIC->GetBlackboardComponent()->GetValueAsBool("InRange"))
		// {
		// 	ResetTarget(nullptr);
		// 	return;
		// }
		AIC->GetBlackboardComponent()->SetValueAsObject(TargetKeyName,Actor);
		AIC->GetBlackboardComponent()->SetValueAsVector("MoveToLoc",Actor->GetActorLocation());

	}
	//we store the actor so we can use it later on Tick and remove it when is lost
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