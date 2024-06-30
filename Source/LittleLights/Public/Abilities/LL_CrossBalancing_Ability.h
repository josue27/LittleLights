// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LL_Ability.h"
#include "LL_CrossBalancing_Ability.generated.h"

class USplineComponent;
class APlayerCharacter;
class ALL_SpecialMovementZone;
/**
 * 
 */
UCLASS()
class LITTLELIGHTS_API ULL_CrossBalancing_Ability : public ULL_Ability
{
	GENERATED_BODY()

public:
	virtual void StartAbility_Implementation(AActor* Instigator, AActor* SecondActor) override;

	virtual void Update_Implementation(float DeltaTime) override;
	virtual void StopAbility_Implementation(AActor* Instigator, AActor* SecondActor) override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite);
	float ZOffset = 10.f;
protected:
	
	UPROPERTY(BlueprintReadOnly)
	APlayerCharacter* PlayerCharacter;
	
	UPROPERTY(BlueprintReadOnly)
	ALL_SpecialMovementZone* JumpOverZone;

	UPROPERTY(BlueprintReadOnly)
	USplineComponent* Spline;

	UPROPERTY(BlueprintReadWrite)
	bool bFalling;
	UPROPERTY(BlueprintReadWrite)
	bool bEndLocation;
	UPROPERTY(BlueprintReadWrite)
	bool bStartBalance;

	UPROPERTY(BlueprintReadWrite)
	float CrossingVelocityMultiplier;
	UPROPERTY(BlueprintReadWrite)
	float DistanceWalkedX;
	UPROPERTY(BlueprintReadWrite)
	float UnbalanceVelocity;
	UPROPERTY(BlueprintReadWrite)
	float BalancingX;

	UPROPERTY(BlueprintReadOnly)
	FVector PlayerStartLoc;
	UPROPERTY(BlueprintReadOnly)
	FVector PlayerEndLoc;
	UPROPERTY(BlueprintReadOnly)
	bool bCloserToEnd;
	
};
