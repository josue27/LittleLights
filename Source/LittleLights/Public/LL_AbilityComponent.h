// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LL_Ability.h"
#include "Components/ActorComponent.h"
#include "LL_AbilityComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LITTLELIGHTS_API ULL_AbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULL_AbilityComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Ability")
	TArray<TSubclassOf<ULL_Ability>>DefaultAbilities;
	
	UPROPERTY(BlueprintReadOnly,Category="Ability")
	TArray<ULL_Ability*> Abilities;


	UFUNCTION(BlueprintCallable)
	void AddAbility(AActor* Instigator, TSubclassOf<ULL_Ability> AbilityClass);


	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable,Category="Ability")
	void StartAbilityByName(AActor* Instigator,FName AbilityName);
        
	UFUNCTION(BlueprintCallable,Category="Ability")
	void StopAbilityByName(AActor* Instigator,FName AbilityName);
};
