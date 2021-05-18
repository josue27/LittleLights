// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer_DebugHUD.h"
#include "GameFramework/HUD.h"
#include "LL_GameModeBase.h"

ALL_GameModeBase::ALL_GameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	HUDClass = AMainPlayer_DebugHUD::StaticClass();
}