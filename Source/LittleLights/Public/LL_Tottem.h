// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "Tottem_Piece.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "LL_Tottem.generated.h"


USTRUCT(BlueprintType)
struct FTottemPieceState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TottemPieceType PieceType;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ATottem_Piece* TotemPice;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	AActor* TotemPiece_Dummy;
	
};

UCLASS()
class LITTLELIGHTS_API ALL_Tottem : public AActor, public ILL_GameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALL_Tottem();

	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Components")
	USceneComponent* SceneComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Components")
	UCapsuleComponent* CapsuleCollider;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Totem")
	TArray<FTottemPieceState> TotemPieces;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UI")
	FText TextForInteraction;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Totem")
	bool TotemCompleted;
	
	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual void BeaconCompleted_Implementation() override;

protected:


	UPROPERTY(BlueprintReadWrite, Category="Totem")
	APlayerCharacter* Player;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void AddTotemPiece(APawn* InstigatorActor, ATottem_Piece* TotemPiece);

	UFUNCTION(BlueprintCallable)
	void TotemCompletion();

	UFUNCTION(BlueprintImplementableEvent)
	void MovePieceAnim(AActor* TotemPiece);

	UFUNCTION(BlueprintCallable)
	void MovePieceAnimEnded();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
