// Fill out your copyright notice in the Description page of Project Settings.


#include "Tottem_Piece.h"

#include "PlayerCharacter.h"

// Sets default values
ATottem_Piece::ATottem_Piece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ATottem_Piece::Interact_Implementation(APawn* InstigatorPawn)
{
	ILL_GameplayInterface::Interact_Implementation(InstigatorPawn);
	APlayerCharacter* Player = Cast<APlayerCharacter>(InstigatorPawn);
	if(Player)
	{
		Player->AddTottemPiece(this);
		Deactivate_Implementation();
	}
	
}

// Called when the game starts or when spawned
void ATottem_Piece::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATottem_Piece::Deactivate_Implementation()
{
	RootComponent->SetVisibility(false,true);

}


// Called every frame
void ATottem_Piece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

