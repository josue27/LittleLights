// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LL_Ability.h"
#include "LL_CrouchCross_Ability.generated.h"

/**
 * 
 */
UCLASS()
class LITTLELIGHTS_API ULL_CrouchCross_Ability : public ULL_Ability
{
	GENERATED_BODY()
public:

	UFUNCTION()
	virtual void StartAbility_Implementation(AActor* Instigator, AActor* SecondActor) override;
};
