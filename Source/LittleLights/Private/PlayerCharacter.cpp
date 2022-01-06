// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
//#include "FirePit.h"
#include "AIController.h"
#include "LittleLights/FirePit.h"
#include "Kismet/KismetMathLibrary.h"
#include "LittleLights/Torch.h"
#include "Components/TimelineComponent.h"
#include  "GameFramework/SpringArmComponent.h"
#include "JumpOverZone.h"
#include "Level_Manager_Base.h"
#include "DrawDebugHelpers.h"

#include "Camera/CameraComponent.h"
#include "LL_InteractorComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 540, 0);
	//Use controller rotation yaw = false en los class defaults
	PosicionSpawnBengala = CreateDefaultSubobject<UArrowComponent>(TEXT("PosicionSpawnBengala"));
	PosicionSpawnBengala->SetupAttachment(RootComponent);
	TorchPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("TorhcSpawnPos"));
	TorchPosition->SetupAttachment(RootComponent);
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bInheritPitch = false;
	SpringArmComponent->bInheritRoll = false;
	SpringArmComponent->bInheritYaw = false;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComponent);
	FillLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("FillLight"));
	FillLight->SetupAttachment(RootComponent);

	InteractorComp = CreateDefaultSubobject<ULL_InteractorComponent>(TEXT("InteractoComp"));
	AbilityComponent = CreateDefaultSubobject<ULL_AbilityComponent>(TEXT("AbilityComponent"));

	bUseControllerRotationYaw = false;
	bIsAlive = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (TorchClass != nullptr && bStartWithLight)
	{
		SpawnLanternOrb();
	}

	FillLightInitRotation = FillLight->GetComponentRotation();

		
	


	CurrentStamine = Stamine;

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;

		TimelineProgress.BindUFunction(this, FName("TimelineRoll_Progress"));

		CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);


	}
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UpdateRotacion();
	
	CurveTimeline.TickTimeline(DeltaTime);

	//LookingAt();
	UpdateFov();

	if (bBalancing)
	{
		Balancing_X += UnbalanceVelocity;
		if (Balancing_X <= -0.9f || Balancing_X >= 0.9)
		{
			bLostBalance = true;
			bBalancing = false;
			if (Temp_JumpOverZone)
			{
				Temp_JumpOverZone->PlayerLostBalance();
				PlayerFall();
			}
		}
		else
		{
			bLostBalance = false;

		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoverAdelante"), this, &APlayerCharacter::MovimientoForward);
	PlayerInputComponent->BindAxis(TEXT("MoverDerecha"), this, &APlayerCharacter::MovimientoRight);
	PlayerInputComponent->BindAction(TEXT("InputFlare"), IE_Pressed, this, &APlayerCharacter::ShootFlare);
	PlayerInputComponent->BindAction(TEXT("InteractInput"), IE_Pressed, this, &APlayerCharacter::ActionButtonCall);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &APlayerCharacter::SprintAction);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &APlayerCharacter::SprintCancelled);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayerCharacter::JumpButtonCall);

}


int32 APlayerCharacter::GetBengalas()
{
	return BengalasDisponibles;
}


void APlayerCharacter::SpawnLanternOrb()
{
	if (TorchClass == nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("No Torchclass specified, ignoring spawn of Lantern"))
		return;
	}
	FVector TorchPos = TorchPosition->GetComponentLocation();
	FRotator TorchRotation = TorchPosition->GetComponentRotation();
	//Torch = GetWorld()->SpawnActor<ATorch>(TorchClass, TorchPos, TorchRotation);
	Torch = GetWorld()->SpawnActor<ATorch>(TorchClass);
	Torch->SetOwner(this);
	//Torch->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	Torch->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("RightHandSocket"));
	//Torch->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	//Torch->SetActorLocationAndRotation(TorchPos, TorchRotation);
}
/// <summary>
/// Called when user press the action button to light up de torch if any
/// </summary>
void APlayerCharacter::LightUpTorch(float AmountRefill)
{
	/*if (bLightingTorch || !Torch)
	{
		
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Restarting Torch light"));
	bCanMove = false;//make function
	bLightingTorch = true;
	FRotator RotateTo = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FirePitTemp->GetActorLocation());
	RotateTo.Pitch = GetActorRotation().Pitch;
	RotateTo.Roll = GetActorRotation().Roll;
	SetActorRotation(RotateTo);*/
	
	//GetWorld()->GetTimerManager().SetTimer(DelayLightingTorch, this, &APlayerCharacter::TorchLightingCompleted, 4.0f, false);
	//TempRefillAmount = AmountRefill;
	
}
void APlayerCharacter::TorchLightingCompleted()
{

	if (Torch != nullptr)
	{
		TempRefillAmount = 30.0f;

		if(TempRefillAmount > 0.0f)
		{
			Torch->StartDecay(TempRefillAmount,true);
		}else
		{
			Torch->StartDecay(DefaultTorchDecay,true);
		}
		
	}
	bLightingTorch = false;
	bCanMove = true;
}

/// <summary>
/// Function Called by LightUpTorch after SetTimer just to let the animation play
/// </summary>


void APlayerCharacter::SprintAction()
{
	AbilityComponent->StartAbilityByName(this,"Sprint");
}

void APlayerCharacter::SprintCancelled()
{
	
	AbilityComponent->StopAbilityByName(this,"Sprint");

}




void APlayerCharacter::PlayerCatchByMonster_Implementation()
{
}

void APlayerCharacter::AddTottemPiece(ATottem_Piece* Piece)
{
	TottemPieces.AddUnique(Piece);
}

bool APlayerCharacter::IsPlayerAlive()
{
	return  bIsAlive;
}

void APlayerCharacter::TorchLightDecay()
{

	if (Torch == nullptr)
		return;

}

void APlayerCharacter::StopCharacter()
{
	bCanMove = false;
}

void APlayerCharacter::ContinueMovement()
{
	bCanMove = true;
}

void APlayerCharacter::ActionButtonCall()
{
	if(InteractorComp)
	{
		InteractorComp->PrimaryInteract();
	}
	return;
	if (bInJumpOverZone && Temp_JumpOverZone)
	{

		//JumpOver();
	}
}

void APlayerCharacter::JumpOver(class AJumpOverZone* TempZone)
{
	if (bJumpingOver) return;

	Temp_JumpOverZone = TempZone;

	bJumpingOver = true;//for animation BP
	bCanMove = false;
	//Jump();
	if (Temp_JumpOverZone)
		Temp_JumpOverZone->StartJumpOver(EspecialMovementZoneType::JumpOver, this);


}

void APlayerCharacter::ShowHint(bool showhint, const FString& textToShow)
{
	bShowHints = showhint;


	TextHint = textToShow != " " ? FText::FromString(textToShow) : FText::FromString("Press E for action");

}

void APlayerCharacter::BalanceUpdate()
{
}

void APlayerCharacter::MovimientoForward(float AxisValue)
{
	if (!bCanMove && bBalancing)
	{
		if (Temp_JumpOverZone)
		{
			UE_LOG(LogTemp, Warning, TEXT("Moving Forwad: %f"), AxisValue);
			Temp_JumpOverZone->MovePlayerAlongSpline(AxisValue);
		}

	}
	if ( !bCanMove)
		return;
	//TODO:Poner opcion para invertir el control para ver si eso soluciona que no tengamos que voltear el startplayer position
	FRotator Rotation = Controller->GetControlRotation();
	Rotation.Pitch = 0.0f;
	Rotation.Roll = 0.0f;
	 FRotator RotationWithDiff(0, Rotation.Yaw - JoystickAnlgeDifference, 0);
	// FVector Direction = FRotationMatrix(Yaw).GetUnitAxis(EAxis::X);
	// AddMovementInput(Direction, AxisValue * VelocidadMovimiento * GetWorld()->DeltaTimeSeconds);
	AddMovementInput(RotationWithDiff.Vector(), AxisValue * VelocidadMovimiento  * GetWorld()->DeltaTimeSeconds);

	//DEPRECATED(Because the initial configs in the BeginPlay Movement Calculations
	// if (SpringArmRef != nullptr)
	// {
	// 	SpringArmRef->TargetOffset = GetActorForwardVector() * 120.0f;
	// }
	//UE_LOG(LogTemp,Warning,TEXT("MOVIENDO"));
}

void APlayerCharacter::MovimientoRight(float AxisValue)
{
	if (!bCanMove && bBalancing)
	{
		
			Balancing_X += AxisValue * BalancingSensitivity;
		
	}
	if (!bCanMove)
	{

		return;
	}

	//BUG TO FIX: When pressing with 2 keyboard keys the velocity get higher(250) but with
	//analog stick it doesnt happen
	FRotator Rotation = Controller->GetControlRotation();
	Rotation.Pitch = 0.0f;
	Rotation.Roll = 0.0f;
	FRotator Yaw(0, Rotation.Yaw - JoystickAnlgeDifference, 0);
	//FVector Direction = FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y);
	FVector Direction = FRotationMatrix(Yaw).GetScaledAxis(EAxis::Y);//Esto segun TomLooman


	 AddMovementInput(Direction, AxisValue *  VelocidadMovimiento  * GetWorld()->DeltaTimeSeconds);
	

	//DEPRECATED(Because the initial configs in the BeginPlay and Movement Calculations
	// if (SpringArmRef != nullptr)
	// {
	// 	SpringArmRef->TargetOffset = GetActorForwardVector() * 120.0f;
	//}

	
}

/// <summary>
/// When user press jump, it doesn't jump , first cast a linetrace to see if is in front of a JumpOverZone(EspecialMovementZone) and there 
/// if it is, take different actions, jumpover, crouch, etc.
/// </summary>
void APlayerCharacter::JumpButtonCall()
{
	
	//Create LaneTrace to detect if in front of JumpOverObstacle || WalkUnderObstacle || BalancingObstalce
	FVector LineStart = GetActorLocation();
	FVector LineEnd = LineStart + (GetActorRotation().Vector() * JumpDistanceDetection);
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

		AJumpOverZone* EspecialMovementZone = Cast<AJumpOverZone>(Hit.GetActor());
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
			else if (EspecialMovementZone->MovementZone_Type == EspecialMovementZoneType::CrossBalancing)
			{
				UE_LOG(LogTemp, Warning, TEXT("In Balancing Zone"));

				CrossBalancing(EspecialMovementZone);
			}
			else
			{

				RollForward();
			}

		}
	}
	else
	{
		RollForward();

	}


	//else
		//RollForward
}

void APlayerCharacter::LookingAt()
{


}



void APlayerCharacter::RotacionHorizontal(float AxisValue)
{
	float CantidadRotacion = AxisValue * VelocidadRotacion * GetWorld()->GetDeltaSeconds();
	FRotator RotacionZ = FRotator(0, CantidadRotacion, 0);
	RotacionFinal = FQuat(RotacionZ);
	AddControllerYawInput(AxisValue * VelocidadRotacion * GetWorld()->GetDeltaSeconds());
	// AddActorLocalRotation(RotacionFinal,true);
}

void APlayerCharacter::UpdateRotacion()
{
	AddActorLocalRotation(RotacionFinal, true);

}

void APlayerCharacter::ShootFlare()
{
	if (!bFlaresEnabled || GetBengalas() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough flares or not enabled"));
		return;
	}
	if (Flare == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not flare set to be spawned"));

		return;
	}
	FVector PosSpawn = PosicionSpawnBengala->GetComponentLocation();
	FRotator RotSpawn = PosicionSpawnBengala->GetComponentRotation();
	AActor* SpawnedFlare = GetWorld()->SpawnActor<AActor>(Flare, PosSpawn, RotSpawn);

	BengalasDisponibles -= 1;
	UE_LOG(LogTemp, Warning, TEXT("Shooted Flare"));
}


void APlayerCharacter::RollForward()
{
	if (bJumping || !bCanJump)
	{
		return;
	}
	bJumping = true;


	//DEPRECATED: we change it to recieve a notify from animation
	/*CurveTimeline.PlayFromStart();
	GetWorld()->GetTimerManager().SetTimer(DelayForJumpAnimation, this, &APlayerCharacter::JumpCompleted, DelayForCompletedJump, false);*/
	//VelocidadMovimiento = NormalMaxVelocity;
}
void APlayerCharacter::CrouchUnder(AJumpOverZone* TempZone)
{
	if (bIsCrossingUnder) return;

	Temp_JumpOverZone = TempZone;

	bIsCrossingUnder = true;//for animation BP
	bCanMove = false;
	//Jump();
	if (Temp_JumpOverZone)
		Temp_JumpOverZone->StartJumpOver(EspecialMovementZoneType::Crouch, this);
}
void APlayerCharacter::CrossBalancing(AJumpOverZone* TempZone)
{
	/*Jump();*/
	bCanMove = false;
	UE_LOG(LogTemp, Warning, TEXT("Calling balancing"));
	Temp_JumpOverZone = TempZone;
	bBalancing = true;
	if (Temp_JumpOverZone)
		Temp_JumpOverZone->StartJumpOver(EspecialMovementZoneType::CrossBalancing, this);

}
void APlayerCharacter::TimelineRoll_Progress(float Value)
{

	FVector EndLoc = (GetActorForwardVector() * JumpDistance) + GetActorLocation();
	FVector NewLoc = FMath::Lerp(GetActorLocation(), EndLoc, Value);
	SetActorLocation(NewLoc);
}

void APlayerCharacter::JumpCompleted()
{
	bJumping = false;
}

// void APlayerCharacter::MovePlayerTo(FVector Location, float Speed)
// {
// 	APlayerController* PC = UGameplayStatics::GetPlayerController(this,0);
// 	if(!PC)return;
// 	DisableInput(PC);
// 	float MaxWalkSpeedTemp = GetCharacterMovement()->GetMaxSpeed();
// 	
// }



void APlayerCharacter::UpdateFov()
{

	
	if(!Torch || !CameraComp)
	{
		return;
	}
	float DeltaIntensity = Torch->DeltaIntensity;
	if(Torch->bStartDecay)//ojo startdecay se vuelve false cuando se acaba el fuego, checar esa logica porque si no lo camara se regresa
	{
		//SpringArm
		SpringArmComponent->TargetOffset = GetActorForwardVector() * 120.0f;
		//Camera
		float CurrentFOV = FMath::Lerp(Fov_A,Fov_B,DeltaIntensity);
		CameraComp->SetFieldOfView(CurrentFOV);
		
		//FillLight
		float ConeAngleLerp = FMath::Lerp(20.0f,40.0f,DeltaIntensity);
		FillLight->SetOuterConeAngle(ConeAngleLerp);
		FillLight->SetInnerConeAngle(ConeAngleLerp-5);
		FVector FillLightRelativeLoc = FillLight->GetRelativeLocation();
		 FillLightRelativeLoc.X = FMath::Lerp(0.0f,400.0f,DeltaIntensity);
		FillLight->SetRelativeLocation(FillLightRelativeLoc,false);
		FillLight->SetWorldRotation(FillLightInitRotation);
	}else
	{
		CameraComp->SetFieldOfView(Fov_B);
		
	}
	
	
}


void APlayerCharacter::StartGame_Implementation()
{
	bCanMove = true;
}
