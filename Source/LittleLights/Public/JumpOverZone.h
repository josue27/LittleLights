// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JumpOverZone.generated.h"

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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
		void StartJumpOver();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsJumping = false;
};
