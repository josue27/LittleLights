// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/LL_CrossBalancing_Ability.h"

#include "JumpOverZone.h"
#include "LLGamePlayFunctionLibrary.h"
#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SpecialMovementZones/LL_SpecialMovementZone.h"

void ULL_CrossBalancing_Ability::StartAbility_Implementation(AActor* Instigator, AActor* SecondActor)
{
	Super::StartAbility_Implementation(Instigator, SecondActor);

	 PlayerCharacter = Cast<APlayerCharacter>(Instigator);
	 JumpOverZone = Cast<ALL_SpecialMovementZone>(SecondActor);

	if (PlayerCharacter)
	{
		PlayerCharacter->bCanMove = false;
		PlayerCharacter->bBalancing = true;
		PlayerCharacter->bInJumpOverZone = true;
		PlayerCharacter->DisableInteraction(true);
		PlayerCharacter->GetCapsuleComponent()->SetActive(false);
		//TODO: Add player Unbalance functionality
	
	}
	if(JumpOverZone)
	{
		Spline = JumpOverZone->SplinePath;
		JumpOverZone->DisableCollisions(true);
	}


	 bCloserToEnd = ULLGamePlayFunctionLibrary::IsCloserToEnd(PlayerCharacter->GetActorLocation(),Spline);
	//Set starting location and rotation player

	PlayerStartLoc = bCloserToEnd ? Spline->GetLocationAtSplinePoint(Spline->GetNumberOfSplinePoints(),ESplineCoordinateSpace::World) : Spline->GetLocationAtSplinePoint(0,ESplineCoordinateSpace::World);
	PlayerEndLoc = bCloserToEnd ? Spline->GetLocationAtSplinePoint(0,ESplineCoordinateSpace::World) : Spline->GetLocationAtSplinePoint(Spline->GetNumberOfSplinePoints(),ESplineCoordinateSpace::World);
	
	PlayerStartLoc.Z += PlayerCharacter->GetDefaultHalfHeight()+ZOffset;
	PlayerEndLoc.Z += PlayerCharacter->GetDefaultHalfHeight()+ZOffset;
	PlayerCharacter->SetActorLocation(PlayerStartLoc);
	
	PlayerStartLoc.Z = PlayerCharacter->GetActorLocation().Z;
	FRotator PlayerRotation = UKismetMathLibrary::FindLookAtRotation(PlayerCharacter->GetActorLocation(), bCloserToEnd ? PlayerEndLoc : PlayerStartLoc);
	PlayerCharacter->SetActorRotation(PlayerRotation,ETeleportType::ResetPhysics);

	//PlayerCharacter->SetActorLocationAndRotation(PlayerStartLoc,PlayerRotation,false,0,ETeleportType::None);
	
	bStartBalance = true;
}

void ULL_CrossBalancing_Ability::Update_Implementation(float DeltaTime)
{
	Super::Update_Implementation(DeltaTime);
	if(!bStartBalance)return;

	if(PlayerCharacter)
	{
		if(bCloserToEnd)
		{
			DistanceWalkedX += (PlayerCharacter->MovementInputVector.Y * -1.f) * (CrossingVelocityMultiplier * DeltaTime);

		}
		else
		{
			
			DistanceWalkedX += PlayerCharacter->MovementInputVector.Y * (CrossingVelocityMultiplier * DeltaTime);
		}
			

		//PlayerCharacter->bIsBalancingIdel = PlayerCharacter->MovementInputVector.Y < 0.2f;
		DrawDebugSphere(GetWorld(), PlayerStartLoc, 50.0f, 16, FColor::Yellow);
		DrawDebugSphere(GetWorld(), PlayerEndLoc, 50.0f, 16, FColor::Yellow);
		
			
		PlayerCharacter->SetActorLocation(FMath::Lerp(PlayerStartLoc, PlayerEndLoc, DistanceWalkedX));
		
		if (DistanceWalkedX >= 1.0f || DistanceWalkedX < 0.f)
		{
			AbilityComponent->StopAbilityByName(PlayerCharacter, "CrossBalancing", nullptr);
			bStartBalance = false;
		}
		
	}
}

void ULL_CrossBalancing_Ability::StopAbility_Implementation(AActor* Instigator, AActor* SecondActor)
{
	Super::StopAbility_Implementation(Instigator, SecondActor);


	
	
	DistanceWalkedX= 0.0f;
	
	bStartBalance = false;
	BalancingX = 0.f;
	bFalling = false;
	PlayerCharacter->bBalancing = false;
	PlayerCharacter->bInJumpOverZone = false;
	PlayerCharacter->GetCapsuleComponent()->SetActive(true);
	PlayerCharacter->DisableInteraction(false);
	PlayerCharacter = nullptr;
	//JumpOverZone->DisableCollisions(false);
	//JumpOverZone->DisableMeshCollisions(3.0f);
	JumpOverZone = nullptr;
	PlayerStartLoc = FVector::Zero();
	PlayerEndLoc = FVector::Zero();
	
}
