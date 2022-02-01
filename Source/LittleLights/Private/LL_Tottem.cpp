// Fill out your copyright notice in the Description page of Project Settings.


#include "LL_Tottem.h"

#include "LL_PlayerState.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "LittleLights/LittleLights.h"
#include "LittleLights/LL_GameModeBase.h"

// Sets default values
ALL_Tottem::ALL_Tottem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = SceneComponent;
	CapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleColider"));
	CapsuleCollider->SetupAttachment(SceneComponent);

}
// Called when the game starts or when spawned
void ALL_Tottem::BeginPlay()
{
	Super::BeginPlay();
	
}


void ALL_Tottem::Interact_Implementation(APawn* InstigatorPawn)
{
	//ILL_GameplayInterface::Interact_Implementation(InstigatorPawn);
	if(Player== nullptr)
	{
		 Player = Cast<APlayerCharacter>(InstigatorPawn);
	}

	if(Player->TottemPieces.Num() > 0)
	{
		AddTotemPiece_Implementation(Player,Player->TottemPieces[0]);
		Player->TottemPieces.RemoveAt(0);
		Player->TottemPieces.Sort();
		UE_LOG(LogTemp,Warning,TEXT("Got Totem Piece from player "));
	}
}


FText ALL_Tottem::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	if(TotemCompleted)
	{
		return NSLOCTEXT("InteractableActors","Totem","Totem completed");

	}
	return TextForInteraction;
}

void ALL_Tottem::AddTotemPiece_Implementation(APawn* InstigatorActor,ATottem_Piece* TotemPiece)
{
	for(int32 i=0;i<TotemPieces.Num();i++)
	{
		if(TotemPieces[i].PieceType == TotemPiece->PieceType && TotemPieces[i].TotemPice == nullptr)
		{
			ALL_PlayerState* PS = Cast<ALL_PlayerState>(InstigatorActor->GetPlayerState());
			if(PS)
			{
				PS->RemoveTotemPiece();
				UE_LOG(LogTemp,Warning,TEXT(" PS"));

			}
			TotemPieces[i].TotemPice = TotemPiece;
			if(TotemPieces[i].TotemPiece_Dummy != nullptr)
			{
				MovePieceAnim(TotemPieces[i].TotemPiece_Dummy);
			}
			UE_LOG(LogTemp,Warning,TEXT("piece found and added to tottem"));

			break;
		}
	}
	

}


void ALL_Tottem::TotemCompletion()
{

	int32 NumPiecesInPlace = 0;
	for(FTottemPieceState TotemState : TotemPieces)
	{
		if(TotemState.TotemPice != nullptr)
		{
			NumPiecesInPlace +=1;
		}
	}
	if(NumPiecesInPlace >= TotemPieces.Num())
	{
		ALL_GameModeBase* GM = Cast<ALL_GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		Execute_BeaconCompleted(GM);
		Execute_BeaconCompleted(this);
		TotemCompleted = true;
	}
	
	
}

void ALL_Tottem::BeaconCompleted_Implementation()
{

	LogOnScreen(GetWorld(),"Beacon completed");
}

void ALL_Tottem::MovePieceAnimEnded()
{

	if(Player->TottemPieces.Num() == 0)
	{
		TotemCompletion();
	}else
	{
		//TODO:Warning this might be recursive
		Interact_Implementation(Player);
	}
}

// Called every frame
void ALL_Tottem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

