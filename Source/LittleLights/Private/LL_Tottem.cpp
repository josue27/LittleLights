// Fill out your copyright notice in the Description page of Project Settings.


#include "LL_Tottem.h"

#include "LLGameManager.h"
#include "LL_PlayerState.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "LittleLights/LittleLights.h"
#include "LittleLights/LL_GameModeBase.h"
#include "LittleLights/LL_PlayerControllerBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"
#include "LL_PlayerState.h"
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
	LLPlayerState = LLPlayerState ==nullptr? Cast<ALL_PlayerState>(UGameplayStatics::GetPlayerState(this, 0)) : LLPlayerState;
	if (LLPlayerState)
	{
		LLPlayerState->LevelPiecesToFind = TotemPieces.Num();
	}

	for (FTottemPieceState& TootemPiece : TotemPieces)
	{
		if (TootemPiece.TotemPiecePlaced) {
			TootemPiece.TotemPiecePlaced->SetActorHiddenInGame(true);
			TootemPiece.TotemPiecePlaced->SetActorEnableCollision(false);
		}
	}
}


void ALL_Tottem::Interact_Implementation(APawn* InstigatorPawn)
{
	//ILL_GameplayInterface::Interact_Implementation(InstigatorPawn);
	if(Player== nullptr)
	{
		 Player = Cast<APlayerCharacter>(InstigatorPawn);

	}
	if (Player == nullptr)return;




	if(!discovered)
	{
		UE_LOG(LogTemp,Warning,TEXT("Totem discovered "));
		discovered = true;
		TotemDiscoveredEvent();//implentation in BP
		LLPlayerController = LLPlayerController == nullptr ? Cast<ALL_PlayerControllerBase>(UGameplayStatics::GetPlayerController(this,0)): LLPlayerController;
		if (LLPlayerController)
		{
			LLPlayerController->ShowTotemPiecesHUD(true);
		}
	}else if(Player->TottemPieces.Num() > 0)
	{
		AddTotemPiece_Implementation(Player);
		
	}

	LLPlayerState = LLPlayerState ==nullptr? Cast<ALL_PlayerState>(UGameplayStatics::GetPlayerState(this, 0)) : LLPlayerState;
	if (LLPlayerState)
	{
		LLPlayerState->OnInteractionStarted.Broadcast(this,true);
		
		FTimerHandle InteractionTimeHandler;
		FTimerDelegate TimerCallback;
		TimerCallback.BindLambda([&]
		{
			// callback;
			LLPlayerState->OnInteractionEnded.Broadcast(this,false);
		});
		GetWorldTimerManager().SetTimer(InteractionTimeHandler,TimerCallback,2.f,false);
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

void ALL_Tottem::AddTotemPiece_Implementation(APlayerCharacter* InstigatorPlayer)
{
	UE_LOG(LogTemp,Warning,TEXT("Tottem pieces:%i"),TotemPieces.Num());
	LLPlayerController = LLPlayerController == nullptr ? Cast<ALL_PlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0)) : LLPlayerController;

	TMap<TottemPieceType,FVector> PiecesToMove;
	for(auto& PlayerTotemPiece : InstigatorPlayer->TottemPieces)
	{
		for(int32 i = 0; i < TotemPieces.Num(); i++)
		{
			//if type and there is no piece in place
			if(PlayerTotemPiece->PieceType == TotemPieces[i].PieceType   && !TotemPieces[i].Delivered)
			{
				ALL_PlayerState* PS = Cast<ALL_PlayerState>(Player->GetPlayerState());
				if(PS)
				{
					PS->RemoveTotemPiece();
					UE_LOG(LogTemp,Warning,TEXT(" PS"));
					LLPlayerController->TotemPiecesDeliveredHUD();

				}
			
				TotemPieces[i].Delivered = true;
			
				PiecesToMove.Add(TotemPieces[i].PieceType,FVector::Zero());

				UE_LOG(LogTemp,Warning,TEXT("piece found and added to tottem"));

			
				// InstigatorPlayer->TottemPieces.RemoveAt(0); 
				// InstigatorPlayer->TottemPieces.Sort();
				UE_LOG(LogTemp,Warning,TEXT("Got Totem Piece from player "));

				//break;
			}
		}
	}
	InstigatorPlayer->TottemPieces.Empty();
	MovePieceAnim( PiecesToMove);
	
}

//Called in blueprint for the moment
void ALL_Tottem::MovePieceAnimEnded()
{
	//This is because we want all the pieces animation to finish and
	//by logic the last one will be removed and TottemPieces will be 0 so we could
	//check the completition
	return;
	if(Player->TottemPieces.Num() == 0)
	{
		TotemCompletion();


	}else
	{
		//TODO:Warning this might be recursive
		Interact_Implementation(Player);
	}
	
}
/**
 *This will tell the gamemanager that we completed the totem, for the mean time we consider this level completed
 *and will save the game
 */

void ALL_Tottem::SendLevelCompleted()
{
	if(bDebug)return;
	
	if(ULLGameManager* GameManager = GetGameInstance()->GetSubsystem<ULLGameManager>())
	{
		GameManager->LevelCompleted(LevelCompleted);
		GameManager->SaveGame();
	}
		
}

/**
 * Checks if the totem has been completed and if successful trigger completion events
 */
void ALL_Tottem::TotemCompletion()
{

	int32 NumPiecesInPlace = 0;
	for(FTottemPieceState TotemState : TotemPieces)
	{
		if(TotemState.Delivered )
		{
			NumPiecesInPlace +=1;
		}
	}
	if(NumPiecesInPlace >= TotemPieces.Num())
	{
		ALL_GameModeBase* GM = Cast<ALL_GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		//remember that you have to call this in each actor that has implemented this cause this is an interface not a MulticasEvent
		Execute_BeaconCompleted(GM);
		Execute_BeaconCompleted(this);
		
		TotemCompleted = true;

		LLPlayerController = LLPlayerController == nullptr ? Cast<ALL_PlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0)) : LLPlayerController;
		LLPlayerController->ShowTotemPiecesHUD(true);
		SendLevelCompleted();
	}
	
	
}

void ALL_Tottem::BeaconCompleted_Implementation()
{

	LogOnScreen(GetWorld(),"Beacon completed");
}

// Called every frame
void ALL_Tottem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALL_Tottem::SetEndPositions()
{
	for (auto& TotemPiece : TotemPieces)
	{
		if (TotemPiece.TotemPiecePlaced != nullptr)
		{
			TotemPiece.EndPosition = TotemPiece.TotemPiecePlaced->GetActorLocation();
		}
	}
}