// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/LL_Orb_Ability.h"

#include "PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"


void ULL_Orb_Ability::SpawnOrb(APlayerCharacter* PC)
{
}

void ULL_Orb_Ability::LightUpTorch(AActor* Instigator)
{

	APlayerCharacter* PC = Cast<APlayerCharacter>(Instigator);
	if(PC)
	{
		PC->bCanMove = false;//make function
		PC->bLightingTorch = true;
		PC->bUpdateFov = false;
		// TODO: How to get the position of the pond for the rotation

		//FRotator RotateTo = UKismetMathLibrary::FindLookAtRotation(PC->GetActorLocation(), ->GetActorLocation());
		//RotateTo.Pitch = PC->GetActorRotation().Pitch;
		//RotateTo.Roll = PC->GetActorRotation().Roll;
		//PC->SetActorRotation(RotateTo);
	
		//GetWorld()->GetTimerManager().SetTimer(DelayLightingTorch, this, &APlayerCharacter::TorchLightingCompleted, 4.0f, false);
		//TempRefillAmount = AmountRefill;
	}else
	{
		
		return;
	}

	///THE END OF THIS ABILITY IS IN THE PLAYER FOR THE MOMENT Called TorchLightingCompleted()
}

void ULL_Orb_Ability::EndLighting()
{

	//TODO:We shoudl recieve the call from the AnimationBP(get the component and the stopbyname ) then here we get the owner of the comp or the ABP and set the proper variables
	
}

void ULL_Orb_Ability::LightDecay()
{
	if (AbilityComponent == nullptr)return;

	

}

