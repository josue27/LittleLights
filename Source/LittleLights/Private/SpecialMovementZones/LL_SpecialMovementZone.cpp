// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialMovementZones/LL_SpecialMovementZone.h"

#include "LLGamePlayFunctionLibrary.h"
#include "LL_PlayerState.h"
#include "PlayerCharacter.h"
#include "LittleLights/LittleLights.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ALL_SpecialMovementZone::ALL_SpecialMovementZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);
	SplinePath = CreateDefaultSubobject<USplineComponent>(TEXT("SplinePath"));
	SplinePath->SetupAttachment(RootComp);
	TriggerCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerCollider"));
	TriggerCollider->SetupAttachment(RootComp);
	BlockerCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockerCollider"));
	BlockerCollider->SetupAttachment(RootComp);

	

}

// Called when the game starts or when spawned
void ALL_SpecialMovementZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALL_SpecialMovementZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ALL_SpecialMovementZone::Interact_Implementation(APawn* InstigatorPawn)
{
	APlayerCharacter* PC = Cast<APlayerCharacter>(InstigatorPawn);
	if (PC)
	{
		ULL_AbilityComponent* AC = Cast<ULL_AbilityComponent>(InstigatorPawn->GetComponentByClass(ULL_AbilityComponent::StaticClass()));
		if (AC)
		{
			switch (MovementZone_Type)
			{
			case ELLSpecialMovementZoneType::SMZ_Crouch:
				AC->StartAbilityByName(InstigatorPawn, "Crouch", this);
				UE_LOG(LogTemp, Warning, TEXT("%s Sent Crouch ability"), *GetNameSafe(this));
				ULLGamePlayFunctionLibrary::ScreenLog(TEXT("%s Sent Crouch ability"));
				LogOnScreen(this, FString(TEXT("%s Sent Crouch ability")));

				break;
			case ELLSpecialMovementZoneType::SMZ_CrossBalancing:
				AC->StartAbilityByName(InstigatorPawn, "CrossBalancing", this);
				UE_LOG(LogTemp, Warning, TEXT("%s Sent Crossbalancing ability"), *GetNameSafe(this));
				break;
			case ELLSpecialMovementZoneType::SMZ_JumpOver:
				AC->StartAbilityByName(InstigatorPawn, "JumpOver", this);
				UE_LOG(LogTemp, Warning, TEXT("%s Sent JumpOver ability"), *GetNameSafe(this));
				break;
			default:
				UE_LOG(LogTemp, Warning, TEXT("%s Warning MovementZone not registered/invalid"), *GetNameSafe(this));
				break;
			}
		}
		//Call slow down, mainly for beast
		ALL_PlayerState* PlayerState = Cast<ALL_PlayerState>(PC->GetPlayerState());
		if(PlayerState)
		{
			PlayerState->OnInteractionStarted.Broadcast(this,true);
			PlayerState->OnInteractionEnded.AddDynamic(this,&ALL_SpecialMovementZone::InteractionEnded_Implementation);
		}

	}
}

void ALL_SpecialMovementZone::InteractionEnded_Implementation(AActor* InstigatorPawn, bool bSlowTime)
{
	PlayerEndedTask();
}

FText ALL_SpecialMovementZone::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(InstigatorPawn);
	if (Player && !InteractionMessage.IsEmpty())
	{
		return InteractionMessage;
	}
	return FText::GetEmpty();
}

void ALL_SpecialMovementZone::PlayerEndedTask()
{
	BlockerCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TriggerCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ALL_PlayerState* PlayerState = Cast<ALL_PlayerState>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	if(PlayerState)
	{
		PlayerState->OnInteractionEnded.RemoveDynamic(this,&ALL_SpecialMovementZone::InteractionEnded_Implementation);
	}
	if(bIsSingleUse)
	{
		TriggerCollider->SetCollisionResponseToChannels(ECollisionResponse::ECR_Ignore);
		
	}
}