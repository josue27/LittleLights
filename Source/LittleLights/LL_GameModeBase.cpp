// Fill out your copyright notice in the Description page of Project Settings.


#include "LL_GameModeBase.h"
#include "LittleLights.h"
#include "MainPlayer_DebugHUD.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Kismet/GameplayStatics.h"
#include "Locations/LL_TargetPoint.h"
#include "PlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Torch.h"
#include "Engine/AssetManager.h"

ALL_GameModeBase::ALL_GameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	//LevelManager =Cast<ALevel_Manager_Base>( UGameplayStatics::GetActorOfClass(GetWorld(),ALevel_Manager_Base::StaticClass()));
	//HUDClass = AMainPlayer_DebugHUD::StaticClass();
	


}
void ALL_GameModeBase::StartPlay()
{
	Super::StartPlay();
	StartSequence();
	StartBeastTimer();
}

void ALL_GameModeBase::StartSequence_Implementation()
{
	
	//TODO: we need to get all actor of SpecialLocation and the check wick one is the start and the exit
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(),ALL_TargetPoint::StaticClass());
	ALL_TargetPoint* StartTargetLocation = Cast<ALL_TargetPoint>(UGameplayStatics::GetActorOfClass(GetWorld(),ALL_TargetPoint::StaticClass()));
	Player = Cast<APlayerCharacter>( UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	
	if (EnableIntroMovement)
	{
		if(StartTargetLocation)
		{
				if(Player)
				{
					Player->MovePlayerTo(StartTargetLocation->GetActorLocation(),150.0f,true);
				}
		}else
			{
				LogOnScreen(GetWorld(),"No Start Target found",FColor::Yellow);
				UE_LOG(LogTemp,Warning,TEXT("Could not start the sequence"))
			}	
	}
	
	//StartBeastTimer();

	if(Player)
	{
		if (Player->Torch)
		{
		
			Player->Torch->TurnOnOrb();
		}
	

	}
}

void ALL_GameModeBase::TottemCompleted_Implementation()
{
	UE_LOG(LogTemp,Log,TEXT("Game mode received completition msg from Totem "));
	
}



void ALL_GameModeBase::StartBeastTimer()
{
	if(TimeToBeast >=0)
	{
		GetWorldTimerManager().SetTimer(SpawnBeastTimerHandler,this,&ALL_GameModeBase::SpawnBeast_Implementation,TimeToBeast,false);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("No beast will spawn with timer"));
		return;
	}
	

}



void ALL_GameModeBase::SpawnBeast_Debug()
{
	SpawnBeast_Implementation();
}

void ALL_GameModeBase::LevelCompleted()
{
	CurrentGameState = EGameState::LevelCompleted;
	TArray<AActor*> CallableActors;
	UGameplayStatics::GetAllActorsWithInterface(this, ULL_GameplayInterface::StaticClass(), CallableActors);
	for (AActor* CallableActor : CallableActors)
	{

		ILL_GameplayInterface::Execute_BeaconCompleted(CallableActor);

	}

	LightUpMoon();
}



float ALL_GameModeBase::DeltaDistanceToBeast()
{
	if(Player && BeastAI)
	{
		float Distance = FVector::Distance(Player->GetActorLocation(),BeastAI->GetActorLocation());
	}
	return 1.0f;
}

void ALL_GameModeBase::SpawnBeast_Implementation()
{
	if(ensure(BeastAI_Class) )
	{
		LogOnScreen(GetWorld(),"Can not spawn, BeastAI_Class = none ");

		return;
	}
	if(BeastAI)
	{
		LogOnScreen(GetWorld()," Beast was already spawned");
		return;
	}
	LogOnScreen(GetWorld(),"Starting Beast spawn");

	//TODO: Make an EQS for get a spawnable location
	UEnvQueryInstanceBlueprintWrapper* QueryLocationInstance = UEnvQueryManager::RunEQSQuery(this,SpawnBeastLocationQuery,this,EEnvQueryRunMode::RandomBest5Pct,nullptr);
	if(ensure(QueryLocationInstance))
	{
		QueryLocationInstance->GetOnQueryFinishedEvent().AddDynamic(this,&ALL_GameModeBase::OnLocationQueryCompleted);
	}

	


}
void ALL_GameModeBase::OnLocationQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus != EEnvQueryStatus::Success)
	{
		LogOnScreen(this,"AIBeast spawn failed on QueryLocation");
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if(Locations.IsValidIndex(0))
	{
		UAssetManager* Manager = UAssetManager::GetIfValid();
		if(Manager)
		{
			
		}
		FActorSpawnParameters Spawnparams;
		Spawnparams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		BeastAI = GetWorld()->SpawnActor<AActor>(BeastAI_Class,Locations[0],FRotator::ZeroRotator,Spawnparams);

		//Spawn hud
		BeastState_Instance = CreateWidget(UGameplayStatics::GetPlayerController(GetWorld(),0),BeastState_WidgetClass);
		if(BeastState_Instance)
		{
			BeastState_Instance->AddToViewport();
		}

		
		LogOnScreen(GetWorld(),"End Beast spawn");
	}
	
}

void ALL_GameModeBase::GameStart()
{
	if (Player)
	{
		if (Player->Implements<ULL_GameplayInterface>())
		{
			ILL_GameplayInterface::Execute_StartGame(Player);
		}
	}
}



void ALL_GameModeBase::BeaconCompleted_Implementation()
{
	UE_LOG(LogTemp,Warning,TEXT("Beacon completed received at GameMode"));

	///Change state of exit gate to OPEN
	FinalDoor = Cast<ADoorBase>( UGameplayStatics::GetActorOfClass(this,ADoorBase::StaticClass()));
	if(FinalDoor)
	{
		FinalDoor->bDoorUnlocked = true;
		LogOnScreen(GetWorld(),"Door Unlocked");
	}
	///Destroy the Beast
	if(BeastAI)
	{
		BeastAI->Destroy();
		LogOnScreen(GetWorld(),"Beast Destroyed");

	}
	//Send Msg to player

	if(Player)
	{
		Execute_BeaconCompleted(Player);
	}

	///Instanciate a Widget for the player to know that the Totem is completed
	/////DEPRECATED:better in BP
	/*if(TotemCompleted_WidgetClass)
	{
		TotemCompleted_WidgetInstance = CreateWidget(UGameplayStatics::GetPlayerController(GetWorld(),0),TotemCompleted_WidgetClass);
		if(TotemCompleted_WidgetInstance)
		{
			TotemCompleted_WidgetInstance->AddToViewport(0);
			FTimerHandle RemoveWidgetTH;
			GetWorldTimerManager().SetTimer(RemoveWidgetTH,this,&ALL_GameModeBase::RemoveWidget,3.0f);
		}
	}*/
}

void ALL_GameModeBase::PlayerEndedIntroMovement()
{
	if (!Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO PLAYER IN GAMEMODE"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Player ended intro movement"));
	Player->EnableInput(UGameplayStatics::GetPlayerController(this, 0));
	Player->ResetWalkSpeed();
	if (Player->Torch)
	{

		Player->Torch->StartDecay(StartWithDecayLight,true);
		UE_LOG(LogTemp, Warning, TEXT("GM: Lighting orb"));

	}

	GameStart();
}

void ALL_GameModeBase::PlayerIntroMovement()
{

	if (ensure(Player) && PlayerIntroDestiny)
	{
		Player->MovePlayerTo(PlayerIntroDestiny->GetActorLocation());
	}

}

void ALL_GameModeBase::RemoveWidget()
{
	if(TotemCompleted_WidgetInstance)
	{
		TotemCompleted_WidgetInstance->RemoveFromParent();
		TotemCompleted_WidgetInstance = nullptr;
	}
}