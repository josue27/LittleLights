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
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Level_Manager_Base.h"
#include "DrawDebugHelpers.h"
#include "LLComponents/LL_ToolsComponent.h"
#include "Camera/CameraComponent.h"
#include "LL_InteractorComponent.h"
#include "LL_PlayerState.h"
#include "Abilities/LL_Sprint_Ability.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Perception/AISense_Hearing.h"
#include "AI/LL_AIBeast.h"
#include "Components/CapsuleComponent.h"
#include "LittleLights/LittleLights.h"
#include "LittleLights/LL_GameModeBase.h"
#include "Tools/LL_Orb.h"

//TODO: Debug sobre los estados de la bestia mejor tenerlos ahi en debug
//Debug de posible rutas de la bestia??
static TAutoConsoleVariable<bool> CVarShowBeastLoc(TEXT("ll.ShowDistancePlayerBeast"), false, TEXT("Enable debug ray to show direction and distance of the beast from player"), ECVF_Cheat);
static TAutoConsoleVariable<bool> CVarInfiniteSprint(TEXT("ll.InfiniteSprint"), false, TEXT("Disable sprint stamina so you can keep running"), ECVF_Cheat);

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
	SpringArmComp_FillLight->SetupAttachment(RootComponent);

	
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

	CapsulePercentageForTrace = 1.0f;
	DebugLineTraces = true;
	IsOcclusionEnabled = true;

	 MinStepsGivenForSound = 2;
	MaxStepsGivenForSound = 4;
	bInfiniteSprint =false;
	stepsForSound = 10;
}

float APlayerCharacter::PlayAnimation(UAnimMontage* AnimationToPlay)
{
	if(AnimationToPlay)
	{
		PlayAnimMontage(AnimationToPlay,1.f);
		
		return AnimationToPlay->GetPlayLength();
	}
	return 0;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();


	// if(bIsTutorialCharacter)
	// 	return;
	
		
	//Bind delta orb remainig time to the FOV
	if (ToolsComponent && ToolsComponent->Orb)
	{
		//ToolsComponent->OnOrbRemainingTimeChanged.AddDynamic(this, &APlayerCharacter::UpdateFov);
		UpdateFov(this, ToolsComponent->Orb->RemainingDeltaTime);
	}

	//Get MaxStamine for the player
	CurrentStamine = Stamine;//this is weird it should be deleted
	
	if(AbilityComponent)
	{
		
		if(ULL_Ability* SprintAbility = AbilityComponent->GetAbilityByName(FName("Sprint")))
		{
			if(ULL_Sprint_Ability* Sprint = Cast<ULL_Sprint_Ability>( SprintAbility))
			{
				Stamine = Sprint->MaxStamine;
			}
		}
	}
	
	
	//Took from example project from UE
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;

		TimelineProgress.BindUFunction(this, FName("TimelineRoll_Progress"));

		CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);


	}
	stepsForSound = FMath::RandRange(MinStepsGivenForSound, MaxStepsGivenForSound);

	if (FillLight)
	{
		FillLight_InitialOuterConeAngle = FillLight->OuterConeAngle;
		FillLight_InitialInnerConeAngle = FillLight->InnerConeAngle;
	}
	UpdateFov(this, 1.f);
	Beast = Beast == nullptr ? Cast<ALL_AIBeast>(UGameplayStatics::GetActorOfClass(GetWorld(), ALL_AIBeast::StaticClass())) : Beast;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UpdateRotacion();
	if(bIsTutorialCharacter)return;
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

	bInfiniteSprint = CVarInfiniteSprint.GetValueOnGameThread();
	bool ShowBestLoc = CVarShowBeastLoc.GetValueOnGameThread();
	if (ShowBestLoc)
	{
		LLGameMode = LLGameMode == nullptr ? Cast<ALL_GameModeBase>(UGameplayStatics::GetGameMode(this)) : LLGameMode;
		if (LLGameMode && Beast)
		{
			
			DrawDebugLine(GetWorld(), GetActorLocation(), Beast->GetActorLocation(),FColor::Orange,false);
		}
		else
		{
			Beast = Beast == nullptr ? Cast<ALL_AIBeast>(UGameplayStatics::GetActorOfClass(GetWorld(), ALL_AIBeast::StaticClass())) : Beast;

		}
	}
	if (ToolsComponent )
	{
		
		UpdateFov(this, ToolsComponent->GetDeltaRemainOrb());
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// PlayerInputComponent->BindAction(TEXT("InteractInput"), IE_Pressed, this, &APlayerCharacter::ActionButtonCall);
	// PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &APlayerCharacter::SprintAction);
	// PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &APlayerCharacter::SprintCancelled);
	// PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayerCharacter::JumpButtonCall);
	//PlayerInputComponent->BindAction(TEXT("Gamepad"))

	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);
	
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	Input->BindAction(MoveForward_IA,ETriggerEvent::Triggered,this,&APlayerCharacter::Movement);
	Input->BindAction(Interact_IA,ETriggerEvent::Triggered,this,&APlayerCharacter::ActionButtonCall);

	Input->BindAction(Sprint_IA,ETriggerEvent::Triggered, this, &APlayerCharacter::SprintAction);
	Input->BindAction(Sprint_IA, ETriggerEvent::Completed, this, &APlayerCharacter::SprintCancelled);//Might Be wrong
	Input->BindAction(Jump_IA, ETriggerEvent::Triggered, this, &APlayerCharacter::JumpButtonCall);
	Input->BindAction(AdvanceDialogue_IA, ETriggerEvent::Triggered, this, &APlayerCharacter::AdvanceDialogueCall);
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


void APlayerCharacter::SprintAction(const FInputActionValue& Value)
{
	if(Value.Get<bool>())
	{
		
		AbilityComponent->StartAbilityByName(this,"Sprint");
	}
}

void APlayerCharacter::SprintCancelled(const FInputActionValue& Value)
{
	if(!Value.Get<bool>())
	{
		AbilityComponent->StopAbilityByName(this,"Sprint");
	}

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

void APlayerCharacter::AdvanceDialogueCall(const FInputActionValue& Value)
{
	OnDialogueAdvance.Broadcast();
}

void APlayerCharacter::ActionButtonCall(const FInputActionValue& Value)
{
	const bool bPressed = Value.Get<bool>();
	if(InteractorComp && bPressed)
	{
		InteractorComp->PrimaryInteract();
	}
	
}

void APlayerCharacter::ResetWalkSpeed(float speed)
{
	GetCharacterMovement()->MaxWalkSpeed = speed;
}

void APlayerCharacter::JumpOver(class AJumpOverZone* TempZone)
{
	if (bJumpingOver || !bCanMove) return;

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
void APlayerCharacter::Movement(const FInputActionValue& Value)
{
	if(IsValid(Controller))
	{
		FVector2D InputVector = Value.Get<FVector2D>();

		//Add forward direction
		FRotator Rotator = Controller->GetControlRotation();
		FRotator YawRotation = FRotator(0,Rotator.Yaw - JoystickAnlgeDifference,0);
		FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X).GetSafeNormal();
		FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y).GetSafeNormal();

		

		AddMovementInput(ForwardDirection,InputVector.Y * (VelocidadMovimiento * GetWorld()->DeltaTimeSeconds));
		AddMovementInput(RightDirection,InputVector.X * (VelocidadMovimiento * GetWorld()->DeltaTimeSeconds));

		// FString DebugText = FString::Printf(TEXT("Forward Direction; %s Right Direction: %s Axisvalue: %s, Rotationyas %f "),*ForwardDirection.ToString(),*RightDirection.ToString(),*InputVector.ToString(),Rotation.Yaw);
		// LogOnScreen(GetWorld(),DebugText);

	}
}
void APlayerCharacter::ForwardMovement(const FInputActionValue& Value)
{
	float AxisValue= Value.GetMagnitude();
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
	//float CurrentVelocity = FMath::Max(VelocidadMovimiento,AxisValue *  VelocidadMovimiento  * GetWorld()->DeltaTimeSeconds);
	FVector Direction = FRotationMatrix(RotationWithDiff).GetScaledAxis(EAxis::X).GetSafeNormal();
	//FVector Direction = FVector(0.f,0.f,Rotation.Yaw);
	//AddMovementInput(Direction, CurrentVelocity);
	//AddMovementInput(Direction, AxisValue * VelocidadMovimiento  * GetWorld()->DeltaTimeSeconds);

	// FString DebugText = FString::Printf(TEXT("Forward Direction; %s Axisvalue: %f, Rotationyas %f"),*Direction.ToString(),AxisValue,Rotation.Yaw);
	// LogOnScreen(GetWorld(),DebugText);

	FVector2D InputVector = Value.Get<FVector2D>();

	if(IsValid(Controller))
	{
		//Add forward direction
		FRotator Rotator = Controller->GetControlRotation();
		FRotator YawRotation = FRotator(0,Rotator.Yaw - JoystickAnlgeDifference,0);
		FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X).GetSafeNormal();
		FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y).GetSafeNormal();

		

		AddMovementInput(ForwardDirection,InputVector.Y * (VelocidadMovimiento * GetWorld()->DeltaTimeSeconds));
		AddMovementInput(RightDirection,InputVector.X * (VelocidadMovimiento * GetWorld()->DeltaTimeSeconds));

		FString DebugText = FString::Printf(TEXT("Forward Direction; %s Right Direction: %s Axisvalue: %s, Rotationyas %f "),*ForwardDirection.ToString(),*RightDirection.ToString(),*InputVector.ToString(),Rotation.Yaw);
		LogOnScreen(GetWorld(),DebugText);

	}
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
	FVector Direction = FRotationMatrix(Yaw).GetScaledAxis(EAxis::Y).GetSafeNormal();//Esto segun TomLooman

	//float CurrentVelocity = FMath::Max(VelocidadMovimiento,AxisValue *  VelocidadMovimiento  * GetWorld()->DeltaTimeSeconds);

	 AddMovementInput(Direction, AxisValue *  VelocidadMovimiento  * GetWorld()->DeltaTimeSeconds);
	// AddMovementInput(Direction, CurrentVelocity);

	FString DebugText = FString::Printf(TEXT("Forward Direction; %s Axisvalue: %f, Rotationyas %f"),*Direction.ToString(),AxisValue,Rotation.Yaw);
	LogOnScreen(GetWorld(),DebugText);


	
}

/// <summary>
/// DISABLED: we are using the "Interction" button binding, may we should remove this
/// When user press jump, it checks if its hitting an
/// </summary>
void APlayerCharacter::JumpButtonCall(const FInputActionValue& Value)
{
	if(!bCanMove)return;
	
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
	bOrbOff = DeltaRemainingTime < 0.1f;
	if (!bUpdateFov)return;
	//SpringArm
	SpringArmComponent->TargetOffset = GetActorForwardVector() * 120.0f;
	if (bChangeArmLength && SpringArmComponent)
	{
		float CurrentArmLength = FMath::Lerp(ArmLength_A, ArmLength_B, DeltaRemainingTime);
		SpringArmComponent->TargetArmLength = CurrentArmLength;
	}
	else
	{

		//Camera
		float CurrentFOV = FMath::Lerp(Fov_A, Fov_B, DeltaRemainingTime);
		CameraComp->SetFieldOfView(CurrentFOV);
	}

	if (FillLight)
	{
		//DeltaTimeRemainingTime
		//ArmLength_B = OuterConeAngle
		//	ArmLenght_A = FinalOuterConeAngle
		float FinalOuterConeAngle = (FillLight_InitialOuterConeAngle * ArmLength_A) / ArmLength_B;
		FillLight->OuterConeAngle = FMath::Lerp( FinalOuterConeAngle, FillLight_InitialOuterConeAngle, DeltaRemainingTime);
		float FinalInnerConeAngle = (FillLight_InitialInnerConeAngle * ArmLength_A) / ArmLength_B;
		FillLight->InnerConeAngle = FMath::Lerp( FinalInnerConeAngle, FillLight_InitialInnerConeAngle, DeltaRemainingTime);
		
	}
	
	
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

void APlayerCharacter::DisableInteraction(bool disable)
{
	if (InteractorComp)
	{
		InteractorComp->bCanInteract = !disable;
	}
}

bool APlayerCharacter::HideOccludedActor(const AActor* Actor)
{
	FCameraOccludedActor* ExistingOccludedActor = OccludedActors.Find(Actor);
	if (ExistingOccludedActor && ExistingOccludedActor->IsOccluded)
	{
		if (DebugLineTraces) UE_LOG(LogTemp, Warning, TEXT("Actor %s was already occluded. Ignoring."),
									*Actor->GetName());
		return false;
	}
	if (ExistingOccludedActor && IsValid(ExistingOccludedActor->Actor))
	{
		ExistingOccludedActor->IsOccluded = true;
		OnHideOccludedActor(*ExistingOccludedActor);
		if (DebugLineTraces) UE_LOG(LogTemp, Warning, TEXT("Actor %s exists, but was not occluded. Occluding it now."), *Actor->GetName());
	}
	else
	{
		UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(
		  Actor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		FCameraOccludedActor OccludedActor;
		OccludedActor.Actor = Actor;
		OccludedActor.StaticMesh = StaticMesh;
		OccludedActor.Materials = StaticMesh->GetMaterials();
		OccludedActor.IsOccluded = true;
		OccludedActors.Add(Actor, OccludedActor);
		OnHideOccludedActor(OccludedActor);
		if (DebugLineTraces) UE_LOG(LogTemp, Warning, TEXT("Actor %s does not exist, creating and occluding it now."), *Actor->GetName());
	}
	return true;
}

bool APlayerCharacter::OnHideOccludedActor(const FCameraOccludedActor& OccludedActor) const
{
	for (int i = 0; i < OccludedActor.StaticMesh->GetNumMaterials(); ++i)
	{
		OccludedActor.StaticMesh->SetMaterial(i, FadeMaterial);
	}
	return true;
}

void APlayerCharacter::ShowOccludedActor(FCameraOccludedActor& OccludedActor)
{
	if (!IsValid(OccludedActor.Actor))
	{
		OccludedActors.Remove(OccludedActor.Actor);
	}
	OccludedActor.IsOccluded = false;
	OnShowOccludedActor(OccludedActor);
}

bool APlayerCharacter::OnShowOccludedActor(const FCameraOccludedActor& OccludedActor) const
{
	for (int matIdx = 0; matIdx < OccludedActor.Materials.Num(); ++matIdx)
	{
		OccludedActor.StaticMesh->SetMaterial(matIdx, OccludedActor.Materials[matIdx]);
	}
	return true;
}

void APlayerCharacter::ForceShowOccludedActors()
{
	for (auto& Elem : OccludedActors)
	{
		if (Elem.Value.IsOccluded)
		{
			ShowOccludedActor(Elem.Value);
			if (DebugLineTraces) UE_LOG(LogTemp, Warning, TEXT("Actor %s was occluded, force to show again."), *Elem.Value.Actor->GetName());
		}
	}
}

/**
 * @brief Called by a timer on BP it occludes all actors between the camera and character that are marked in the tracechannel and
 */
void APlayerCharacter::SyncOccludedActors()
{
	if (!ShouldCheckCameraOcclusion()) return;
	// Camera is currently colliding, show all current occluded actors
	// and do not perform further occlusion
	if (SpringArmComponent->bDoCollisionTest)
	{
		ForceShowOccludedActors();
		return;
	}
	FVector Start = CameraComp->GetComponentLocation();
	FVector End = GetActorLocation();
	TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypes;
	
	CollisionObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3));//OccludedObject channel
	TArray<AActor*> ActorsToIgnore; // TODO: Add configuration to ignore actor types
	TArray<FHitResult> OutHits;
	auto ShouldDebug = DebugLineTraces ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	bool bGotHits = UKismetSystemLibrary::CapsuleTraceMultiForObjects(
	  GetWorld(), Start, End, GetCapsuleComponent()->GetScaledCapsuleRadius() * CapsulePercentageForTrace,
	  GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * CapsulePercentageForTrace, CollisionObjectTypes, true,
	  ActorsToIgnore,
	  ShouldDebug,
	  OutHits, true);
	if (bGotHits)
	{
		// The list of actors hit by the line trace, that means that they are occluded from view
		TSet<const AActor*> ActorsJustOccluded;
		// Hide actors that are occluded by the camera
		for (FHitResult Hit : OutHits)
		{
			const AActor* HitActor = Cast<AActor>(Hit.GetActor());
			HideOccludedActor(HitActor);
			ActorsJustOccluded.Add(HitActor);
		}
		// Show actors that are currently hidden but that are not occluded by the camera anymore 
		for (auto& Elem : OccludedActors)
		{
 			{
				ShowOccludedActor(Elem.Value);
				if (DebugLineTraces)
				{
					UE_LOG(LogTemp, Warning,
						   TEXT("Actor %s was occluded, but it's not occluded anymore with the new hits."), *Elem.Value.Actor->GetName());
				}
			}
		}
	}
	else
	{
		ForceShowOccludedActors();
	}
}


