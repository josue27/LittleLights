// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LL_GameplayInterface.h"
#include "DialogueSystem/LL_DialogueComponent.h"
#include "GameFramework/Character.h"
#include "LL_TrollCharacter.generated.h"

UCLASS()
class LITTLELIGHTS_API ALL_TrollCharacter : public ACharacter, public ILL_GameplayInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALL_TrollCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="LL Dialogue")
	ULL_DialogueComponent* DialogueComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region GameplayInterface
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual void InteractionEnded_Implementation(AActor* InstigatorPawn, bool bSlowTime) override;

	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
#pragma endregion 

};
