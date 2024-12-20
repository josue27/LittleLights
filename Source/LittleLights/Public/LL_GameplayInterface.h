// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LL_GameplayInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULL_GameplayInterface : public UInterface
{
	GENERATED_BODY()

};

/**
 * 
 */
class LITTLELIGHTS_API ILL_GameplayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	void Interact(APawn* InstigatorPawn);

	UFUNCTION(BlueprintNativeEvent)
	void InteractionEnded(AActor* InstigatorPawn,bool bSlowTime);
	UFUNCTION(BlueprintNativeEvent)
	void StartGame();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BeaconCompleted();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	FText GetInteractText(APawn* InstigatorPawn);

	/**
	 * @brief For the moment used to activate on object afeter a dialogue ended
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ActivateObject();

	/**
	 * @brief For the moment used to activate on objects after a dialogue ended
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ActivateObjectsOnList();
};
