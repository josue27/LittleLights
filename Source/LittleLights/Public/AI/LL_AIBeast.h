// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LL_AIBeast.generated.h"

class UPawnSensingComponent;
UCLASS()
class LITTLELIGHTS_API ALL_AIBeast : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALL_AIBeast();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,Category="Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere,Category="AI")
	FName TargetKeyName ="TargetPlayer";	

	

	UFUNCTION()
	void PlayerSeen(APawn* PlayerPawn);

	UFUNCTION()
	void SetTarget(AActor* Actor);

	
	UFUNCTION()
	void ResetTarget(AActor* Actor);
	
	UPROPERTY()
	AActor* TargetActorTemp;

	virtual void PostInitializeComponents() override;


	
public:	

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "LLBeast")
		bool bIsAttacking;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void BasicAttackSequence();



};

