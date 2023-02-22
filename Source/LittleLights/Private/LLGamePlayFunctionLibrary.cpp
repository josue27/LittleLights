// Fill out your copyright notice in the Description page of Project Settings.


#include "LLGamePlayFunctionLibrary.h"

#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

bool ULLGamePlayFunctionLibrary::IsPlayerAlive(AActor* PlayerActor)
{
	APlayerCharacter* PC = Cast<APlayerCharacter>(PlayerActor);
	if(PC && PC->IsPlayerAlive())
	{
		return  true;
	}
	return false;
}

float ULLGamePlayFunctionLibrary::TimeToLocation(float Velocity, FVector Start, FVector End)
{
	float Dist = FVector::Dist(Start,End);
	return  Dist / Velocity;
}

//TODO: Change name of this function or make it take 2 FVectors not 1 and 1 spline comp,
///Returns true if player is closer to end point of the given spline
bool ULLGamePlayFunctionLibrary::IsCloserToEnd(FVector PlayerPos, USplineComponent* Spline)
{
	float DistanceToStart = FVector::Dist2D(PlayerPos, Spline->GetLocationAtSplinePoint(0,ESplineCoordinateSpace::World));
	float DistanceToEnd = FVector::Dist2D(PlayerPos,Spline->GetLocationAtSplinePoint(Spline->GetNumberOfSplinePoints(),ESplineCoordinateSpace::World));

	if(DistanceToStart < DistanceToEnd)
	{
		return  false;
	}
	return true;
}
//If the actor is a totem piece we check if it is taken or visible, by default false 
bool ULLGamePlayFunctionLibrary::IsTotemPieceGrabable(AActor* Totempiece)
{
	ATottem_Piece* TP = Cast<ATottem_Piece>(Totempiece);
	if(TP )
	{
		if(TP->bTaken)
		{
			UE_LOG(LogTemp,Warning,TEXT("Hitted with Totem Object already TAKEN"));
			
			return false;

		}
		if(!TP->bIsVisible)
		{
			UE_LOG(LogTemp,Warning,TEXT("Hitted with Totem not visible"));
			
			return false;
		}
			
		
	}
	//if is true that means that is another type of actor BUT we still want to tell this is grabable
	return true;
}

void ULLGamePlayFunctionLibrary::ScreenLog(FString text, FColor Color)
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1,15.0f,Color,text);
	}
}


