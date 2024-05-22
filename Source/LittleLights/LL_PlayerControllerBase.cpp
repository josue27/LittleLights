// Fill out your copyright notice in the Description page of Project Settings.


#include "LL_PlayerControllerBase.h"


#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Level_Manager_Base.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "LLGame_HUD.h"
#include "Widget/LL_PlayerOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "DialogueSystem/LL_DialogueComponent.h"
#include "Widget/LL_ArrowInputWidget.h"
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
	//SetInputMode(FInputModeUIOnly());//not working with EnhanceInput
	bShowMouseCursor = true;

}

void ALL_PlayerControllerBase::TogglePauseMenu(const FInputActionValue& Value)
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

	// InputComponent->BindAction("PauseMenu",IE_Pressed,this,&ALL_PlayerControllerBase::TogglePauseMenu);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);
	
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent);
	Input->BindAction(Pause_IA,ETriggerEvent::Triggered,this,&ALL_PlayerControllerBase::TogglePauseMenu);
	//Input->BindAction(Pause_IA,ETriggerEvent::Completed,this,&ALL_PlayerControllerBase::TogglePauseMenu);
	// Input->BindAction(AdvanceDialogue_IA,ETriggerEvent::Triggered,this,&ALL_PlayerControllerBase::ActionButtonCall);
	
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

void ALL_PlayerControllerBase::ShowKeyWithTimeToPressUI(FString keymsg, AActor* ActorToAttach, float TimeRemainng)
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
		InteractionWidgetInstance->TimeRemainingUI = TimeRemainng;
	}
}

#pragma region Arrow

void ALL_PlayerControllerBase::ShowArrowToPressUI(FKey KeyToPress, AActor* ActorToAttach)
{
	if (ArrowWidgetInstance == nullptr && ensure(DefaultWidgetClass))
	{
		ArrowWidgetInstance = CreateWidget<ULL_ArrowInputWidget>(GetWorld(), ArrowPressWidgetClass);
		ArrowWidgetInstance->AttachedActor = ActorToAttach;
	}
	if (ArrowWidgetInstance)
	{
		ArrowWidgetInstance->AttachedActor = ActorToAttach;//duplicated??
		if (!ArrowWidgetInstance->IsInViewport())
		{
			ArrowWidgetInstance->AddToViewport();
		}
		ArrowWidgetInstance->SetArrow(KeyToPress);
	}
}
void ALL_PlayerControllerBase::RemoveArrowToPressUI()
{
	if(ArrowWidgetInstance)
	{
		ArrowWidgetInstance->RemoveFromParent();
	}
}
//TODO: merge these to one
void ALL_PlayerControllerBase::ShowArrowWithTimeToPressUI(FKey KeyPressed, AActor* ActorToAttach, float TimeRemainng)
{
	if (ArrowWidgetInstance == nullptr && ensure(DefaultWidgetClass))
	{
		ArrowWidgetInstance = CreateWidget<ULL_ArrowInputWidget>(GetWorld(), ArrowPressWidgetClass);
		ArrowWidgetInstance->AttachedActor = ActorToAttach;
	}
	if (ArrowWidgetInstance)
	{
		ArrowWidgetInstance->AttachedActor = ActorToAttach;//duplicated??
		if (!ArrowWidgetInstance->IsInViewport())
		{
			ArrowWidgetInstance->AddToViewport();
		}
		ArrowWidgetInstance->SetArrow(KeyPressed);
		ArrowWidgetInstance->TimeRemainingUI = TimeRemainng;
	}
}
#pragma endregion 

void ALL_PlayerControllerBase::ShowTotemPiecesHUD(bool bShow)
{
	if (LL_GameHUD)
	{
		LL_GameHUD->PlayerOverlay->ShowTotemPieceHUD(bShow);
	}
}

void ALL_PlayerControllerBase::TotemPiecesDeliveredHUD()
{
	if (LL_GameHUD)
	{
		LL_GameHUD->PlayerOverlay->TotemPieceDeliveredHUD();
	}
}

void ALL_PlayerControllerBase::ShowDialogue(FLL_DialogueLineStruct DialogueStruct, bool bShow)
{
	if(LL_GameHUD )
	{
	
		LL_GameHUD->PlayerOverlay->ShowDialogue(bShow, DialogueStruct);
		if(DialogueStruct.DialogueLines.Num()>0)
			LL_GameHUD->PlayerOverlay->OnDialogueLinesOver.AddDynamic(this,&ALL_PlayerControllerBase::DialogueEnded);
		
	}
}

void ALL_PlayerControllerBase::ShowNextDialogue()
{
	if(LL_GameHUD)
	{
	
		LL_GameHUD->PlayerOverlay->NextDialogue();
	
	}
}

void ALL_PlayerControllerBase::DialogueEnded_Implementation()
{
	ShowDialogue(FLL_DialogueLineStruct(),false);
	OnDialogueLinesOver.Broadcast();
	if(LL_GameHUD)
		LL_GameHUD->PlayerOverlay->OnDialogueLinesOver.RemoveDynamic(this,&ALL_PlayerControllerBase::DialogueEnded);

}

void ALL_PlayerControllerBase::ShowBeastPresenceImg_Implementation(bool bShow)
{
	if(LL_GameHUD)
	{
		LL_GameHUD->PlayerOverlay->ShowBeastPresenceHUD(bShow);
	}
}
