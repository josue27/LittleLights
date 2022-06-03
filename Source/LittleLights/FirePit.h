// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LL_GameplayInterface.h"
#include "GameFramework/Actor.h"
#include "FirePit.generated.h"

class UPointLightComponent;
class USphereComponent;
UCLASS()
class LITTLELIGHTS_API AFirePit : public AActor, public  ILL_GameplayInterface
{
	GENERATED_BODY()

	void Interact_Implementation(APawn* InstigatorPawn) override;
	FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
public:	
	// Sets default values for this actor's properties
	AFirePit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	USceneComponent* Root;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UPointLightComponent* PitLight;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	USphereComponent* SphereCollision;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText InteractionMessage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Pond")
	float RefillAmount = 30.0f;
	
	//UFUNCTION()
	//void PlayerInPit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	//UFUNCTION()
	//void PlayerLeftPit(UPrimitiveComponent* OverlappedComp,class AActor * OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
