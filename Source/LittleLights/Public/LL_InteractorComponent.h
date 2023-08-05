// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Widget/LL_WorldUserWidget.h"
#include "LL_InteractorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LITTLELIGHTS_API ULL_InteractorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULL_InteractorComponent();
	UPROPERTY(BlueprintReadWrite)
	bool bCanInteract;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Interactor Component")
	float RadiusRay = 30.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Interactor Component")
	float DistanceRay = 200.0f;
	//UI that shows fot the player in 2Dpos
	UPROPERTY(EditDefaultsOnly,Category="UI")
	TSubclassOf<ULL_WorldUserWidget> DefaultWidgetClass;
	UPROPERTY()
	ULL_WorldUserWidget* InteractionWidgetInstance;

	UFUNCTION()
	void FindInteractable();

	UPROPERTY()
	AActor* InteractableActor;

	UPROPERTY(BlueprintReadWrite)
	bool bInteractionStarted ;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PrimaryInteract();


};
