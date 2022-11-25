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
#include "LLComponents/LL_ToolsComponent.h"
#include "Camera/CameraComponent.h"
#include "LL_InteractorComponent.h"
#include "LL_PlayerState.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Perception/AISense_Hearing.h"
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
	
	TorchPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("TorhcSpawnPos"));
	TorchPosition->SetupAttachment(RootComponent);
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bInheritPitch = false;
	SpringArmComponent->bInheritRoll = false;
	SpringArmComponent->bInheritYaw = false;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComp_FillLight = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp_FillLight"));
	SpringArmComp_FillLight->bDoCollisionTest = false;
	SpringArmComp_FillLight->bInheritPitch = false;
	SpringArmComp_FillLight->bInheritRoll = false;
	SpringArmComp_FillLight->bInheritYaw = false;

	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComponent);
	FillLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("FillLight"));
	FillLight->SetupAttachment(SpringArmComp_FillLight);

	InteractorComp = CreateDefaultSubobject<ULL_InteractorComponent>(TEXT("InteractoComp"));
	AbilityComponent = CreateDefaultSubobject<ULL_AbilityComponent>(TEXT("AbilityComponent"));
	ToolsComponent = CreateDefaultSubobject<ULL_ToolsComponent>(TEXT("ToolsComponent"));
	ToolsComponent->PlayerCharacter = this;

	AIPerceptionStimuliSource = CreateDefaultSubobject< UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimuliSourceComponent"));

	bUseControllerRotationYaw = false;
	bIsAlive = true;
	bUpdateFov = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();


	

	//TODO:DELETE??
	FillLightInitRotation = FillLight->GetComponentRotation();

		
	//Bind delta orb remainig time to the FOV
	if (ToolsComponent)
	{
		ToolsComponent->OnOrbRemainingTimeChanged.AddDynamic(this, &APlayerCharacter::UpdateFov);
	}


	CurrentStamine = Stamine;

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;

		TimelineProgress.BindUFunction(this, FName("TimelineRoll_Progress"));

		CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);


	}
	stepsForSound = FMath::RandRange(MinStepsGivenForSound, MaxStepsGivenForSound);

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UpdateRotacion();
	
	CurveTimeline.TickTimeline(DeltaTime);

	//LookingAt();
	//UpdateFov();//DELETE?

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
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::ForwardMovement);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::RightMovement);
	PlayerInputComponent->BindAction(TEXT("InteractInput"), IE_Pressed, this, &APlayerCharacter::ActionButtonCall);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &APlayerCharacter::SprintAction);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &APlayerCharacter::SprintCancelled);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayerCharacter::JumpButtonCall);
	//PlayerInputComponent->BindAction(TEXT("Gamepad"))
}

//DEPRECATED
//This mechanic is no longer in use
int32 APlayerCharacter::GetBengalas()
{
	return BengalasDisponibles;
}

/// <summary>
/// DEPRECATED
/// Called when user press the action button to light up de torch if any
/// </summary>
void APlayerCharacter::SpawnLanternOrb()
{
	
}
/// <summary>
/// DEPRECATED
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

//DEPRECATED: moved to LL_ToolsComponent and LL_Orb
void APlayerCharacter::TorchLightingCompleted()
{
	

	if (Torch != nullptr)
	{
		TempRefillAmount = 30.0f;

		if(TempRefillAmount > 0.0f)
		{
			Torch->StartDecay(TempRefillAmount,false);
			ResetCameraPosition();
		}else
		{
			Torch->StartDecay(DefaultTorchDecay,false);
			ResetCameraPosition();
		}
		
	}
	bLightingTorch = false;
	bCanMove = true;
}

void APlayerCharacter::ResetCameraPosition_Implementation()
{
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
	ALL_PlayerState* PS = Cast<ALL_PlayerState>(GetPlayerState());
	if(PS)
	{
		bool Added = PS->AddTotemPiece();
	}
}

bool APlayerCharacter::IsPlayerAlive()
{
	return  bIsAlive;
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

void APlayerCharacter::ResetWalkSpeed(float speed)
{
	GetCharacterMovement()->MaxWalkSpeed = speed;
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

void APlayerCharacter::ForwardMovement(float AxisValue)
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


}

void APlayerCharacter::RightMovement(float AxisValue)
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


	
}

/// <summary>
/// DISABLED: we are using the "Interction" button binding, may we should remove this
/// When user press jump, it checks if its hitting an
/// </summary>
void APlayerCharacter::JumpButtonCall()
{
	/*if(InteractorComp)
	{
		InteractorComp->PrimaryInteract();
	}
	return;*/
	AbilityComponent->StartAbilityByName(this, "RollForward");

}

void APlayerCharacter::LookingAt()
{


}



void APlayerCharacter::CameraHorizontalRotation(float AxisValue)
{
	float RotationAmount = AxisValue * VelocidadRotacion * GetWorld()->GetDeltaSeconds();
	FRotator RotationZ = FRotator(0, RotationAmount, 0);
	EndRotation = FQuat(RotationZ);
	AddControllerYawInput(AxisValue * VelocidadRotacion * GetWorld()->GetDeltaSeconds());
	// AddActorLocalRotation(RotacionFinal,true);
}

void APlayerCharacter::UpdateRotation()
{
	AddActorLocalRotation(EndRotation, true);

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



void APlayerCharacter::UpdateFov(AActor* InstigatorActor, float DeltaRemainingTime)
{
	if (!bUpdateFov)return;
	//SpringArm
	SpringArmComponent->TargetOffset = GetActorForwardVector() * 120.0f;
	//Camera
	float CurrentFOV = FMath::Lerp(Fov_A, Fov_B, DeltaRemainingTime);
	CameraComp->SetFieldOfView(CurrentFOV);
	return;
	
}


void APlayerCharacter::StartGame_Implementation()
{
	bCanMove = true;
}


void APlayerCharacter::StepAnimationGiven()
{
	stepsGiven++;

	if (stepsGiven >= stepsForSound)
	{
		MakeStepNoise();
	}
}
void APlayerCharacter::MakeStepNoise()
{
	stepsGiven = 0;
	stepsForSound = FMath::RandRange(MinStepsGivenForSound, MaxStepsGivenForSound);
	
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(),10.f,this);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue,"Sound Made");
	}
}