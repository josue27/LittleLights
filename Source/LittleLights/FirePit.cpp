// Fill out your copyright notice in the Description page of Project Settings.


#include "FirePit.h"
#include "Components/PointLightComponent.h"
#include "Components/SphereComponent.h"
#include "PlayerCharacter.h"
#include "LLComponents/LL_ToolsComponent.h"
#include "Personaje.h"



// Sets default values
AFirePit::AFirePit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	Mesh->SetupAttachment(Root);
	PitLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("MainLight"));
	PitLight->SetupAttachment(Root);
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AFirePit::BeginPlay()
{
	Super::BeginPlay();
	//SphereCollision->OnComponentBeginOverlap.AddDynamic(this,&AFirePit::PlayerInPit);
	//SphereCollision->OnComponentEndOverlap.AddDynamic(this,&AFirePit::PlayerLeftPit);
	
}

// Called every frame
void AFirePit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AFirePit::Interact_Implementation(APawn* InstigatorPawn)
{
	if(bIsTutorial)
	{
		if(bCompleted)
			return;
	}
	
	APlayerCharacter* Player = Cast<APlayerCharacter>(InstigatorPawn);
	if(Player)
	{
		bFillingOrb = true;

		if (GetDistanceTo(InstigatorPawn) > 150)
		{
			FVector DirectionToFirePit = GetActorLocation() - Player->GetActorLocation();
			DirectionToFirePit.Z = 0.f;
			//Rotate character to look at center of this actor
			float YawAngle = FMath::Atan2(DirectionToFirePit.Y, DirectionToFirePit.X) * (180.f / PI);
			Player->SetActorRotation(FRotator(0.f, YawAngle, 0.f));
			

			//Move character closer
			FVector Direction2D = DirectionToFirePit.GetSafeNormal(); // Normal ya sin Z
			FVector NewLocation = GetActorLocation() - (Direction2D * 140.f);
			NewLocation.Z = Player->GetActorLocation().Z; // Mantener altura del Player
			// DrawDebugBox(GetWorld(),NewLocation,FVector(5, 5, 5),FColor::Red,false,100.f,0,3.f);
			// DrawDebugLine(GetWorld(),Player->GetActorLocation(),NewLocation,FColor::Red,false,100.f,0,3.f);
			if (!Player->OnAutomaticMovementEnded.IsAlreadyBound(this,&AFirePit::OnPlayerMovementEnded))
				Player->OnAutomaticMovementEnded.AddUniqueDynamic(this,&AFirePit::OnPlayerMovementEnded);
				
			Player->MovePlayerTo(NewLocation, 400, true, false, false);

			// //Wait until it moves so
			// FTimerDelegate TimerDelegate;
			// TimerDelegate.BindLambda([WeakPlayer = TWeakObjectPtr<APlayerCharacter>(Player), this]()
			// {
			// 	if (!WeakPlayer.IsValid()) return;
			//
			// 	ULL_ToolsComponent* TC = Cast<ULL_ToolsComponent>(WeakPlayer->ToolsComponent);
			// 	if (TC)
			// 	{
			// 		if (TC->Orb == nullptr)
			// 		{
			// 			UE_LOG(LogTemp, Warning, TEXT("FirePit: Orb is null"));
			// 			return;
			// 		}
			// 		TC->StartOrbRefill(RefillAmount);
			// 	}
			// });
			// GetWorldTimerManager().SetTimer(OrbRefillTimerHandle, TimerDelegate, 1.5f, false);
		}
		
	}
	bCompleted = true;
}

FText AFirePit::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	if(bIsTutorial)
	{
		if(bCompleted)
			return FText::GetEmpty();
	}
	APlayerCharacter* Player = Cast<APlayerCharacter>(InstigatorPawn);

	
	if(Player )
	{
		//this is not working because meeeh 
		return InteractionMessage;

	}

	
	return FText::GetEmpty();
	//return ILL_GameplayInterface::GetInteractText_Implementation(InstigatorPawn);
}

void AFirePit::OnPlayerMovementEnded(APlayerCharacter* PlayerCharacter, bool bLightUpTorch, bool bStartDecay)
{
	if (bFillingOrb == false) return;
	
	ULL_ToolsComponent* TC = Cast<ULL_ToolsComponent>(PlayerCharacter->ToolsComponent);
	if (TC)
	{
		if (TC->Orb == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("FirePit: Orb is null"));
			return;
		}
		TC->StartOrbRefill(RefillAmount);
	}
	
	bFillingOrb = false;
}