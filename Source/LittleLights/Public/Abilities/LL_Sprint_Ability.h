// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LL_Ability.h"
#include "LL_Sprint_Ability.generated.h"

/**
 * 
 */
UCLASS()
class LITTLELIGHTS_API ULL_Sprint_Ability : public ULL_Ability
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "LL Ability ")
	float MaxStamine = 8.f;


};
