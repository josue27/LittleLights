// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LL_Orb.generated.h"

class ULL_ToolsComponent;

UCLASS()
class LITTLELIGHTS_API ALL_Orb : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALL_Orb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* TorchMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterial* OrbLiquidMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstanceDynamic* InstanceLiquidMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* LiquidMesh;
	UFUNCTION()
		void UpdateLight(float DeltaTime);

	UFUNCTION(BlueprintCallable)
		void StartDecay();
		UFUNCTION(BlueprintCallable)
		void StopDecay();
	UFUNCTION()
		void RefillOrb(float Amount);

	UPROPERTY()
	ULL_ToolsComponent* ToolsComponent;
	
	UPROPERTY(BlueprintReadWrite)
	float RemainingLightTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxLightUpTime;//MAX??
	
	UPROPERTY(BlueprintReadWrite)
	float RemainingDeltaTime;

	bool bStartDecay;



	
};
