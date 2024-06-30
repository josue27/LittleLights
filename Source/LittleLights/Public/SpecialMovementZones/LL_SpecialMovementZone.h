// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LL_GameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SpecialZonesTypes.h"
#include "LL_SpecialMovementZone.generated.h"
class USplineComponent;
class UBoxComponent;
UCLASS()
class LITTLELIGHTS_API ALL_SpecialMovementZone : public AActor,  public  ILL_GameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALL_SpecialMovementZone();
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	virtual void InteractionEnded_Implementation(AActor* InstigatorPawn, bool bSlowTime) override;
	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ELLSpecialMovementZoneType MovementZone_Type;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FText InteractionMessage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsJumping = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsCrouching = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsBalancing = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* RootComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USplineComponent* SplinePath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* TriggerCollider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* BlockerCollider;
	//If true it will destroy after the player completes de movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSingleUse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUsed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnabled;

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void DisableMeshCollisions(float ActivationDelay);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void PlayerEndedTask();

	UFUNCTION(BlueprintCallable)
	void DisableCollisions(bool bDisable);

};
