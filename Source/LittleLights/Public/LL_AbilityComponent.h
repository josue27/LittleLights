// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "LL_AbilityComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStamineChanged, AActor*, InstigatorActor, float, NewStamina, float, Delta);

class ULL_Ability;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LITTLELIGHTS_API ULL_AbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="LL Ability | Tags")
	FGameplayTagContainer ActiveGameplayTags;
	// Sets default values for this component's properties
	ULL_AbilityComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable,Category="Ability")
	void StartAbilityByName(AActor* Instigator,FName AbilityName, AActor* ActorInfo = nullptr);

	//@Instigator is Player
	UFUNCTION(BlueprintCallable,Category="Ability")
	void StopAbilityByName(AActor* Instigator,FName AbilityName,  AActor* ActorInfo = nullptr);

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnStamineChanged OnStamineChanged;

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Ability")
	TArray<TSubclassOf<ULL_Ability>>DefaultAbilities;
	
	UPROPERTY(BlueprintReadOnly,Category="Ability")
	TArray<ULL_Ability*> Abilities;


	UFUNCTION(BlueprintCallable)
	void AddAbility(AActor* Instigator, TSubclassOf<ULL_Ability> AbilityClass);


	
};
