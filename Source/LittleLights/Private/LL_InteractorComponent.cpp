// Fill out your copyright notice in the Description page of Project Settings.


#include "LL_InteractorComponent.h"

#include "DrawDebugHelpers.h"
#include "LL_GameplayInterface.h"

// Sets default values for this component's properties
ULL_InteractorComponent::ULL_InteractorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULL_InteractorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULL_InteractorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULL_InteractorComponent::PrimaryInteract()
{
	APawn* PlayerOwner = Cast<APawn>(GetOwner());

	FVector Start = PlayerOwner->GetActorLocation();
	FVector End = Start + (PlayerOwner->GetActorForwardVector() * 200.0f);

	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	Shape.SetSphere(20.0f);
	FCollisionObjectQueryParams QueryParams;
	
	
	bool bInteractionHit = GetWorld()->SweepMultiByObjectType(Hits,Start,End,FQuat::Identity,QueryParams,Shape);
	for(FHitResult Hit:Hits)
	{
		AActor* Actor = Hit.GetActor();
		if(Actor->Implements<ULL_GameplayInterface>())
		{
			
			ILL_GameplayInterface::Execute_Interact(Actor,PlayerOwner);
			UE_LOG(LogTemp,Warning,TEXT("Hitted with Interactive Object"));
			break;
		}
		DrawDebugSphere(GetWorld(),Hit.ImpactPoint,30.0f,16,FColor::Red,false,2.0f);
	}
	DrawDebugLine(GetWorld(),Start,End,FColor::Emerald,false,2.0f);
}

