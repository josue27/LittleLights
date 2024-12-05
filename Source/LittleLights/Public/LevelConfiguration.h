// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelConfiguration.generated.h"

enum class ELLMapsIndexEntry : uint8;
/**
 * 
 */
UCLASS()
class LITTLELIGHTS_API ULevelConfiguration : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ELLMapsIndexEntry InLevel;
	//Initial Orb Logic
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bStartWithDecay;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bStartWithLightUp;
	
	//Inital Player state
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bStartWithIntroMovement;
};
