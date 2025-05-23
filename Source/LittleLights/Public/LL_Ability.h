// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LL_AbilityComponent.h"

#include "LL_Ability.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class LITTLELIGHTS_API ULL_Ability : public UObject
{
	GENERATED_BODY()

	public:

	
	
	UPROPERTY(EditDefaultsOnly,Category="Ability")
	bool bTick;
	UPROPERTY(EditDefaultsOnly,Category="Ability")
	FName AbilityName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float RemainingActionTime;

	UFUNCTION(BlueprintNativeEvent,Category="Ability")
	void StartAbility(AActor* Instigator, AActor* SecondActor =nullptr);
	
	UFUNCTION(BlueprintNativeEvent,Category="Ability")
	void StopAbility(AActor* Instigator,AActor* SecondActor =nullptr);

	UFUNCTION(BlueprintNativeEvent,Category="Ability")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintCallable,Category="Ability")
	bool IsRunning();

	UFUNCTION(BlueprintCallable,Category="Ability Tags")
	ULL_AbilityComponent* GetOwningComponent();

	UFUNCTION()
	void Initialize(ULL_AbilityComponent* AbilityComp);

	UFUNCTION(BlueprintCallable)
	UWorld* GetWorld() const override;

	UFUNCTION(BlueprintNativeEvent,Category="Abilit Tags")
	void Update(float DeltaTime);
	virtual void Update_Implementation(float DeltaTime);
	protected:

	UPROPERTY()
	bool bIsRunning;

	
	UPROPERTY(BlueprintReadOnly)
	ULL_AbilityComponent* AbilityComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "LL Ability | Tags")
		FGameplayTagContainer GrantsTags;//add tags when using this ability
	UPROPERTY(EditDefaultsOnly, Category = "LL Ability | Tags")
		FGameplayTagContainer BlockedTags;//tags that blocks this ability


};
