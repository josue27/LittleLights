// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer_DebugHUD.h"
#include "LittleLights/Personaje.h"
#include "PlayerCharacter.h"

void AMainPlayer_DebugHUD::DrawHUD()
{
	Super::DrawHUD();
	UE_LOG(LogTemp, Warning, TEXT("no console elements"));

	APlayerCharacter* player = Cast<APlayerCharacter>(GetOwningPawn());

	if (player != nullptr)
	{
		AddFloat(L"Velocity", player->GetVelocity().Size());
		AddFloat(L"VelocidadMovimiento", player->VelocidadMovimiento);
		AddInt(L"Bengalas", player->BengalasDisponibles);
		AddInt(L"Fragments in amount(in hand)", player->FragmentsAmount);
		AddBool(L"Can Move", player->bCanMove);
		AddBool(L"Sprinting", player->bSprint);
		AddFloat(L"CurrentStamine", player->CurrentStamine);
		AddBool(L"Jumping", player->bJumping);
		AddBool(L"InJumpOverZone", player->bInJumpOverZone);
		AddBool(L"IsJumpingOver", player->bJumpingOver);
	}
	/*else
	{
		UE_LOG(LogTemp, Warning, TEXT("no console elements"));
	}*/

}