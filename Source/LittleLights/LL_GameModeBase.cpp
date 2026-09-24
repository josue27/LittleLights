// Fill out your copyright notice in the Description page of Project Settings.


#include "LL_GameModeBase.h"

#include "LevelConfiguration.h"
#include "LittleLights.h"
#include "LLGameManager.h"
#include "MainPlayer_DebugHUD.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Kismet/GameplayStatics.h"
#include "Locations/LL_TargetPoint.h"
#include "PlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Torch.h"
#include "LL_Tottem.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/AssetManager.h"
#include "LLComponents/LL_ToolsComponent.h"

ALL_GameModeBase::ALL_GameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	//LevelManager =Cast<ALevel_Manager_Base>( UGameplayStatics::GetActorOfClass(GetWorld(),ALevel_Manager_Base::StaticClass()));
	//HUDClass = AMainPlayer_DebugHUD::StaticClass();
	


}
//ATTTENTION BeginPlays starts before STARTPLAY
void ALL_GameModeBase::BeginPlay()
{
	Super::BeginPlay();	
	LlGameManager = GetGameInstance()->GetSubsystem<ULLGameManager>();

}
void ALL_GameModeBase::StartPlay()
{
	Super::StartPlay();
	//Check if we have a saved file
	
	if(LlGameManager )
	{
		if(!LlGameManager->OnLoadGameCompleted.IsAlreadyBound(this,&ThisClass::SaveFileLoaded))
			LlGameManager->OnLoadGameCompleted.AddUniqueDynamic(this,&ThisClass::SaveFileLoaded);

		LlGameManager->LoadGame();
	}
	else
	{
		StartSequence();
		StartBeastTimer();
	}
	
}
void ALL_GameModeBase::SaveFileLoaded(bool bSuccess)
{
	ULLGameManager* GameManager = GetGameInstance()->GetSubsystem<ULLGameManager>();

	if (bSuccess && GetLevelConfiguration() && GameManager)
	{
		const ELLMapsIndexEntry EnteringLevel = GetLevelConfiguration()->InLevel;
		const bool bResume = GameManager->GameSave.bHasMidLevelProgress
			&& GameManager->GameSave.InLevel == EnteringLevel;

		if (bResume)
		{
			RestoreLevelProgress();
		}
		else
		{
			GameManager->ClearLevelProgress();
			GameManager->LevelChanged(EnteringLevel);
			StartSequence();
		}
	}
	else
	{
		StartBeastTimer();
	}


}

void ALL_GameModeBase::RestoreLevelProgress()
{
	ULLGameManager* GameManager = GetGameInstance()->GetSubsystem<ULLGameManager>();
	if (!GameManager)
	{
		return;
	}

	// 1. Totem: marcar piezas ya entregadas
	if (ALL_Tottem* Totem = Cast<ALL_Tottem>(UGameplayStatics::GetActorOfClass(GetWorld(), ALL_Tottem::StaticClass())))
	{
		for (FTottemPieceState& State : Totem->TotemPieces)
		{
			if (GameManager->GameSave.DeliveredPieces.Contains(State.PieceType))
			{
				State.Delivered = true;
				if (State.TotemPiecePlaced)
				{
					State.TotemPiecePlaced->SetActorHiddenInGame(true);
					State.TotemPiecePlaced->SetActorEnableCollision(false);
				}
				if (State.TotemPice)
				{
					State.TotemPice->SetActorHiddenInGame(true);
				}
			}
		}
	}

	// 2. Posicion del jugador
	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Player && !GameManager->GameSave.PlayerLocation.IsZero())
	{
		Player->SetActorLocation(GameManager->GameSave.PlayerLocation);
	}

	// 3. Orbe
	if (Player && Player->ToolsComponent)
	{
		const float Delta = FMath::Clamp(GameManager->GameSave.OrbRemainingDelta, 0.f, 1.f);
		Player->ToolsComponent->RefillOrb(Delta * 30.0f, false);
	}

	// 4. Habilitar input (equivalente a fin de intro, sin el movimiento)
	if (Player)
	{
		Player->EnableInput(UGameplayStatics::GetPlayerController(this, 0));
		Player->ResetWalkSpeed();
	}
	GameStart();
}

void ALL_GameModeBase::StartSequence_Implementation()
{
	
	//TODO: we need to get all actor of SpecialLocation and the check wick one is the start and the exit
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(),ALL_TargetPoint::StaticClass());
	ALL_TargetPoint* StartTargetLocation = Cast<ALL_TargetPoint>(UGameplayStatics::GetActorOfClass(GetWorld(),ALL_TargetPoint::StaticClass()));
	Player = Cast<APlayerCharacter>( UGameplayStatics::GetPlayerPawn(GetWorld(),0));

	bool bStartWithIntroMovement = false;
	const bool bHasLevelConfig = (LevelConfigurationDataAsset != nullptr);
	const bool bStartWithLightUp = bHasLevelConfig && LevelConfigurationDataAsset->bStartWithLightUp;
	const bool bStartWithDecay = bHasLevelConfig && LevelConfigurationDataAsset->bStartWithDecay;
	if(bHasLevelConfig)
	{
		bStartWithIntroMovement = LevelConfigurationDataAsset->bStartWithIntroMovement;
	}
	if (Player)
	{
		if (Player->ToolsComponent)
		{
			if(bStartWithLightUp)
				Player->ToolsComponent->RefillOrb(30.0f, false);
			
			UE_LOG(LogTemp, Warning, TEXT("GM: Lighting orb"));
		}
	}
	if (bStartWithIntroMovement)
	{
		if (StartTargetLocation)
		{
			if (Player)
			{
				//TODO: the speed should be calculated by the charactermovement not an arbitrary value or set value
				Player->MovePlayerTo(StartTargetLocation->GetActorLocation(), 150.0f, true);
				if (Player->ToolsComponent)
				{
					//This is so when we start the level the light is up
					if(bStartWithLightUp)
						Player->ToolsComponent->RefillOrb(30.0f, bStartWithDecay);
					
					UE_LOG(LogTemp, Warning, TEXT("GM: Lighting orb"));
				}
			}
		}
		else
		{
			LogOnScreen(GetWorld(), "No Start Target found", FColor::Yellow);
			UE_LOG(LogTemp, Warning, TEXT("Could not start the sequence"))

			PlayerEndedIntroMovement(bStartWithLightUp,bStartWithDecay);

		}
	}
	else
	{
		PlayerEndedIntroMovement(bStartWithLightUp,bStartWithDecay);

	}
	
	//StartBeastTimer();

	
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

/**
 * Get the data asset that contains the configuration for the current level
 * @return ULevelConfiguration data asset
 */
ULevelConfiguration* ALL_GameModeBase::GetLevelConfiguration()
{
	if(LevelConfigurationDataAsset)
	{
		return LevelConfigurationDataAsset;
	}
	return  nullptr;
}


float ALL_GameModeBase::DeltaDistanceToBeast()
{
	
	if(Player && BeastAI)
	{
		float Distance = FVector::Distance(Player->GetActorLocation(),BeastAI->GetActorLocation());
		return Distance;
	}
	return 1.f;
}

void ALL_GameModeBase::SpawnBeast_Implementation()
{
	// if(ensure(BeastAI_Class) )
	// {
	// 	LogOnScreen(GetWorld(),"Can not spawn, BeastAI_Class = none ");
	//
	// 	return;
	// }
	// if(BeastAI)
	// {
	// 	LogOnScreen(GetWorld()," Beast was already spawned");
	// 	return;
	// }
	// LogOnScreen(GetWorld(),"Starting Beast spawn");
	//
	// //TODO: Make an EQS for get a spawnable location
	// UEnvQueryInstanceBlueprintWrapper* QueryLocationInstance = UEnvQueryManager::RunEQSQuery(this,SpawnBeastLocationQuery,this,EEnvQueryRunMode::RandomBest5Pct,nullptr);
	// if(ensure(QueryLocationInstance))
	// {
	// 	QueryLocationInstance->GetOnQueryFinishedEvent().AddDynamic(this,&ALL_GameModeBase::OnLocationQueryCompleted);
	// }

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

	//Save game
	if (ULLGameManager* GameManager = GetGameInstance()->GetSubsystem<ULLGameManager>())
	{
		GameManager->ClearLevelProgress();
		GameManager->LevelCompleted(GetLevelConfiguration()->InLevel);
		GameManager->SaveGame();
	}
	
}

void ALL_GameModeBase::PlayerEndedIntroMovement(bool bLightUpOrb, bool bStartOrbDecay)
{
	if (!Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO PLAYER IN GAMEMODE"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Player ended intro movement"));
	Player->EnableInput(UGameplayStatics::GetPlayerController(this, 0));
	Player->ResetWalkSpeed();
	if (Player->ToolsComponent && bLightUpOrb)
	{
		Player->ToolsComponent->RefillOrb(30.0f, bStartOrbDecay);
		
		
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

void ALL_GameModeBase::StartOrbDecay()
{
	if (!Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO PLAYER IN GAMEMODE"));
		return;
	}

	if (Player->ToolsComponent)
	{
		Player->ToolsComponent->RefillOrb(30.0f);
		Player->ToolsComponent->StartOrbDecay();

		UE_LOG(LogTemp, Warning, TEXT("GM: Lighting orb"));

	}
}