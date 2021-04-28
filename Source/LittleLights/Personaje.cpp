// Fill out your copyright notice in the Description page of Project Settings.


#include "Personaje.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Torch.h"
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
	if(TorchClass == nullptr || !bStartWithLight)
		return;
	Torch = GetWorld()->SpawnActor<ATorch>(TorchClass);
	Torch->SetOwner(this);
	Torch->AttachToActor(this,FAttachmentTransformRules::KeepRelativeTransform);

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
	PlayerInputComponent->BindAction(TEXT("InputFlare"),IE_Pressed,this,&APersonaje::ShootFlare);
		// PlayerInputComponent->BindAxis(TEXT("RotarHorizontal"),this,&APersonaje::RotacionHorizontal);

}

int32 APersonaje::GetBengalas() 
{
	return BengalasDisponibles;
}

void APersonaje::LightUpTorch() 
{
	UE_LOG(LogTemp,Warning,TEXT("Torch turned on %s"),*GetName());
}

void APersonaje::MovimientoForward(float AxisValue) 
{
	if(VelocidadMovimiento <= 0)
		return;
	//TODO:Poner opcion para invertir el control para ver si eso soluciona que no tengamos que voltear el startplayer position
	FRotator Rotation = Controller->GetControlRotation();
	FRotator Yaw(0,Rotation.Yaw,0);
	FVector Direction = FRotationMatrix(Yaw).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction,AxisValue * VelocidadMovimiento * GetWorld()->DeltaTimeSeconds);
	//UE_LOG(LogTemp,Warning,TEXT("MOVIENDO"));
}

void APersonaje::MovimientoRight(float AxisValue) 
{
	if(VelocidadMovimiento <= 0)
		return;
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

void APersonaje::ShootFlare() 
{
	if(!bFlaresEnabled || GetBengalas() <= 0)
	{
		UE_LOG(LogTemp,Warning,TEXT("Not enough flares or not enabled"));
		return;
	}
	if(Flare == nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("Not flare set to be spawned"));

		return;
	}
	FVector PosSpawn = PosicionSpawnBengala->GetComponentLocation();
	FRotator RotSpawn = PosicionSpawnBengala->GetComponentRotation();
	AActor* SpawnedFlare = GetWorld()->SpawnActor<AActor>(Flare,PosSpawn,RotSpawn);

	BengalasDisponibles -=1;
	UE_LOG(LogTemp,Warning,TEXT("Shooted Flare"));
}

