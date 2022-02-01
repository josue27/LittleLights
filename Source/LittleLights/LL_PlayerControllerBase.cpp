// Fill out your copyright notice in the Description page of Project Settings.


#include "LL_PlayerControllerBase.h"


#include "Level_Manager_Base.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ALL_PlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();


}

void ALL_PlayerControllerBase::DisableAllInput()
{
	DisableInput(this);

	bShowMouseCursor = false;
}

void ALL_PlayerControllerBase::ChangeToGameInput()
{
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;

}

void ALL_PlayerControllerBase::ChangeToUIInput()
{
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;

}

void ALL_PlayerControllerBase::TogglePauseMenu()
{
	if(PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;
		ChangeToGameInput();
		UGameplayStatics::SetGamePaused(GetWorld(),false);

		return;
	}
	PauseMenuInstance = CreateWidget(this,PauseMenuClass);
	if(PauseMenuInstance)
	{
		
		PauseMenuInstance->AddToViewport(100);
		ChangeToUIInput();
		UGameplayStatics::SetGamePaused(GetWorld(),true);
	}
}

void ALL_PlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseMenu",IE_Pressed,this,&ALL_PlayerControllerBase::TogglePauseMenu);
}
