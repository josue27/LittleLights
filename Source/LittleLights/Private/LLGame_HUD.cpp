// Fill out your copyright notice in the Description page of Project Settings.


#include "LLGame_HUD.h"
#include "GameFramework/PlayerController.h"
#include "Widget/LL_PlayerOverlay.h"
#include "PlayerCharacter.h"

void ALLGame_HUD::BeginPlay()
{
	Super::BeginPlay();

	
}
void ALLGame_HUD::AddPlayerOverlay()
{
	APlayerController* PC = GetOwningPlayerController();
	if (PlayerOverlayClass && PC)
	{
		PlayerOverlay = CreateWidget<ULL_PlayerOverlay>(PC, PlayerOverlayClass);
		PlayerOverlay->AddToViewport(0);
	}
	// Get player
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPawn());
	if (PlayerOverlay && PlayerCharacter)
	{
	
		PlayerOverlay->DeltaStamine = PlayerCharacter->Stamine;
		//set stamine ammount
		//set delta stamine ammount
	}
}


