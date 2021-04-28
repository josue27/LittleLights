// Fill out your copyright notice in the Description page of Project Settings.


#include "Torch.h"
#include "Components/PointLightComponent.h"
// Sets default values
ATorch::ATorch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	TorchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Torch Mesh"));
	TorchMesh->SetupAttachment(Root);

	TorchLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Main Light"));
	TorchLight->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void ATorch::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATorch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(TorchLight)
	{
		if(TorchLight->Intensity > 0)
			TorchLight->Intensity -= LightDecayVelocity * DeltaTime;
	}
}

