// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer_DebugHUD.h"
#include "LittleLights/Personaje.h"

void AMainPlayer_DebugHUD::DrawHUD()
{
	Super::DrawHUD();
	UE_LOG(LogTemp, Warning, TEXT("no console elements"));

	APersonaje* player = Cast<APersonaje>(GetOwningPawn());

	if (player != nullptr)
	{
		AddFloat(L"Velocity", player->GetVelocity().Size());
		AddFloat(L"VelocidadMovimiento", player->VelocidadMovimiento);
		AddInt(L"Bengalas", player->BengalasDisponibles);
		AddInt(L"Fragments in hand", player->FragmentsAmount);
		AddBool(L"Can Move", player->bCanMove);
		AddBool(L"Sprinting", player->bSprint);
		AddFloat(L"CurrentStamine", player->CurrentStamine);
		
	}
	/*else
	{
		UE_LOG(LogTemp, Warning, TEXT("no console elements"));
	}*/

}