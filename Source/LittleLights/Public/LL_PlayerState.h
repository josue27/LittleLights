// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LL_PlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPiecesCountChanged, ALL_PlayerState*,PlayerState,int32,Pieces);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRefillingOrb,bool,bRefilling);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObstacleStarted,bool,bStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOrbRefillFinished, bool, bRefillingFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractionStarted,AActor*,ActorInteracting,bool,bSlowTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractionEnded,AActor*,ActorInteracting,bool,bSlowTime);
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

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LevelState")
		int32 LevelPiecesToFind = 0;

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "LevelState")
	FOnRefillingOrb OnRefillingOrb;

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "LevelState")
	FOnObstacleStarted OnObstacleStarted;

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "LevelState")
	FOnOrbRefillFinished OnOrbRefillFinished;
	UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "LevelState")
	FOnInteractionStarted OnInteractionStarted;
	UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "LevelState")
	FOnInteractionEnded OnInteractionEnded;
};
