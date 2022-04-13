// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LL_Ability.h"
#include "LL_Orb_Ability.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOrbDeltaChanged,float,NewDeltaLeft);
/**
 * 
 */
UCLASS()
class LITTLELIGHTS_API ULL_Orb_Ability : public ULL_Ability
{
	GENERATED_BODY()

	public:

	UFUNCTION(BlueprintCallable,Category="Orb")
	void LightUpTorch(AActor* Instigator);

	UFUNCTION(BlueprintCallable,Category="Orb")
	void EndLighting();

	UPROPERTY(BlueprintAssignable,BlueprintCallable)
	FOnOrbDeltaChanged OnOrbDeltaChanged;
};
