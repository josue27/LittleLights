// Copyright Epic Games, Inc. All Rights Reserved.


#include "LittleLightsGameModeBase.h"
#include "MainPlayer_DebugHUD.h"

ALittleLightsGameModeBase::ALittleLightsGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	HUDClass = AMainPlayer_DebugHUD::StaticClass();
}
