// Fill out your copyright notice in the Description page of Project Settings.


#include "Personaje.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "FirePit.h"
#include "Kismet/KismetMathLibrary.h"
#include "Torch.h"
#include "Components/TimelineComponent.h"
#include  "GameFramework/SpringArmComponent.h"
#include "JumpOverZone.h"
#include "Level_Manager_Base.h"
#include "DrawDebugHelpers.h"
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
	TorchPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("TorhcSpawnPos"));
	TorchPosition->SetupAttachment(RootComponent);
}	

// Called when the game starts or when spawned
void APersonaje::BeginPlay()
{
	Super::BeginPlay();


	if(TorchClass == nullptr || !bStartWithLight)
		return;
	FVector TorchPos = TorchPosition->GetComponentLocation();
	FRotator TorchRotation = TorchPosition->GetComponentRotation();
	Torch = GetWorld()->SpawnActor<ATorch>(TorchClass, TorchPos, TorchRotation);
	Torch->SetOwner(this);
	Torch->AttachToActor(this,FAttachmentTransformRules::KeepRelativeTransform);
	Torch->SetActorLocationAndRotation(TorchPos,TorchRotation);

	VelocidadMovimiento = NormalMaxVelocity;
	CurrentStamine = Stamine;

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		
		TimelineProgress.BindUFunction(this, FName("TimelineRoll_Progress"));
		
		CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);
		

	}
	
		
}



// Called every frame
void APersonaje::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UpdateRotacion();
	SprintUpdate();
	CurveTimeline.TickTimeline(DeltaTime);

	LookingAt();

	if (Torch != nullptr)
	{

	}

	
}

// Called to bind functionality to input
void APersonaje::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoverAdelante"),this,&APersonaje::MovimientoForward);
	PlayerInputComponent->BindAxis(TEXT("MoverDerecha"),this,&APersonaje::MovimientoRight);
	PlayerInputComponent->BindAction(TEXT("InputFlare"),IE_Pressed,this,&APersonaje::ShootFlare);
	PlayerInputComponent->BindAction(TEXT("InteractInput"),IE_Pressed,this,&APersonaje::ActionButtonCall);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &APersonaje::SprintAction);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &APersonaje::SprintCancelled);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APersonaje::JumpButtonCall);

		// PlayerInputComponent->BindAxis(TEXT("RotarHorizontal"),this,&APersonaje::RotacionHorizontal);

}

int32 APersonaje::GetBengalas() 
{
	return BengalasDisponibles;
}

/// <summary>
/// Called when user press the action button to light up de torch if any
/// </summary>
void APersonaje::LightUpTorch()
{
	if (bLightingTorch)
	{
		bLightingTorch = false;
		return;
	}
	else if(bInFirePit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Restarting Torch light"));
		bCanMove = false;//make function
		if (FirePitTemp)
		{
			bLightingTorch = true;
			FRotator RotateTo = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FirePitTemp->GetActorLocation());
			RotateTo.Pitch = GetActorRotation().Pitch;
			RotateTo.Roll = GetActorRotation().Roll;
			SetActorRotation(RotateTo);
			GetWorld()->GetTimerManager().SetTimer(DelayLightingTorch, this, &APersonaje::TorchLightingCompleted, 4.0f, false);
			if (Torch != nullptr)
			{
				Torch->bStartDecay = false;
			}
		}
	}


}


/// <summary>
/// Function Called by LightUpTorch after SetTimer just to let the animation play
/// </summary>


void APersonaje::SprintAction()
{
	if (CurrentStamine > 0)
	{
		VelocidadMovimiento = CurrentStamine > 0 ? SprintVelocity : NormalMaxVelocity;
		CurrentStamine = FMath::Abs((GetWorld()->GetTimeSeconds() + CurrentStamine) - GetWorld()->GetTimeSeconds());
		bSprint = true;

	}
	else 
	{

		VelocidadMovimiento = NormalMaxVelocity;
	}
}

void APersonaje::SprintCancelled()
{
	VelocidadMovimiento = NormalMaxVelocity;
	bSprint = false;
	UE_LOG(LogTemp, Warning, TEXT("SprintCancelled"));

}

void APersonaje::SprintUpdate()
{
	if (!bSprint )
	{
		CurrentStamine = FMath::Clamp(CurrentStamine + GetWorld()->GetDeltaSeconds(),0.0f,Stamine);

		return;
	}

	CurrentStamine -=  GetWorld()->GetDeltaSeconds();
	if (CurrentStamine <= 0.0f)
	{
		VelocidadMovimiento = NormalMaxVelocity;
		CurrentStamine = 0.0f;

	}
	UE_LOG(LogTemp, Warning, TEXT("CurrenStamine: %f"), CurrentStamine);
}

void APersonaje::TorchLightingCompleted()
{
	bLightingTorch = false;
	if (Torch != nullptr)
	{
		if (LevelManager != nullptr)
		{
			Torch->StartDecay(LevelManager->TorchLightUpTime);
		}
		Torch->StartDecay(DefaultTorchDecay);//TODO:Hardcoded with need the level manager ref
	}
	bCanMove = true;
}

void APersonaje::TorchLightDecay()
{

	if (Torch == nullptr)
		return;
	
}

void APersonaje::StopCharacter()
{
	bCanMove = false;
}

void APersonaje::ContinueMovement()
{
	bCanMove = true;
}

void APersonaje::ActionButtonCall()
{

	if (bInFirePit)
	{
		LightUpTorch();
	}
	else if(bInJumpOverZone && Temp_JumpOverZone)
	{

		//JumpOver();
	}
}

void APersonaje::JumpOver(class AJumpOverZone* TempZone)
{
	if (bJumpingOver) return;

	Temp_JumpOverZone = TempZone;

	bJumpingOver = true;//for animation BP
	bCanMove = false;
	//Jump();
	//if(Temp_JumpOverZone)
	//	Temp_JumpOverZone->StartJumpOver(EspecialMovementZoneType::JumpOver,this);


}

void APersonaje::ShowHint(bool showhint, const FString& textToShow)
{
	bShowHints = showhint;
	

	TextHint =  textToShow != " "?  FText::FromString(textToShow) : FText::FromString("Press E for action");
	
}

void APersonaje::MovimientoForward(float AxisValue)
{
	if(VelocidadMovimiento <= 0 || !bCanMove)
		return;
	//TODO:Poner opcion para invertir el control para ver si eso soluciona que no tengamos que voltear el startplayer position
	FRotator Rotation = Controller->GetControlRotation();
	FRotator Yaw(0,Rotation.Yaw - JoystickAnlgeDifference,0);
	FVector Direction = FRotationMatrix(Yaw).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction,AxisValue * VelocidadMovimiento * GetWorld()->DeltaTimeSeconds);

	if (SpringArmRef != nullptr)
	{
		SpringArmRef->TargetOffset = GetActorForwardVector() * 120.0f;
	}
	//UE_LOG(LogTemp,Warning,TEXT("MOVIENDO"));
} 



/// <summary>
/// When user press jump, it doesn't jump , first cast a linetrace to see if is in front of a JumpOverZone(EspecialMovementZone) and there 
/// if it is, take different actions, jumpover, crouch, etc.
/// </summary>
void APersonaje::JumpButtonCall()
{

	//Create LaneTrace to detect if in front of JumpOverObstacle || WalkUnderObstacle || BalancingObstalce
	FVector LineStart = GetActorLocation();
	FVector LineEnd = LineStart +( GetActorRotation().Vector() * JumpDistanceDetection);
	FHitResult Hit;
	FCollisionQueryParams TraceParams;
	//This is important or wont trace line
	TraceParams.AddIgnoredActor(this);
	TraceParams.AddIgnoredActor(GetOwner());

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, LineStart, LineEnd, ECC_Visibility, TraceParams);
	
	DrawDebugLine(GetWorld(), LineStart, LineEnd, FColor::Red, false, 3.0f);
	
	if (bHit)
	{
		DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Red, false, 3.0f);

		AJumpOverZone* EspecialMovementZone = Cast<AJumpOverZone>(Hit.Actor);
		if (EspecialMovementZone != nullptr)
		{
			//If in front of JumpOverObstacle
			if (EspecialMovementZone->MovementZone_Type == EspecialMovementZoneType::JumpOver)
			{
				UE_LOG(LogTemp, Warning, TEXT("In JumpOver Zone"));

				JumpOver(EspecialMovementZone);
			}
			else if (EspecialMovementZone->MovementZone_Type == EspecialMovementZoneType::Crouch)
			{
				UE_LOG(LogTemp, Warning, TEXT("In Crouch Zone"));
				CrouchUnder(EspecialMovementZone);
			}
			//else if in front of WalkUnderObstacle
				//WalkUnder()
			//els if in front of BalancinObstacle
				//StartBalancing()
			else
			{

				//RollForward();
			}
			
		}
	}
	else
	{
		//RollForward();

	}
		
	
	//else
		//RollForward
}

void APersonaje::LookingAt()
{


}

void APersonaje::MovimientoRight(float AxisValue)
{
	if(VelocidadMovimiento <= 0 || !bCanMove)
		return;

	//BUG TO FIX: When pressing with 2 keyboard keys the velocity get higher(250) but with
	//analog stick it doesnt happen
	FRotator Rotation = Controller->GetControlRotation();
	FRotator Yaw(0,Rotation.Yaw - JoystickAnlgeDifference,0);
	FVector Direction = FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction,AxisValue * VelocidadMovimiento * GetWorld()->DeltaTimeSeconds);	

	if (SpringArmRef != nullptr)
	{
		SpringArmRef->TargetOffset = GetActorForwardVector() * 120.0f;
	}
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


void APersonaje::RollForward()
{
	if (bJumping)
	{
		return;
	}
	bJumping = true;

	

	CurveTimeline.PlayFromStart();
	GetWorld()->GetTimerManager().SetTimer(DelayForJumpAnimation, this, &APersonaje::JumpCompleted, DelayForCompletedJump, false);
	//VelocidadMovimiento = NormalMaxVelocity;
}
void APersonaje::CrouchUnder(AJumpOverZone* TempZone)
{
	if (bIsCrossingUnder) return;

	Temp_JumpOverZone = TempZone;

	bIsCrossingUnder = true;//for animation BP
	bCanMove = false;
	//Jump();
	//if (Temp_JumpOverZone)
	//	Temp_JumpOverZone->StartJumpOver(EspecialMovementZoneType::Crouch, this);
}
void APersonaje::TimelineRoll_Progress(float Value)
{

	FVector EndLoc = (GetActorForwardVector() * JumpDistance) + GetActorLocation();
	FVector NewLoc= FMath::Lerp(GetActorLocation(), EndLoc, Value);	
	SetActorLocation(NewLoc);
}

void APersonaje::JumpCompleted()
{
	bJumping = false;
}

