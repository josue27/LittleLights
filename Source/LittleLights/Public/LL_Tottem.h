// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "Tottem_Piece.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "LL_Tottem.generated.h"

enum class ELLMapsIndexEntry : uint8;
class ALL_PlayerControllerBase;
class ALL_PlayerState;
class AStaticMeshActor;

USTRUCT(BlueprintType)
struct FTottemPieceState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TottemPieceType PieceType;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ATottem_Piece* TotemPice;//The piece that the player has to recover and set to the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AStaticMeshActor* TotemPiecePlaced;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	AActor* TotemPiece_Dummy;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool Delivered;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector EndPosition;
	
	
};

UCLASS()
class LITTLELIGHTS_API ALL_Tottem : public AActor, public ILL_GameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALL_Tottem();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="LL Totem")
	bool discovered;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Components")
	USceneComponent* SceneComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Components")
	UCapsuleComponent* CapsuleCollider;


	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="LL Totem")
	TArray<FTottemPieceState> TotemPieces;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="LL Totem|UI")
	FText TextForInteraction;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="LL Totem")
	bool TotemCompleted;

	
	
	virtual FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual void BeaconCompleted_Implementation() override;

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void TotemDiscoveredEvent();

	UFUNCTION(CallInEditor)
		void SetEndPositions();

	

protected:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="LL Toteam")
	bool bDebug;
	UPROPERTY(BlueprintReadWrite, Category="LL Totem")
	APlayerCharacter* Player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LL Totem")
	ELLMapsIndexEntry LevelCompleted;//when the user completes the totem what level should we take into account as finished?
	UPROPERTY(BlueprintReadWrite, Category = "LL Totem | UI")
		UUserWidget* DiscoverTotemWidget;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void AddTotemPiece(APlayerCharacter* InstigatorPlayer);

	UFUNCTION(BlueprintCallable)
	void TotemCompletion();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void MovePieceAnim(const TMap<TottemPieceType, FVector>& PiecesToMoveInfo);

	UFUNCTION(BlueprintCallable)
	void MovePieceAnimEnded();

	UFUNCTION(BlueprintCallable)
	void SendLevelCompleted();
	
private:
	UPROPERTY()
		ALL_PlayerControllerBase* LLPlayerController;
	UPROPERTY()
		ALL_PlayerState* LLPlayerState;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
