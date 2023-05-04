// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LL_Ability.h"
#include "LL_JumpVault_Ability.generated.h"

/**
 * 
 */
class APlayerCharacter;
class ALL_SpecialMovementZone;
class ULL_WorldUserWidget;
class ALL_PlayerControllerBase;

UCLASS()
class LITTLELIGHTS_API ULL_JumpVault_Ability : public ULL_Ability
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TArray<FKey> KeysToPress;
	UFUNCTION()
	virtual void StartAbility_Implementation(AActor* Instigator, AActor* SecondActor) override;
	UFUNCTION()
	virtual void StopAbility_Implementation(AActor* Instigator, AActor* SecondActor = nullptr) override;

	UPROPERTY()
	bool bCanReceiveInput;

	UPROPERTY()
	TArray<FVector> PathPositions;

	UPROPERTY()
	ALL_SpecialMovementZone* SpecialMovementZone;
	UPROPERTY()
	APlayerCharacter* Player;



	UPROPERTY()
	ALL_PlayerControllerBase* LLPlayerController = nullptr;

private:
	UPROPERTY()
	int InKeyPressed;

	UFUNCTION()
	void KeyPressed(FKey KeyPressed);

	UFUNCTION()
	void PlayerEndedMovement(APlayerCharacter* PlayerActor,bool bLightUpOrb, bool bStartOrbDecay);
};
