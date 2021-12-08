// Fill out your copyright notice in the Description page of Project Settings.


#include "LL_GameModeBase.h"

#include "LittleLights.h"
#include "MainPlayer_DebugHUD.h"
#include "ToolBuilderUtil.h"


#include "Kismet/GameplayStatics.h"
#include "Locations/LL_TargetPoint.h"



ALL_GameModeBase::ALL_GameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	//LevelManager =Cast<ALevel_Manager_Base>( UGameplayStatics::GetActorOfClass(GetWorld(),ALevel_Manager_Base::StaticClass()));
	//HUDClass = AMainPlayer_DebugHUD::StaticClass();
	StartSequence();
}

void ALL_GameModeBase::StartSequence_Implementation()
{
	//TODO: we need to get all actor of SpecialLocation and the check wick one is the start and the exit
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(),ALL_TargetPoint::StaticClass());
	ALL_TargetPoint* StartTargetLocation = Cast<ALL_TargetPoint>(UGameplayStatics::GetActorOfClass(GetWorld(),ALL_TargetPoint::StaticClass()));
	if(StartTargetLocation)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>( UGameplayStatics::GetPlayerPawn(GetWorld(),0));
		if(Player)
		{
			Player->MovePlayerTo(StartTargetLocation->GetActorLocation());
		}
	}else
	{
		LogOnScreen(GetWorld(),"No Start Target found",FColor::Yellow);
		UE_LOG(LogTemp,Warning,TEXT("Could not start the sequence"))
	}
}

void ALL_GameModeBase::TottemCompleted_Implementation()
{

	
}
