// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/LL_JumpVault_Ability.h"

#include "LLGamePlayFunctionLibrary.h"
#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LittleLights/LittleLights.h"
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
	RemainingActionTime =  TimeToPressKey + Player->GetWorld()->GetTimeSeconds();

}
void ULL_JumpVault_Ability::Update_Implementation(float DeltaTime)
{
	
	if(Player && IsRunning())
	{
		RemainingDeltaJumpTime = FMath::Clamp( ((RemainingActionTime - Player->GetWorld()->GetTimeSeconds()) / TimeToPressKey),0.f,1.f);
		RemainingDeltaJumpTime = FMath::FInterpTo(RemainingDeltaJumpTime, 0.f, DeltaTime, 0.5f);

		FString TRemainingStrinag = FString::Printf(TEXT("Remaining: %f"),RemainingDeltaJumpTime);
		LogOnScreen(Player->GetWorld(),TRemainingStrinag,FColor::Red,0.1f);
		if (LLPlayerController)
		{
			LLPlayerController->ShowKeyWithTimeToPressUI("Press "+KeysToPress[InKeyPressed].ToString(), Player,RemainingDeltaJumpTime);
		}
		if(RemainingDeltaJumpTime <= 0.0f)
		{
			AbilityComponent->StopAbilityByName(Player, "JumpOver", SpecialMovementZone);
		}
	}
}



void ULL_JumpVault_Ability::StopAbility_Implementation(AActor* Instigator, AActor* SecondActor)
{
	Super::StopAbility_Implementation(Instigator, SecondActor);
	
	Player->ResetWalkSpeed(400.0f);
	Player->bJumpingOver = false;
	Player->bCanMove = true;
	Player->OnAutomaticMovementEnded.RemoveDynamic(this, &ULL_JumpVault_Ability::PlayerEndedMovement);
	Player->OnKeyPressed.RemoveDynamic(this, &ULL_JumpVault_Ability::KeyPressed);
	Player->DisableInteraction(false);


	//Call the stop interaction, Mainly for the Beast slowmotion
	if (LLPlayerController)
	{
		LLPlayerController->RemoveKeyToPressUI();
	}

	Player = nullptr;
	SpecialMovementZone->PlayerEndedTask();

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
			if (LLPlayerController)
			{
				LLPlayerController->RemoveKeyToPressUI();
			}
			
			
		}
		else
		{
			InKeyPressed += 1;
			if (LLPlayerController)
			{
				LLPlayerController->ShowKeyWithTimeToPressUI("Press "+KeysToPress[InKeyPressed].ToString(), Player,RemainingActionTime);

			}
			bCanReceiveInput = true;
			RemainingActionTime = TimeToPressKey+Player->GetWorld()->GetTimeSeconds();
		}

	}
	else
	{
		float AnimDuration = Player->PlayAnimation(JumpFailedAnimation);
		FTimerHandle FailedAnimationTimerHandle;
		FTimerDelegate FailedTimerDelegate;
		FailedTimerDelegate.BindLambda([&]
		{
			
			AbilityComponent->StopAbilityByName(Player, "JumpOver", SpecialMovementZone);
		});
		Player->GetWorldTimerManager().SetTimer(FailedAnimationTimerHandle,FailedTimerDelegate,AnimDuration,false);
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
