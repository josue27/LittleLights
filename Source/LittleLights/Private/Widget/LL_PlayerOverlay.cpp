// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/LL_PlayerOverlay.h"

#include "Components/Image.h"
#include "DialogueSystem/LL_DialogueComponent.h"

void ULL_PlayerOverlay::ShowTotemPieceHUD_Implementation(bool bShow)
{
}

void ULL_PlayerOverlay::TotemPieceDeliveredHUD_Implementation()
{
}

void ULL_PlayerOverlay::ShowBeastPresenceHUD_Implementation(bool bShow)
{
	if(bShow)
	{
		BeastAlertImg->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		BeastAlertImg->SetVisibility(ESlateVisibility::Hidden);

	}
}

void ULL_PlayerOverlay::ShowDialogue_Implementation(bool bShow,  FLL_DialogueLineStruct DialogueStruct)
{
	ShowGameHUD(bShow);
}

void ULL_PlayerOverlay::NextDialogue_Implementation()
{
	
}

void ULL_PlayerOverlay::SetSequenceTimer_Implementation(float InTime)
{
}

void ULL_PlayerOverlay::ShowGameHUD_Implementation(bool bShow)
{
	
}
