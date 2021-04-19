// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickable_Object.h"
#include "Components/SphereComponent.h"
// Sets default values
APickable_Object::APickable_Object()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	EsferaCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Esfera Collision"));
	RootComponent = EsferaCollision;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	Mesh->SetupAttachment(EsferaCollision);
}

// Called when the game starts or when spawned
void APickable_Object::BeginPlay()
{
	Super::BeginPlay();
	EsferaCollision->OnComponentBeginOverlap.AddDynamic(this, &APickable_Object::Recogido);
}

// Called every frame
void APickable_Object::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickable_Object::Recogido(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
	UE_LOG(LogTemp,Warning,TEXT("Objeto %s beginoverlap recogiendo"),*GetName());
}

