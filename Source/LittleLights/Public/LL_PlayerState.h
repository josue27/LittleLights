// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LL_PlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPiecesCountChanged, ALL_PlayerState*,PlayerState,int32,Pieces);
/**
 * 
 */
UCLASS()
class LITTLELIGHTS_API ALL_PlayerState : public APlayerState
{
	GENERATED_BODY()
protected:
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="LevelState")
	int32 levelPieces = 0;

	public:
	
	UPROPERTY(BlueprintAssignable,Category="LevelState")
	FOnPiecesCountChanged OnPiecesCountChanged;
	
	UFUNCTION(BlueprintCallable,Category="LevelState")
	int32 GetLevelPieces();
	
	UFUNCTION(BlueprintCallable, Category="State")
	bool AddTotemPiece();

	UFUNCTION(BlueprintCallable,Category="State")
	bool RemoveTotemPiece();

	
	
};
