// Fill out your copyright notice in the Description page of Project Settings.


#include "LL_InteractorComponent.h"

#include "DrawDebugHelpers.h"
#include "InterchangeResult.h"
#include "LLGamePlayFunctionLibrary.h"
#include "LL_GameplayInterface.h"
#include "PlayerCharacter.h"
#include "Tottem_Piece.h"
#include "Blueprint/UserWidget.h"
#include "SpecialMovementZones/LL_SpecialMovementZone.h"
#include "Widget/LL_WorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarDrawDebugInteraction(TEXT("ll.DrawDebugInteraction"),false,TEXT("Drawing Debug InteractiveComponent"),ECVF_Cheat);


// Sets default values for this component's properties
ULL_InteractorComponent::ULL_InteractorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bInteractionStarted = false;
	// ...
}


// Called when the game starts
void ULL_InteractorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	bCanInteract = true;
}



// Called every frame
void ULL_InteractorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(bCanInteract)
		FindInteractable();
}

void ULL_InteractorComponent::FindInteractable()
{
	APawn* PlayerOwner = Cast<APawn>(GetOwner());

	FVector Start = PlayerOwner->GetActorLocation();
	FVector End = Start + (PlayerOwner->GetActorForwardVector() * DistanceRay);

	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	Shape.SetSphere(RadiusRay);
	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	
	InteractableActor = nullptr;
	bool bInteractionHit = GetWorld()->SweepMultiByObjectType(Hits,Start,End,FQuat::Identity,QueryParams,Shape);
	for(FHitResult Hit:Hits)
	{
		if(CVarDrawDebugInteraction.GetValueOnGameThread())
		{
			DrawDebugSphere(GetWorld(),Hit.ImpactPoint,30.0f,16,FColor::Red,false,2.0f);

		}
		
		AActor* Actor = Hit.GetActor();
		if(Actor->Implements<ULL_GameplayInterface>() && !Cast<APlayerCharacter>(Actor))
		{
			if(ALL_SpecialMovementZone* SpecialMZ = Cast<ALL_SpecialMovementZone>(Actor))
			{
				if(SpecialMZ->bIsSingleUse && SpecialMZ->bUsed)
				{
					break;
				}
				InteractableActor = Actor;
			}
			else
			{
				InteractableActor = Actor;
			}
			
			
			
			//UE_LOG(LogTemp,Warning,TEXT("Hitted with Interactive Object"));
			break;
		}
	}
	//IF the actor is a TottemPiece we check this, if is true that means that is another type of actor o it is grabbable
	/*if(!ULLGamePlayFunctionLibrary::IsTotemPieceGrabable(InteractableActor))
	{
		InteractableActor = nullptr;
	}*/
	//Spawn Widget
	if(InteractableActor)
	{
		
		if(InteractionWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			InteractionWidgetInstance = CreateWidget<ULL_WorldUserWidget>(GetWorld(),DefaultWidgetClass);
			InteractionWidgetInstance->AttachedActor = InteractableActor;
		}
		if(InteractionWidgetInstance)
		{
			InteractionWidgetInstance->AttachedActor = InteractableActor;//duplicated??
			if(!InteractionWidgetInstance->IsInViewport())
			{
				InteractionWidgetInstance->AddToViewport();
			}
			InteractionWidgetInstance->TextOfInteraction = ILL_GameplayInterface::Execute_GetInteractText(InteractableActor,PlayerOwner);
			//If there is no text remove | Todo: Should we paste a default text?
			if(InteractionWidgetInstance->TextOfInteraction.IsEmpty())
			{
				InteractionWidgetInstance->TextOfInteraction = FText::FromString("PRESS E");
				UE_LOG(LogTemp,Warning,TEXT("%s no interact text found, set default"),*GetName());
			}
		}
	}else
	{
		if(InteractionWidgetInstance)
		{
			InteractionWidgetInstance->RemoveFromParent();
			bInteractionStarted = false;
		}
	}
	if(CVarDrawDebugInteraction.GetValueOnGameThread())
		DrawDebugLine(GetWorld(),Start,End,FColor::Emerald,false,2.0f);
	
}

void ULL_InteractorComponent::PrimaryInteract()
{
	if(InteractableActor)
	{
		APawn* OwnerPawn = Cast<APawn>(GetOwner());

		//IF is a Totem Piece
		ATottem_Piece* TP = Cast<ATottem_Piece>(InteractableActor);
		if(TP )
		{
			if(TP->bTaken)
			{
				UE_LOG(LogTemp,Warning,TEXT("Hitted with Totem Object already TAKEN"));
			
				return;

			}
			if(!TP->bIsVisible)
			{
				UE_LOG(LogTemp,Warning,TEXT("Hitted with Totem not visible"));
			
				return;
			}
			
		
		}

		//It might be a special zone , pond or something else(for the moment)
		ILL_GameplayInterface::Execute_Interact(InteractableActor,OwnerPawn);
		if(InteractionWidgetInstance)
		{
			InteractionWidgetInstance->RemoveFromParent();
		}
		bInteractionStarted = true;
	}
	
}

