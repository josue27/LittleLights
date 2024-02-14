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
	bCanReceiveInput = true;
	bCompleted = false;
	Player = Cast<APlayerCharacter>(AbilityComponent->GetOwner());
	if (Player)
	{
		Player->bCanMove = false;
		Player->Crouch();// Crouch();
		Player->bIsCrossingUnder = true;
		Player->DisableInteraction(true);
		//TODO: make the calls of this delegates on the plaeyr
	
		Player->OnKeyPressed.AddDynamic(this,&ULL_CrouchCross_Ability::KeyPressed);
	}
	SpecialMovementZone->BlockerCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpecialMovementZone->TriggerCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	//Set Position from spline
	USplineComponent* Spline = SpecialMovementZone->SplinePath;
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
		for(int32 i = 0 ; i < KeyToPress.Num()-1;i++)
		{
			RandKeysToPress.Add(KeyToPress[FMath::RandRange(0,KeyToPress.Num()-1)]);
		}
		//LLPlayerController->ShowKeyToPressUI("Press " + RandKeysToPress[InKeyPressed].ToString(), Player);
		LLPlayerController->ShowArrowToPressUI(RandKeysToPress[InKeyPressed], Player);
	}
	RemainingActionTime =  TimeToPressKey + Player->GetWorld()->GetTimeSeconds();
}


void ULL_CrouchCross_Ability::KeyPressed(FKey KeyPressed)
{
	if (KeyPressed.GetFName() == FKey("E") || KeyPressed.GetFName() == FKey("e"))return;

	if (RandKeysToPress.IsValidIndex(InKeyPressed) && KeyPressed == RandKeysToPress[InKeyPressed])
	{

	    Player = Player == nullptr? Cast<APlayerCharacter>(AbilityComponent->GetOwner()): Player;
		if (Player)
		{
			Player->OnAutomaticMovementEnded.AddDynamic(this,&ULL_CrouchCross_Ability::PlayerEndedMovement);
			Player->MovePlayerTo(PathPositions[InKeyPressed], 100.f, true,false,false);
			bCanReceiveInput = false;
		}
		InKeyPressed += 1;
		
	}
	else
	{
		AbilityComponent->StopAbilityByName(Player, "Crouch", SpecialMovementZone);
		LLPlayerController->RemoveArrowToPressUI();
	}
}
void ULL_CrouchCross_Ability::PlayerEndedMovement(APlayerCharacter* PlayerCaller,bool bLightUpOrb, bool bStartOrbDecay)
{
	if (Player)
		Player->OnAutomaticMovementEnded.RemoveDynamic(this,&ULL_CrouchCross_Ability::PlayerEndedMovement);
	
	if (InKeyPressed >= RandKeysToPress.Num())
	{
		bCompleted = true;
		AbilityComponent->StopAbilityByName(Player, "Crouch", SpecialMovementZone);

	
		if (LLPlayerController)
		{
			LLPlayerController->RemoveArrowToPressUI();
		}
		
		return;

	}

	///
	if (LLPlayerController)
	{
		//LLPlayerController->ShowKeyWithTimeToPressUI("Press "+ RandKeysToPress[InKeyPressed].ToString(), Player,RemainingDeltaActionTime);
		LLPlayerController->ShowArrowWithTimeToPressUI(RandKeysToPress[InKeyPressed], Player,RemainingDeltaActionTime);
	}
	RemainingActionTime = TimeToPressKey + Player->GetWorld()->GetTimeSeconds();
	bCanReceiveInput = true;

}


void ULL_CrouchCross_Ability::Update_Implementation(float DeltaTime)
{
	Super::Update_Implementation(DeltaTime);
	if(bCompleted || !bCanReceiveInput)return;//in this case because we are moving we dont want to count the time so we wait
	if(Player && IsRunning())
	{
		//we set a delta for the UI so it can set the slider 0 to 1
		RemainingDeltaActionTime = FMath::Clamp( ((RemainingActionTime - Player->GetWorld()->GetTimeSeconds()) / TimeToPressKey),0.f,1.f);
		RemainingDeltaActionTime = FMath::FInterpTo(RemainingDeltaActionTime, 0.f, DeltaTime, 0.5f);

		//FString TRemainingStrinag = FString::Printf(TEXT("Remaining: %f"),RemainingDeltaJumpTime);
		//LogOnScreen(Player->GetWorld(),TRemainingStrinag,FColor::Red,0.1f);
		if (IsValid(LLPlayerController))
		{
			//LLPlayerController->ShowKeyWithTimeToPressUI("Press "+RandKeysToPress[InKeyPressed].ToString(), Player,RemainingDeltaActionTime);
			LLPlayerController->ShowArrowWithTimeToPressUI(RandKeysToPress[InKeyPressed], Player,RemainingDeltaActionTime);

		}
		if(RemainingDeltaActionTime <= 0.0f)
		{
			AbilityComponent->StopAbilityByName(Player, "Crouch", SpecialMovementZone);
			LLPlayerController->RemoveArrowToPressUI();
		}
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
