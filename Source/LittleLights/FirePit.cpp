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
		ULL_ToolsComponent* TC = Cast<ULL_ToolsComponent>(Player->ToolsComponent);
		//ULL_AbilityComponent* AC = Cast<ULL_AbilityComponent>( Player->GetComponentByClass(ULL_AbilityComponent::StaticClass()));
		if(TC)
		{
			TC->StartOrbRefill(RefillAmount);
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
		//this is not working because
		return InteractionMessage;

	}

	
	return FText::GetEmpty();
	//return ILL_GameplayInterface::GetInteractText_Implementation(InstigatorPawn);
}