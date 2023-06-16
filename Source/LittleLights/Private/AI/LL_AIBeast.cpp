// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/LL_AIBeast.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "LLGamePlayFunctionLibrary.h"
#include "LL_PlayerState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
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

	LLPlayer = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerController(GetWorld(),0)->GetPawn());
	AIC = nullptr ? Cast<AAIController>(GetController()) : AIC;

	ALL_PlayerState* LLPlayerState = Cast<ALL_PlayerState>(LLPlayer->GetPlayerState());
	if(LLPlayerState)
	{
		LLPlayerState->OnInteractionStarted.AddDynamic(this,&ALL_AIBeast::ALL_AIBeast::UserStartedInteraction);
		LLPlayerState->OnInteractionEnded.AddDynamic(this,&ALL_AIBeast::ALL_AIBeast::UserFinishedInteraction);
	}
}

// Called every frame
void ALL_AIBeast::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	/*if(TargetActorTemp && !PawnSensingComp->HasLineOfSightTo(TargetActorTemp))
	{
		ResetTarget(TargetActorTemp);
	}*/
	if (LLPlayer )
	{
		AIC = nullptr ? Cast<AAIController>(GetController()) : AIC;
		if (AIC)
		{
			AIC->GetBlackboardComponent()->SetValueAsBool("CanChasePlayer", LLPlayer->bOrbOff);

		}
	}
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

	
	AIC = nullptr ? Cast<AAIController>(GetController()) : AIC;
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
	AIC = nullptr ? Cast<AAIController>(GetController()) : AIC;

	if(AIC)
	{
		AIC->GetBlackboardComponent()->ClearValue(TargetKeyName);
		//AIC->GetBlackboardComponent()->SetValueAsVector("MoveToLoc",Actor->GetActorLocation());

	}
	TargetActorTemp = nullptr;
}

void ALL_AIBeast::UserStartedInteraction(AActor* ActorInteractor, bool bSlowTime)
{
	CustomTimeDilation = bSlowTime ? SlowTimeOnInteraction : CustomTimeDilation;
}

void ALL_AIBeast::UserFinishedInteraction(AActor* ActorInteractor, bool bSlowTime)
{
	CustomTimeDilation = bSlowTime ? SlowTimeOnInteraction : 1.f;

}


void ALL_AIBeast::ResetTeleports()
{
	CurrentTeleports = 0;
}

void ALL_AIBeast::Destroyed()
{
	
	Super::Destroyed();
	if(LLPlayer == nullptr)return;
	ALL_PlayerState* LLPlayerState = Cast<ALL_PlayerState>(LLPlayer->GetPlayerState());
	if(LLPlayerState)
	{
		LLPlayerState->OnInteractionStarted.RemoveDynamic(this,&ALL_AIBeast::ALL_AIBeast::UserStartedInteraction);
		LLPlayerState->OnInteractionStarted.RemoveDynamic(this,&ALL_AIBeast::ALL_AIBeast::UserFinishedInteraction);
	}
}
