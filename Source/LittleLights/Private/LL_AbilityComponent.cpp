// Fill out your copyright notice in the Description page of Project Settings.


#include "LL_AbilityComponent.h"
#include "LL_Ability.h"
#include "LittleLights/LittleLights.h"

// Sets default values for this component's properties
ULL_AbilityComponent::ULL_AbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	
}


// Called when the game starts
void ULL_AbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	for(TSubclassOf<ULL_Ability> Ability:DefaultAbilities)
	{
		AddAbility(GetOwner(),Ability);
	}
	
}

void ULL_AbilityComponent::AddAbility(AActor* Instigator, TSubclassOf<ULL_Ability> AbilityClass)
{
	if(!ensure(AbilityClass))
	{
		return;
	}
	//Create object to ADD
	ULL_Ability* NewAbility = NewObject<ULL_Ability>(GetOwner(),AbilityClass);
	if(NewAbility)
	{
		NewAbility->Initialize(this);
		Abilities.AddUnique(NewAbility);
	}
	
}


void ULL_AbilityComponent::StartAbilityByName(AActor* Instigator, FName AbilityName,AActor* ActorInfo )
{
	for(ULL_Ability* Ability : Abilities)
	{
		if(Ability->AbilityName == AbilityName)
		{
			if(!Ability->CanStart(Instigator))
			{
				continue;//remember to go back
			}
			Ability->StartAbility(Instigator,ActorInfo);
			return;
		}
	}
}

void ULL_AbilityComponent::StopAbilityByName(AActor* Instigator, FName AbilityName,AActor* ActorInfo )
{
	for(ULL_Ability* Ability : Abilities)
	{
		if(Ability->AbilityName == AbilityName)
		{
			if(Ability->IsRunning())
			{
				Ability->StopAbility(Instigator,ActorInfo);
				//FString compstring =  FString::Printf((TEXT("Stop ability:%s"), AbilityName));
				LogOnScreen(this, FString(TEXT("Ability stopped")));

				return;
			}
		}
	}
	
}


// Called every frame
void ULL_AbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	for(ULL_Ability* Ability : Abilities)
	{
		if(Ability->bTick)
		{
			Ability->Update(DeltaTime);
		}
	}
}

