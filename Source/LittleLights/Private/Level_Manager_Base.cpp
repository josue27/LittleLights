// Fill out your copyright notice in the Description page of Project Settings.


#include "Level_Manager_Base.h"

#include "LL_GameplayInterface.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialParameterCollection.h"

// Sets default values
ALevel_Manager_Base::ALevel_Manager_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevel_Manager_Base::BeginPlay()
{
	Super::BeginPlay();
	Player =  Cast<APlayerCharacter>( UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	if(EnableIntroMovement)
	{
		PlayerIntroMovement();
	}

}

void ALevel_Manager_Base::GameStart()
{
	if(Player)
	{
		if(Player->Implements<ULL_GameplayInterface>())
		{
			ILL_GameplayInterface::Execute_StartGame(Player);
		}
	}
}


// Called every frame
void ALevel_Manager_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevel_Manager_Base::LevelCompleted()
{
	CurrentGameState = EGameState::LevelCompleted;
	TArray<AActor*> CallableActors;
	UGameplayStatics::GetAllActorsWithInterface(this,ULL_GameplayInterface::StaticClass(),CallableActors);
	for(AActor* CallableActor : CallableActors)
	{
		
		ILL_GameplayInterface::Execute_BeaconCompleted(CallableActor);
		
	}
	
	LightUpMoon();
}

void ALevel_Manager_Base::PlayerEndedIntroMovement()
{
	if(!Player)
		return;
	UE_LOG(LogTemp,Warning,TEXT("Player ended intro movement"));
	Player->EnableInput(UGameplayStatics::GetPlayerController(this,0));
	GameStart();
}

void ALevel_Manager_Base::PlayerIntroMovement()
{

	if(ensure(Player) && PlayerIntroDestiny)
	{
		Player->MovePlayerTo(PlayerIntroDestiny->GetActorLocation());
	}
}


