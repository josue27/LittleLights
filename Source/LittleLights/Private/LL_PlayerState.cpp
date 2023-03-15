// Fill out your copyright notice in the Description page of Project Settings.


#include "LL_PlayerState.h"

int32 ALL_PlayerState::GetLevelPieces()
{
	return levelPieces;
}

bool ALL_PlayerState::AddTotemPiece()
{
	//TODO:Check if we can add pieces
	levelPieces++;
	OnPiecesCountChanged.Broadcast(this,levelPieces);
	UE_LOG(LogTemp,Warning,TEXT("Totem Piece added to score"));
	return true;
}

/// <summary>
/// When the player insert a piece of totem to the totem
/// </summary>
/// <returns></returns>
bool ALL_PlayerState::RemoveTotemPiece()
{
	if(levelPieces <= 0)
	{
		
		return  false;
	}
	levelPieces -=1;
	OnPiecesCountChanged.Broadcast(this,levelPieces);
	UE_LOG(LogTemp,Warning,TEXT("Totem Piece REMOVED from score"));

	return  true;
}
