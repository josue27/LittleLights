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

	/*UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
		TArray<FVector>PathPoints;*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* RootComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USplineComponent* SplinePath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* TriggerCollider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* BlockerCollider;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
