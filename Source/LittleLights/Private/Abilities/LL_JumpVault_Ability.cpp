// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/LL_JumpVault_Ability.h"

#include "LLGamePlayFunctionLibrary.h"
#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LittleLights/LL_PlayerControllerBase.h"
#include "SpecialMovementZones/LL_SpecialMovementZone.h"

void ULL_JumpVault_Ability::StartAbility_Implementation(AActor* Instigator, AActor* SecondActor)
{
	Super::StartAbility_Implementation(Instigator, SecondActor);

	SpecialMovementZone = Cast< ALL_SpecialMovementZone>(SecondActor);
	if (SpecialMovementZone == nullptr)return;
	InKeyPressed = 0;
	Player = Cast<APlayerCharacter>(AbilityComponent->GetOwner());
	if (Player)
	{
		Player->bCanMove = false;
		Player->DisableInteraction(true);
		//TODO: make the calls of this delegates on the plaeyr
		 Player->OnAutomaticMovementEnded.AddDynamic(this,&ULL_JumpVault_Ability::PlayerEndedMovement);
		Player->OnKeyPressed.AddDynamic(this,&ULL_JumpVault_Ability::KeyPressed);
	}
	SpecialMovementZone->BlockerCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpecialMovementZone->TriggerCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	USplineComponent* Spline = SpecialMovementZone->SplinePath;
	Spline->GetSplineLength();
	bool bCloseToEnd = ULLGamePlayFunctionLibrary::IsCloserToEnd(Player->GetActorLocation(), Spline);
	if (bCloseToEnd)
	{
		PathPositions.Add(Spline->GetLocationAtDistanceAlongSpline(Spline->GetSplineLength() , ESplineCoordinateSpace::World));
		PathPositions.Add(Spline->GetLocationAtDistanceAlongSpline(0.f, ESplineCoordinateSpace::World));
		
	}
	else
	{
		PathPositions.Add(Spline->GetLocationAtDistanceAlongSpline(0.f, ESplineCoordinateSpace::World));
		PathPositions.Add(Spline->GetLocationAtDistanceAlongSpline(Spline->GetSplineLength(), ESplineCoordinateSpace::World));
	}
	APlayerController* PC = UGameplayStatics::GetPlayerController(AbilityComponent->GetOwner(), 0);
	LLPlayerController = Cast<ALL_PlayerControllerBase>(PC);
	if (LLPlayerController)
	{
		LLPlayerController->ShowKeyToPressUI("Press " + KeysToPress[InKeyPressed].ToString(), Player);
	}
}



void ULL_JumpVault_Ability::StopAbility_Implementation(AActor* Instigator, AActor* SecondActor)
{
	Super::StopAbility_Implementation(Instigator, SecondActor);
	
d	Player->ResetWalkSpeed(400.0f);
	Player->bJumpingOver = false;
	Player->bCanMove = true;
	//Player->OnAutomaticMovementEnded.RemoveDynamic(this, &ULL_JumpVault_Ability::PlayerEndedMovement);
	Player->OnKeyPressed.RemoveDynamic(this, &ULL_JumpVault_Ability::KeyPressed);
	Player->DisableInteraction(false);


	//Call the stop interaction, Mainly for the Beast slowmotion
	

	Player = nullptr;
	SpecialMovementZone->BlockerCollider->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	SpecialMovementZone->TriggerCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	SpecialMovementZone = nullptr;
	InKeyPressed = 0;
	PathPositions.Empty();
}

void ULL_JumpVault_Ability::KeyPressed(FKey KeyPressed)
{

	if (KeyPressed.GetFName() == FKey("E") || KeyPressed.GetFName() == FKey("e"))return;

	if (KeysToPress.IsValidIndex(InKeyPressed) && KeyPressed == KeysToPress[InKeyPressed])
	{

	
		
		if(InKeyPressed >= KeysToPress.Num()-1)//This means we have pressed all key correctly
		{
			
			//Player->SetActorLocation();
			Player->bJumpingOver = true;
			Player = Cast<APlayerCharacter>(AbilityComponent->GetOwner());
			if (Player)
			{
				Player->MovePlayerTo(PathPositions[1], 100.f, true,false,false);
				bCanReceiveInput = false;
			}

			
			
		}
		else
		{
			InKeyPressed += 1;
			if (LLPlayerController)
			{
				LLPlayerController->ShowKeyToPressUI("Press "+KeysToPress[InKeyPressed].ToString(), Player);
			}
			bCanReceiveInput = true;
		}

	}
	else
	{
		AbilityComponent->StopAbilityByName(Player, "JumpOver", SpecialMovementZone);
		
		if (LLPlayerController)
		{
			LLPlayerController->RemoveKeyToPressUI();
		}
	}
	
}

void ULL_JumpVault_Ability::PlayerEndedMovement(APlayerCharacter* PlayerActor, bool bLightUpOrb, bool bStartOrbDecay)
{
	AbilityComponent->StopAbilityByName(Player, "JumpOver", SpecialMovementZone);

	bCanReceiveInput = true;
	if (LLPlayerController)
	{
		LLPlayerController->RemoveKeyToPressUI();
	}
		
	
}
