// Fill out your copyright notice in the Description page of Project Settings.


#include "LL_Ability.h"


void ULL_Ability::Initialize(ULL_AbilityComponent* AbilityComp)
{
	if(AbilityComp)
	{
		AbilityComponent = AbilityComp;
	}
}

UWorld* ULL_Ability::GetWorld() const
{
	UWorld* World = Cast<UWorld>(GetOuter());
	if(World)
	{
		return  World;
	}
	return nullptr;
}

void ULL_Ability::Update_Implementation(float DeltaTime)
{
}

void ULL_Ability::StartAbility_Implementation(AActor* Instigator,AActor* SecondActor)
{
	UE_LOG(LogTemp,Warning,TEXT("Ability : %s started"),*AbilityName.ToString());
	bIsRunning =true;
	
}

void ULL_Ability::StopAbility_Implementation(AActor* Instigator,AActor* SecondActor )
{
	UE_LOG(LogTemp,Warning,TEXT("Ability : %s stoped"),*AbilityName.ToString());
	bIsRunning = false;

}

bool ULL_Ability::CanStart_Implementation(AActor* Instigator)
{
	if(IsRunning())
	{
		return  false;
	}
	return  true;
}



bool ULL_Ability::IsRunning()
{
	return bIsRunning;
}

ULL_AbilityComponent* ULL_Ability::GetOwningComponent()
{
	return  AbilityComponent;
}


