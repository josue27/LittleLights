// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/LL_Orb.h"

// Sets default values
ALL_Orb::ALL_Orb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	TorchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Torch Mesh"));
	TorchMesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ALL_Orb::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALL_Orb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

