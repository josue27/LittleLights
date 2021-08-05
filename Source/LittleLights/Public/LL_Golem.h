// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LL_Golem.generated.h"

class UPawnSensingComponent;
UCLASS()
class LITTLELIGHTS_API ALL_Golem : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALL_Golem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,Category="Components")
		UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Waypoints")
		bool bFollowWayPoints;
	UPROPERTY(EditAnywhere,BlueprintReadWrite ,Category = "Waypoints", meta =(EditCondition = "bFollowWayPoints"))
		TArray<AActor*> Waypoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoints", meta = (EditCondition = "bFollowWayPoints"))
		bool bMovingToPoint = false;
	UFUNCTION(BlueprintCallable)
		void MoveNextPoint();
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		int32 OnPoint = 0;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent)
		void GolemStoped();

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool bDestroyAtEnd = false;

};
