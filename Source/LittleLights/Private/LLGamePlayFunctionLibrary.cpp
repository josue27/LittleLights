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


