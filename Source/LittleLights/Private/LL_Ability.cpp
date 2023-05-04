// Fill out your copyright notice in the Description page of Project Settings.


#include "LL_Ability.h"

#include "LL_PlayerState.h"
#include "Kismet/GameplayStatics.h"


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
	//appendtags let us add a whole container, instead of add one by one
	AbilityComponent->ActiveGameplayTags.AppendTags(GrantsTags);
}

void ULL_Ability::StopAbility_Implementation(AActor* Instigator,AActor* SecondActor )
{
	UE_LOG(LogTemp,Warning,TEXT("Ability : %s stoped"),*AbilityName.ToString());
	bIsRunning = false;
	//removetags let us remove a whole container, instead of remove one by one
	AbilityComponent->ActiveGameplayTags.RemoveTags(GrantsTags);

	//Call stop interaction, Mainly for the Beast slowmo
	ALL_PlayerState* LLPlayerState = Cast<ALL_PlayerState>(UGameplayStatics::GetPlayerState(GetWorld(),0));
	if(LLPlayerState)
	{
		LLPlayerState->OnInteractionEnded.Broadcast(AbilityComponent->GetOwner(),false);
	}
}

bool ULL_Ability::CanStart_Implementation(AActor* Instigator)
{
	
	if(IsRunning())
	{
		return  false;
	}

	if (AbilityComponent)
	{
		if (AbilityComponent->ActiveGameplayTags.HasAny(BlockedTags))
		{
			return false;
		}
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


