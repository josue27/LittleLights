// Fill out your copyright notice in the Description page of Project Settings.


#include "LLComponents/LL_ToolsComponent.h"
#include "PlayerCharacter.h"
#include "Tools/LL_Orb.h"
#include "Engine/SkeletalMeshSocket.h"
#include "LL_PlayerState.h"
#include "LittleLights/LittleLights.h"

// Sets default values for this component's properties
ULL_ToolsComponent::ULL_ToolsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULL_ToolsComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bStartWithOrb)
	{
		SpawnOrb();
		//RefillOrb(30.f,false);

	}
	
}


// Called every frame
void ULL_ToolsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


#pragma region Orb

void ULL_ToolsComponent::SpawnOrb()
{

	if (OrbClass == nullptr || PlayerCharacter == nullptr)
	{
		return;
	}
	Orb = GetWorld()->SpawnActor<ALL_Orb>(OrbClass);
	if (Orb == nullptr) return;

	Orb->ToolsComponent = this;
	const USkeletalMeshSocket* HandSocket = PlayerCharacter->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(Orb, PlayerCharacter->GetMesh());
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Yellow, "Orb spawned");
	}
}
void ULL_ToolsComponent::StartOrbDecay()
{
	if (Orb)
	{
		Orb->StartDecay();
	}
}

void ULL_ToolsComponent::StartOrbRefill(float Amount)
{
	if (Orb == nullptr)return;
	Orb->StopDecay();
	TempRefillAmount = Amount;
	APlayerCharacter* PC = Cast<APlayerCharacter>(GetOwner());
	if (PC)
	{
		PC->StopCharacter();
		PC->bLightingTorch = true;
		ALL_PlayerState* PS = Cast<ALL_PlayerState>(PC->GetPlayerState());
		if (PS)
		{
			PS->OnRefillingOrb.Broadcast(true);
		}
		//PC->bUpdateFov = false;
	}
	LogOnScreen(GetWorld(),"Orb refill started");
}

void ULL_ToolsComponent::RefillOrb(float Amount, bool bStartDecay)
{
	if (Orb)
	{
		Orb->RefillOrb(Amount);
		

		if(bStartDecay) StartOrbDecay();

		

		APlayerCharacter* PC = Cast<APlayerCharacter>(GetOwner());
		if (PC)
		{
			ALL_PlayerState* PS = Cast<ALL_PlayerState>(PC->GetPlayerState());
			if (PS)
			{
				PS->OnRefillingOrb.Broadcast(false);
				PS->OnOrbRefillFinished.Broadcast(true);
			}
		}

	}
	LogOnScreen(GetWorld(), "Orb refilled");

}

void ULL_ToolsComponent::OrbRefillFinished()
{
	
	RefillOrb(TempRefillAmount);

	APlayerCharacter* PC = Cast<APlayerCharacter>(GetOwner());
		if (PC)
		{
			PC->bLightingTorch = false;	
			PC->ContinueMovement();
			PC->ResetCameraPosition();
			//PC->bUpdateFov = true;
			ALL_PlayerState* PS = Cast<ALL_PlayerState>(PC->GetPlayerState());
			if (PS)
			{
				PS->OnRefillingOrb.Broadcast(false);
				PS->OnOrbRefillFinished.Broadcast(true);
			}
		}	

		LogOnScreen(GetWorld(), "Orb refilled finish called");

	

}

float ULL_ToolsComponent::GetDeltaRemainOrb()
{
	if (Orb)
	{
		return Orb->RemainingDeltaTime;
	}
	return 0.0f;
}

#pragma endregion 

