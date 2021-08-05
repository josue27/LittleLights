// Fill out your copyright notice in the Description page of Project Settings.


#include "Torch.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Engine/World.h"
#include "Personaje.h"
#include "PlayerCharacter.h"
//#include "Kismet/GameplayStatics.h"
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
	TorchLight->Intensity = InitialIntensity;
	CurrentTime = LightUpTime;

	//UE_LOG(LogTemp, Log, TEXT("Spawned by %s"), GetOwner()->GetName());
}

// Called every frame
void ATorch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(TorchLight)
	{/*
		if(TorchLight->Intensity > 0)
			TorchLight->Intensity -= LightDecayVelocity * DeltaTime;*/
		LightDecay();
	}
}

void ATorch::StartDecay(float NewLightUpTime)
{
	if (NewLightUpTime != 0)
	{
		LightUpTime = NewLightUpTime;
	}

	RestartLight();
	bStartDecay = true;
}

void ATorch::RestartLight()
{

	CurrentTime = LightUpTime + GetWorld()->GetTimeSeconds();
}

void ATorch::LightDecay()
{
	if (!bStartDecay)
	{
		return;
	}
	float RemainingTime = FMath::Clamp((CurrentTime - GetWorld()->GetTimeSeconds()) / LightUpTime, 0.0f, 1.0f);
	TorchLight->AttenuationRadius = FMath::Lerp(0.0f, 1000.0f, RemainingTime);
	if (RemainingTime <= 0.0f)
	{
		bStartDecay = false;
		//APersonaje* Player = Cast<APersonaje>(	GetWorld()->GetFirstPlayerController());
		APlayerCharacter* Player = Cast<APlayerCharacter>(	GetOwner());
		if (Player)
		{
			Player->TorchOff();
		}
		TorchLight->Intensity = 0.0f;
		TorchLight->bAffectsWorld = false;
		//TorchLight->AttenuationRadius = 0.0f;
		if (SpotLight_Component != nullptr)
		{
			SpotLight_Component->Intensity = 0.0f;
		}
		DeltaIntensity = 0.0f;
		UE_LOG(LogTemp, Warning, TEXT("Torch Light finished...notifying Player"));
	}
	else
	{
		TorchLight->Intensity = FMath::Lerp(0.0f, InitialIntensity, RemainingTime);
		if (SpotLight_Component != nullptr)
		{

		SpotLight_Component->Intensity = FMath::Lerp(0.0f, InitialIntensity, RemainingTime);
		}
		DeltaIntensity = RemainingTime;
	}
}

