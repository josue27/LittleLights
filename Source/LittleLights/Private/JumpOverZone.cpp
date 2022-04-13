// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpOverZone.h"

#include "PlayerCharacter.h"

// Sets default values
AJumpOverZone::AJumpOverZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AJumpOverZone::Interact_Implementation(APawn* InstigatorPawn)
{
	//ILL_GameplayInterface::Interact_Implementation(InstigatorPawn);
	APlayerCharacter* PC = Cast<APlayerCharacter>(InstigatorPawn);
	if(PC)
	{
		ULL_AbilityComponent* AC = Cast<ULL_AbilityComponent>(InstigatorPawn->GetComponentByClass(ULL_AbilityComponent::StaticClass()));
		if(AC)
		{
			switch (MovementZone_Type)
			{
				case EspecialMovementZoneType::Crouch:
					AC->StartAbilityByName(InstigatorPawn,"Crouch",this);
					UE_LOG(LogTemp,Warning,TEXT("%s Sent Crouch ability"),*GetNameSafe(this));
				break;
				case EspecialMovementZoneType::CrossBalancing:
					AC->StartAbilityByName(InstigatorPawn,"CrossBalancing",this);
				UE_LOG(LogTemp,Warning,TEXT("%s Sent Crossbalancing ability"),*GetNameSafe(this));
				break;
				case EspecialMovementZoneType::JumpOver:
					AC->StartAbilityByName(InstigatorPawn,"JumpOver",this);
				UE_LOG(LogTemp,Warning,TEXT("%s Sent JumpOver ability"),*GetNameSafe(this));
				break;
				default:
					UE_LOG(LogTemp,Warning,TEXT("%s Warning MovementZone not registered/invalid"),*GetNameSafe(this));
					break;
			}
		}
			
	}
}

// Called when the game starts or when spawned
void AJumpOverZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJumpOverZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

