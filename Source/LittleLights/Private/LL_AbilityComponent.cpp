// Fill out your copyright notice in the Description page of Project Settings.


#include "LL_AbilityComponent.h"
#include "LL_Ability.h"
#include "PlayerCharacter.h"
#include "LL_PlayerState.h"
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
	ULL_Ability* NewAbility = NewObject<ULL_Ability>(Instigator,AbilityClass);
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
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *AbilityName.ToString());
				GEngine->AddOnScreenDebugMessage(-1,0.2f, FColor::Red, FailedMsg);
				continue;//remember to go back
			}
			Ability->StartAbility(Instigator,ActorInfo);
			return;
		}
	}
}

void ULL_AbilityComponent::StopAbilityByName(AActor* Instigator, FName AbilityName,AActor* ActorInfo )
{
	for (ULL_Ability* Ability : Abilities)
	{
		if (Ability->AbilityName == AbilityName)
		{
			if (Ability->IsRunning())
			{
				Ability->StopAbility(Instigator, ActorInfo);
				//FString compstring =  FString::Printf((TEXT("Stop ability:%s"), AbilityName));
				LogOnScreen(this, FString(TEXT("Ability stopped")));

				APlayerCharacter* PC = Cast<APlayerCharacter>(Instigator);
				if (PC)
				{
					ALL_PlayerState* PS = Cast<ALL_PlayerState>(PC->GetPlayerState());
					if (PS)
					{
						PS->OnInteractionEnded.Broadcast(GetOwner(), false);
					}
				}

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
		if(Ability->AbilityName == "Sprint" && Ability->bTick)
		{
			Ability->Update(DeltaTime);
		}
		else if(Ability->bTick && Ability->IsRunning())
		{
			Ability->Update(DeltaTime);
		}
	}
	if (GEngine)
	{
		FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
	}
}


ULL_Ability* ULL_AbilityComponent::GetAbilityByName(FName AbilityName)
{
	
	for(int32 i =0; i< Abilities.Num();i++)
	{
		if(Abilities[i]->AbilityName == AbilityName)
		{
			return Abilities[i];
		}
	}
	return  nullptr;
}


