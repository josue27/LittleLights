// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/LL_Orb.h"
#include "LLComponents/LL_ToolsComponent.h"
// Sets default values
ALL_Orb::ALL_Orb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	TorchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Torch Mesh"));
	TorchMesh->SetupAttachment(Root);

	if (LiquidMesh)
	{

		InstanceLiquidMaterial = UMaterialInstanceDynamic::Create(LiquidMesh->GetMaterial(0), this);
	}
}

// Called when the game starts or when spawned
void ALL_Orb::BeginPlay()
{
	Super::BeginPlay();
	//Initial Light
	RemainingLightTime = MaxLightUpTime + GetWorld()->GetTimeSeconds();
	//UpdateLight(1.0f);
}

// Called every frame
void ALL_Orb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bStartDecay)
	{
		UpdateLight(DeltaTime);
	}
	
}

void ALL_Orb::UpdateLight(float DeltaTime)
{
	RemainingDeltaTime = FMath::Clamp((RemainingLightTime - GetWorld()->GetTimeSeconds()) / MaxLightUpTime, 0.0f, 1.0f);
	
	RemainingDeltaTime = FMath::FInterpTo(RemainingDeltaTime, 0.f, DeltaTime, 0.5f);
	if (ToolsComponent)
	{

		ToolsComponent->OnOrbRemainingTimeChanged.Broadcast(this, RemainingDeltaTime);
	}
	if (InstanceLiquidMaterial)
	{
		//InstanceLiquidMaterial->SetVectorParameterValue(FName(TEXT("Emissive_Ctrl"),);
	}
	
}

void ALL_Orb::StartDecay()
{
	//RemainingLightTime = MaxLightUpTime + GetWorld()->GetTimeSeconds();
	bStartDecay = true;
}

void ALL_Orb::StopDecay()
{
	bStartDecay = false;
}

void ALL_Orb::RefillOrb(float Amount)
{

	

	//this should work 
	float AmountToAdd = FMath::Clamp(Amount + RemainingLightTime, 1.f, MaxLightUpTime);
	RemainingLightTime = AmountToAdd + GetWorld()->GetTimeSeconds();

	UpdateLight(1.f);

}

