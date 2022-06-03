// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LL_GameplayInterface.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Tottem_Piece.generated.h"


UENUM(BlueprintType)
enum class TottemPieceType: uint8
{
	PieceA = 0 UMETA(DisplayName ="Piece A"),
	PieceB = 1 UMETA(DisplayName ="Piece B"),
	PieceC = 2 UMETA(DisplayName ="Piece C"),
	PieceD = 3 UMETA(DisplayName ="Piece D"),
	PieceE = 4 UMETA(DisplayName ="Piece E")

	
};

UCLASS()
class LITTLELIGHTS_API ATottem_Piece : public AActor, public ILL_GameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATottem_Piece();


	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Totem Piece")
	TottemPieceType PieceType;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Totem Piece")
	bool bIsVisible;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USceneComponent* SceneComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USphereComponent* SphereCollider;

	

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	 FText GetInteractText_Implementation(APawn* InstigatorPawn) ;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void Deactivate();

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	FText InteractionMessage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bTaken = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDelivered = false;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
