// Fill out your copyright notice in the Description page of Project Settings.


#include "Tottem_Piece.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"

// Sets default values
ATottem_Piece::ATottem_Piece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComponent;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(SceneComponent);
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereCollider->SetupAttachment(SceneComponent);
	SphereCollider->SetCollisionResponseToAllChannels(ECR_Block);
	

}

void ATottem_Piece::Interact_Implementation(APawn* InstigatorPawn)
{
//TODO: but we should change this to a temp table or array that holds the info of the piece grabed and then destroy the BP, it would be easier than changing collision systems
	if(!bIsVisible)return;
	ILL_GameplayInterface::Interact_Implementation(InstigatorPawn);
	APlayerCharacter* Player = Cast<APlayerCharacter>(InstigatorPawn);
	if(Player)
	{
		Player->AddTottemPiece(this);
		Deactivate_Implementation();
		bTaken = true;

	}
	
}

FText ATottem_Piece::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(InstigatorPawn);

	if(bTaken)
	{
		return FText::GetEmpty();

	}
	if(Player && !bIsVisible && !bTaken)
	{
		//this is not working because
  		return NSLOCTEXT("InteractableActors","Totem","You need moon water to grab this!");

	}

	if(Player && !InteractionMessage.IsEmpty())
	{
		return InteractionMessage;
	

	}
	return FText::GetEmpty();
}


// Called when the game starts or when spawned
void ATottem_Piece::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATottem_Piece::Deactivate_Implementation()
{
	RootComponent->SetVisibility(false,true);
	SphereCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GrabbedParticleSystem, GetActorLocation());
}


// Called every frame
void ATottem_Piece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

