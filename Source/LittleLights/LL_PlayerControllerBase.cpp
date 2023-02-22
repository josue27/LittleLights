// Fill out your copyright notice in the Description page of Project Settings.


#include "LL_PlayerControllerBase.h"


#include "Level_Manager_Base.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "LLGame_HUD.h"
#include "Widget/LL_PlayerOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Widget/LL_WorldUserWidget.h"


void ALL_PlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	LL_GameHUD = Cast<ALLGame_HUD>(GetHUD());

	if (LL_GameHUD)
	{
		LL_GameHUD->AddPlayerOverlay();
	}
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


void ALL_PlayerControllerBase::UpdateHUDBeastDistance(float deltaDistance)
{
	if (LL_GameHUD && LL_GameHUD->PlayerOverlay && LL_GameHUD->PlayerOverlay->BeastAlertImg)
	{

		LL_GameHUD->PlayerOverlay->BeastAlertImg->SetOpacity(deltaDistance);
	}


}

void ALL_PlayerControllerBase::ShowKeyToPressUI(FString keymsg, AActor* ActorToAttach)
{
	if (InteractionWidgetInstance == nullptr && ensure(DefaultWidgetClass))
	{
		InteractionWidgetInstance = CreateWidget<ULL_WorldUserWidget>(GetWorld(), DefaultWidgetClass);
		InteractionWidgetInstance->AttachedActor = ActorToAttach;
	}
	if (InteractionWidgetInstance)
	{
		 InteractionWidgetInstance->AttachedActor = ActorToAttach;//duplicated??
		if (!InteractionWidgetInstance->IsInViewport())
		{
			InteractionWidgetInstance->AddToViewport();
		}
		InteractionWidgetInstance->TextOfInteraction = FText::FromString(keymsg);

	}
}

void ALL_PlayerControllerBase::RemoveKeyToPressUI()
{
	if (InteractionWidgetInstance)
	{
		InteractionWidgetInstance->RemoveFromParent();
	}
}