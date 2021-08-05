// Fill out your copyright notice in the Description page of Project Settings.


#include "LL_Golem.h"
#include "Perception/PawnSensingComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

// Sets default values
ALL_Golem::ALL_Golem()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

}

// Called when the game starts or when spawned
void ALL_Golem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALL_Golem::MoveNextPoint()
{
	if (bFollowWayPoints == false || OnPoint >= Waypoints.Num() || Waypoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No more waypoints"));
		if (bDestroyAtEnd)
		{
			Destroy();
		}
		return;
	}

	AActor* Goal = Waypoints[OnPoint];
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), Goal);
	bMovingToPoint = true;
	OnPoint++;
}

// Called every frame
void ALL_Golem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALL_Golem::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

