// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Torch.generated.h"

class UPointLightComponent;
class USpotLightComponent;
UCLASS()
class LITTLELIGHTS_API ATorch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATorch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* TorchMesh;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UPointLightComponent* TorchLight;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MaxIntensity;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MinIntensity;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float LightDecayVelocity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float InitialIntensity = 3000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float  CurrentTime = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LightUpTime = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bStartDecay = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DeltaIntensity = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USpotLightComponent* SpotLight_Component;

	UFUNCTION(BlueprintCallable)
		void StartDecay(float NewLightUpTime);
	UFUNCTION(BlueprintCallable)
		void RestartLight();
	UFUNCTION(BlueprintCallable)
		void LightDecay();
};
