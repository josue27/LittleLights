// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/LL_CrouchCross_Ability.h"
#include "SpecialMovementZones/LL_SpecialMovementZone.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h"
#include "LLGamePlayFunctionLibrary.h"
#include "Widget/LL_WorldUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "LittleLights/LL_PlayerControllerBase.h"
#include "PlayerCharacter.h"

void ULL_CrouchCross_Ability::StartAbility_Implementation(AActor* Instigator, AActor* SecondActor)
{
	Super::StartAbility_Implementation(Instigator, SecondActor);

	SpecialMovementZone = Cast< ALL_SpecialMovementZone>(SecondActor);
	if (SpecialMovementZone == nullptr)return;
	InKeyPressed = 0;

	Player = Cast<APlayerCharacter>(AbilityComponent->GetOwner());
	if (Player)
	{
		Player->bCanMove = false;
		Player->Crouch();// Crouch();
		Player->bIsCrossingUnder = true;
		Player->DisableInteraction(true);
		//TODO: make the calls of this delegates on the plaeyr
		Player->OnAutomaticMovementEnded.AddDynamic(this,&ULL_CrouchCross_Ability::PlayerEndedMovement);
		Player->OnKeyPressed.AddDynamic(this,&ULL_CrouchCross_Ability::KeyPressed);
	}
	SpecialMovementZone->BlockerCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpecialMovementZone->TriggerCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	//Set Position from spline
	USplineComponent* Spline = SpecialMovementZone->SplinePath;
	Spline->GetSplineLength();
	bool bCloseToEnd = ULLGamePlayFunctionLibrary::IsCloserToEnd(Player->GetActorLocation(), Spline);
	if (bCloseToEnd)
	{
		PathPositions.Add(Spline->GetLocationAtDistanceAlongSpline(Spline->GetSplineLength() * 0.60f, ESplineCoordinateSpace::World));
		PathPositions.Add(Spline->GetLocationAtDistanceAlongSpline(Spline->GetSplineLength() * 0.30f, ESplineCoordinateSpace::World));
		PathPositions.Add(Spline->GetLocationAtDistanceAlongSpline(0.f, ESplineCoordinateSpace::World));
		
	}
	else
	{
		PathPositions.Add(Spline->GetLocationAtDistanceAlongSpline(Spline->GetSplineLength() * 0.30f, ESplineCoordinateSpace::World));
		PathPositions.Add(Spline->GetLocationAtDistanceAlongSpline(Spline->GetSplineLength() * 0.60f, ESplineCoordinateSpace::World));
		PathPositions.Add(Spline->GetLocationAtDistanceAlongSpline(Spline->GetSplineLength(), ESplineCoordinateSpace::World));
	}
	APlayerController* PC = UGameplayStatics::GetPlayerController(AbilityComponent->GetOwner(), 0);
	LLPlayerController = Cast<ALL_PlayerControllerBase>(PC);
	if (LLPlayerController)
	{
		LLPlayerController->ShowKeyToPressUI("Press " + KeyToPress[InKeyPressed].ToString(), Player);
	}
}

void ULL_CrouchCross_Ability::PlayerEndedMovement(APlayerCharacter* PlayerCaller,bool bLightUpOrb, bool bStartOrbDecay)
{
	
	if (InKeyPressed == KeyToPress.Num())
	{
		AbilityComponent->StopAbilityByName(Player, "Crouch", SpecialMovementZone);

	
		if (LLPlayerController)
		{
			LLPlayerController->RemoveKeyToPressUI();
		}
		
		return;

	}
	if (LLPlayerController)
	{
		LLPlayerController->ShowKeyToPressUI("Press "+KeyToPress[InKeyPressed].ToString(), Player);
	}
	bCanReceiveInput = true;

}
void ULL_CrouchCross_Ability::KeyPressed(FKey KeyPressed)
{
	if (KeyPressed.GetFName() == FKey("E") || KeyPressed.GetFName() == FKey("e"))return;

	if (KeyToPress.IsValidIndex(InKeyPressed) && KeyPressed == KeyToPress[InKeyPressed])
	{

	    Player = Player == nullptr? Cast<APlayerCharacter>(AbilityComponent->GetOwner()): Player;
		if (Player)
		{
			Player->MovePlayerTo(PathPositions[InKeyPressed], 100.f, true,false,false);
			bCanReceiveInput = false;
		}
		

		InKeyPressed += 1;
	

	}
}

void ULL_CrouchCross_Ability::StopAbility_Implementation(AActor* Instigator, AActor* SecondActor)
{
	Super::StopAbility_Implementation(Instigator, SecondActor);

	Player->UnCrouch();
	Player->ResetWalkSpeed(400.0f);
	Player->bIsCrossingUnder = false;
	Player->bCanMove = true;
	//Player->OnAutomaticMovementEnded.RemoveDynamic(this, &ULL_CrouchCross_Ability::PlayerEndedMovement);
	Player->OnKeyPressed.RemoveDynamic(this, &ULL_CrouchCross_Ability::KeyPressed);
	Player->DisableInteraction(false);
	

	//Call the stop interaction, Mainly for the Beast slowmotion
	

	Player = nullptr;
	
	SpecialMovementZone->PlayerEndedTask();
	SpecialMovementZone = nullptr;
	
	InKeyPressed = 0;
	PathPositions.Empty();
}