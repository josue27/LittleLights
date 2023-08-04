// Fill out your copyright notice in the Description page of Project Settings.


#include "Trolls/LL_TrollCharacter.h"

// Sets default values
ALL_TrollCharacter::ALL_TrollCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DialogueComponent = CreateDefaultSubobject<ULL_DialogueComponent>(TEXT("DialogueComponent"));

}

// Called when the game starts or when spawned
void ALL_TrollCharacter::BeginPlay()
{
	Super::BeginPlay();


	
	
}

// Called every frame
void ALL_TrollCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALL_TrollCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ALL_TrollCharacter::Interact_Implementation(APawn* InstigatorPawn)
{
	ILL_GameplayInterface::Interact_Implementation(InstigatorPawn);
	if(DialogueComponent)
	{
		DialogueComponent->StartDialogue();
	}
}

void ALL_TrollCharacter::InteractionEnded_Implementation(AActor* InstigatorPawn, bool bSlowTime)
{
	ILL_GameplayInterface::InteractionEnded_Implementation(InstigatorPawn, bSlowTime);
}

FText ALL_TrollCharacter::GetInteractText_Implementation(APawn* InstigatorPawn)
{

	//TODO: Connect to DT
	return  FText::FromString("Press E to talk");
}

