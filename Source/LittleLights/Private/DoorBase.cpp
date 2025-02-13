// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorBase.h"

#include "LLGameManager.h"
#include "Components/BoxComponent.h"
// Sets default values
ADoorBase::ADoorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	EndTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("EndTrigger"));
	EndTrigger->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADoorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/**
 * We want to check if the animation  should start.
 * We are using the same doors for entering levels from lobby
 * @return should allow to open or not
 */
bool ADoorBase::CanOpenDoor()
{

	if(ULLGameManager* GameManager = GetGameInstance()->GetSubsystem<ULLGameManager>())
	{
		return GameManager->InLevelCompleted < LevelID;
	}
	return false;
}

