// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickable_Object.generated.h"
class USphereComponent;
UCLASS()
class LITTLELIGHTS_API APickable_Object : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickable_Object();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Components", meta=(AllowPrivateAccess = "true"))
	USphereComponent* EsferaCollision = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Components", meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;
	UFUNCTION()
	virtual void Recogido(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

};
