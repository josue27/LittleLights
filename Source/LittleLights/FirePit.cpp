// Fill out your copyright notice in the Description page of Project Settings.


#include "FirePit.h"
#include "Components/PointLightComponent.h"
#include "Components/SphereComponent.h"
#include "Personaje.h"
// Sets default values
AFirePit::AFirePit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	Mesh->SetupAttachment(Root);
	PitLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("MainLight"));
	PitLight->SetupAttachment(Root);
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AFirePit::BeginPlay()
{
	Super::BeginPlay();
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this,&AFirePit::PlayerInPit);
	 SphereCollision->OnComponentEndOverlap.AddDynamic(this,&AFirePit::PlayerLeftPit);
	
}

// Called every frame
void AFirePit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFirePit::PlayerInPit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
	APersonaje* PersonajeJugador = Cast<APersonaje>(OtherActor);
	if(PersonajeJugador)
	{
		// PersonajeJugador->LightUpTorch();
		
			PersonajeJugador->bInFirePit = true;
			PersonajeJugador->FirePitTemp = this;
			PersonajeJugador->bShowHints = true;
			PersonajeJugador->ShowHint(true, FString(TEXT("Press E or Square to lightup torch")) );

		UE_LOG(LogTemp,Warning,TEXT("Player on firepit"));
	}
}
void AFirePit::PlayerLeftPit(UPrimitiveComponent* OverlappedComp,class AActor * OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//OtherActor->IsA<APersonaje>() == true;//this could also work before casting
	APersonaje* PersonajeJugador = Cast<APersonaje>(OtherActor);
	if(PersonajeJugador)
	{
		// PersonajeJugador->LightUpTorch();
	
			PersonajeJugador->bInFirePit = false;
			PersonajeJugador->FirePitTemp = nullptr;
			PersonajeJugador->bShowHints = false;



		UE_LOG(LogTemp,Warning,TEXT("Player left firepit"));
	}
}
