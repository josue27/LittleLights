// Fill out your copyright notice in the Description page of Project Settings.


#include "Personaje.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"
// Sets default values
APersonaje::APersonaje()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0,540,0);
	//Use controller rotation yaw = false en los class defaults
	PosicionSpawnBengala = CreateDefaultSubobject<UArrowComponent>(TEXT("PosicionSpawnBengala"));
	PosicionSpawnBengala->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APersonaje::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APersonaje::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UpdateRotacion();
}

// Called to bind functionality to input
void APersonaje::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoverAdelante"),this,&APersonaje::MovimientoForward);
	PlayerInputComponent->BindAxis(TEXT("MoverDerecha"),this,&APersonaje::MovimientoRight);
	// PlayerInputComponent->BindAxis(TEXT("RotarHorizontal"),this,&APersonaje::RotacionHorizontal);

}

int32 APersonaje::GetBengalas() 
{
	return BengalasDisponibles;
}

void APersonaje::MovimientoForward(float AxisValue) 
{
	//TODO:Poner opcion para invertir el control para ver si eso soluciona que no tengamos que voltear el startplayer position
	FRotator Rotation = Controller->GetControlRotation();
	FRotator Yaw(0,Rotation.Yaw,0);
	FVector Direction = FRotationMatrix(Yaw).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction,AxisValue * VelocidadMovimiento * GetWorld()->DeltaTimeSeconds);
	//UE_LOG(LogTemp,Warning,TEXT("MOVIENDO"));
}

void APersonaje::MovimientoRight(float AxisValue) 
{
	
	FRotator Rotation = Controller->GetControlRotation();
	FRotator Yaw(0,Rotation.Yaw,0);
	FVector Direction = FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction,AxisValue * VelocidadMovimiento * GetWorld()->DeltaTimeSeconds);	

}

void APersonaje::RotacionHorizontal(float AxisValue) 
{	
	float CantidadRotacion = AxisValue * VelocidadRotacion * GetWorld()->GetDeltaSeconds();
	FRotator RotacionZ = FRotator(0,CantidadRotacion,0);
	RotacionFinal = FQuat(RotacionZ);
	 AddControllerYawInput(AxisValue * VelocidadRotacion * GetWorld()->GetDeltaSeconds());
	// AddActorLocalRotation(RotacionFinal,true);
}

void APersonaje::UpdateRotacion() 
{
	AddActorLocalRotation(RotacionFinal,true);
	
}

