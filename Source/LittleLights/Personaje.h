// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Personaje.generated.h"
class ATorch;
class UArrowComponent;
UCLASS()
class LITTLELIGHTS_API APersonaje : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APersonaje();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VelocidadRotacion = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VelocidadMovimiento = 1.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Components", meta=(AllowPrivateAccess = "true"))
	UArrowComponent* PosicionSpawnBengala;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 BengalasDisponibles = 3;
	UFUNCTION(BlueprintPure)
	int32 GetBengalas();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanMove = true;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bFlaresEnabled	= false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bStartWithLight= true;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<AActor> Flare;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATorch> TorchClass;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ATorch* Torch;
	UFUNCTION()
	void LightUpTorch();

private:
	void MovimientoForward(float AxisValue);
	void MovimientoRight(float AxisValue);
	void RotacionHorizontal(float AxisValue);
	void UpdateRotacion();
	UFUNCTION()
	void ShootFlare();
	FQuat RotacionFinal;

};
