// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LL_Ability.h"
#include "LL_Orb_Ability.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOrbDeltaChanged,float,NewDeltaLeft);
/**
 * 
 */
class ALL_Orb;
class APlayerCharacter;
UCLASS()
class LITTLELIGHTS_API ULL_Orb_Ability : public ULL_Ability
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ALL_Orb* OrbMesh;

	UFUNCTION(BlueprintCallable, Category = "LL Ability | Orb")
	void SpawnOrb(APlayerCharacter* PC);

	UFUNCTION(BlueprintCallable,Category="LL Ability | Orb")
	void LightUpTorch(AActor* Instigator);

	UFUNCTION(BlueprintCallable,Category="LL Ability | Orb")
	void EndLighting();

	UFUNCTION(BlueprintCallable, Category = "LL Ability | Orb")
	void LightDecay();

	UPROPERTY(BlueprintAssignable,BlueprintCallable)
	FOnOrbDeltaChanged OnOrbDeltaChanged;
};
