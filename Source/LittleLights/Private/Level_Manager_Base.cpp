// Fill out your copyright notice in the Description page of Project Settings.


#include "Level_Manager_Base.h"

// Sets default values
ALevel_Manager_Base::ALevel_Manager_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALevel_Manager_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevel_Manager_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

