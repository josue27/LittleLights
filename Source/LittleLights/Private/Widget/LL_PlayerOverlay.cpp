// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/LL_PlayerOverlay.h"

#include "Components/Image.h"

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

void ULL_PlayerOverlay::ShowDialogue_Implementation(bool bShow,const FText& TextToDisplay)
{
	
}
