// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/LL_AIBeastController.h"

void ALL_AIBeastController::BeginPlay()
{
	Super::BeginPlay();

	if(ensureMsgf(BehaviorTree,TEXT("There is no BehaviorTree assignated in AI Controller, assign one")))
	{
		RunBehaviorTree(BehaviorTree);
	}
}
