// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JumpOverZone.generated.h"

class APersonaje;
UENUM(BlueprintType)
enum class EspecialMovementZoneType : uint8
{
	JumpOver = 0 UMETA(DisplayName = "JumpOver"),
	Crouch = 1 UMETA(DisplayName = "Crouch")
};
UCLASS()
class LITTLELIGHTS_API AJumpOverZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJumpOverZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EspecialMovementZoneType MovementZone_Type;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
		void StartJumpOver(const EspecialMovementZoneType& TypeOfMovement, class APersonaje* Player);
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsJumping = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsCrouching = false;
};
