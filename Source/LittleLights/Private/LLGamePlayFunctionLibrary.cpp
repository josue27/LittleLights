// Fill out your copyright notice in the Description page of Project Settings.


#include "LLGamePlayFunctionLibrary.h"

#include "PlayerCharacter.h"

bool ULLGamePlayFunctionLibrary::IsPlayerAlive(AActor* PlayerActor)
{
	APlayerCharacter* PC = Cast<APlayerCharacter>(PlayerActor);
	if(PC && PC->IsPlayerAlive())
	{
		return  true;
	}
	return false;
}
