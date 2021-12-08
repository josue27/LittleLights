// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

static void LogOnScreen(UObject* WorldContext, FString Msg, FColor Color = FColor::White, float Duration = 5.0f)
{
	if(!ensure(WorldContext))
	{
		return;
	}

	UWorld* World = WorldContext->GetWorld();
	if(!ensure(World))
	{
		return;
	}
	//if we need to know if it client or server
	FString NetPrefix = World->IsNetMode(NM_Client) ? "[Client]" : "[SERVER]";
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1,Duration,Color,NetPrefix + Msg);
	}
	
}