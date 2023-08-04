// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LL_AbilityComponent.h"
#include "LL_GameplayInterface.h"
#include "Tottem_Piece.h"

#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/HUD.h"
#include "Components/TimelineComponent.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAutomaticMovementEnded, APlayerCharacter*, PlayerCharacter,bool,bLightUpTorch,bool,bStartDecay);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKeyPressed, FKey, KeyPressed);

class ATorch;
class UArrowComponent;
class AFirePit;
class ALevel_Manager_Base;
class UCurveFloat;
class USpringArmComponent;
class AJumpOverZone;
class ULL_InteractorComponent;
class ULL_ToolsComponent;
class UAIPerceptionStimuliSourceComponent;
class ALL_GameModeBase;
class ALL_AIBeast;


USTRUCT(BlueprintType)
struct FCameraOccludedActor
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	const AActor* Actor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh;
  
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UMaterialInterface*> Materials;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsOccluded;
};

UCLASS()
class LITTLELIGHTS_API APlayerCharacter : public ACharacter, public ILL_GameplayInterface
{
	GENERATED_BODY()

	virtual void StartGame_Implementation() override;
public:
	// Sets default values for this character's properties
	APlayerCharacter();


	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAutomaticMovementEnded OnAutomaticMovementEnded;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnKeyPressed OnKeyPressed;

	UFUNCTION(BlueprintCallable)
		void DisableInteraction(bool disable);

	UPROPERTY(BlueprintReadWrite)
		bool bOrbOff = false;

	UFUNCTION(BlueprintCallable)
	float PlayAnimation(UAnimMontage* AnimationToPlay);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Check "the turbo" he used this
	//FRunTimeFloatCurve
	//This is the timeline
	FTimeline CurveTimeline;
	//this is gonna be the curve to the timeline
	UPROPERTY(EditAnywhere, Category = "Jump Settings")
		UCurveFloat* CurveFloat;

	
	UFUNCTION()
		void RollForward();
	UFUNCTION()
		void TimelineRoll_Progress(float Value);
	UFUNCTION()
		void JumpCompleted();
	

	
	UPROPERTY(BlueprintReadWrite,Category="Player State")
	bool bIsAlive;//changed if captured o anything else;
	
	UFUNCTION( Category="LLPLayer| Player Camera")
	void UpdateFov(AActor* InstigatorActor, float DeltaRemainingTime);
	

	UPROPERTY(EditAnywhere, Category = "Jump Settings")
		float DelayForCompletedJump = 1.1f;
	UPROPERTY(EditAnywhere, Category = "Jump Settings")
		float JumpDistance = 5.0f;
	FTimerHandle DelayForJumpAnimation;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="LLPLayer|Player Camera")
	UCameraComponent* CameraComp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="LLPLayer|Player Camera")
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite ,Category="LLPLayer|Player Camera")
	USpotLightComponent* FillLight;
	UPROPERTY(BlueprintReadWrite,Category="LLPLayer|Player Camera")
	FRotator FillLightInitRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LL|Player Camera")
	USpringArmComponent* SpringArmComp_FillLight;
	UPROPERTY(EditAnywhere)
		bool bChangeArmLength = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLPLayer|Player Camera")
		float Fov_B = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLPLayer|Player Camera")
		float Fov_A = 45.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLPLayer|Player Camera")
		float ArmLength_A = 7000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLPLayer|Player Camera")
		float ArmLength_B = 5000.f;

	float FillLight_InitialOuterConeAngle;
	float FillLight_InitialInnerConeAngle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLPLayer|AI")
		UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSource;
	UPROPERTY(BlueprintreadWrite)
		ALL_GameModeBase* LLGameMode;
	UPROPERTY(BlueprintreadWrite)
		ALL_AIBeast* Beast;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="LLPLayer|Player Camera")
		bool bUpdateFov;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float VelocidadRotacion = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float VelocidadMovimiento = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UArrowComponent* TorchPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 BengalasDisponibles = 3;
	UFUNCTION(BlueprintPure)
		int32 GetBengalas();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanMove = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanSprint = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bStartWithLight = true;
	
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ATorch> TorchClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ATorch* Torch;
	UPROPERTY(EditAnywhere)
		float DefaultTorchDecay = 30.0f;
	UFUNCTION(BlueprintCallable)
		void LightUpTorch(float AmountRefill);// no need for implementation cause is BPImplementable
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bInFirePit = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsLightingUpTorch = false;//NOT USING
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int FragmentsAmount = 0;

	UPROPERTY(VisibleAnywhere)
	ULL_InteractorComponent* InteractorComp;
	//The difference that we rest to de movement in order to correspond
	//the movement from the joystick
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float JoystickAnlgeDifference = 45;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AFirePit* FirePitTemp = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DistanceRatio = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float JumpForce = 3000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Stamine = 10.0f;//TODO: this is not pretty we should use the MaxStamina from the SprintAbility
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentStamine = 0.0f;
	//Deprecated:using the ability settings
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
		// SprintVelocity = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NormalMaxVelocity = 30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bLightingTorch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bWithFlares;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLPLayer|Jump Settings")
		bool bCanJump = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLPLayer|Jump Settings")
		bool bJumping;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLPLayer|Jump Settings")
		float JumpDistanceDetection = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bSprint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsCrossingUnder;		

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UUserWidget* Gameplay_HUD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLPLayer|Hints")
		bool bShowHints = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USpringArmComponent* SpringArmRef = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLPLayer|Jump Over settings")
		bool bInJumpOverZone = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLPLayer|Jump Over settings")
		bool bJumpingOver = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLPLayer|Jump Over settings")
		AJumpOverZone* Temp_JumpOverZone = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLPLayer|Jump Over settings")
		bool bBalancing = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLPLayer|Jump Over settings Balancing")
		float Balancing_X;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLPLayer|Jump Over settings Balancing")
		float BalancingSensitivity = 0.01f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLPLayer|Jump Over settings Balancing")
		float UnbalanceVelocity = 0.001f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLPLayer|Jump Over settings Balancing")
		bool bLostBalance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLPLayer|Hints")
		FText TextHint;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="LLPLayer|Totems")
	TArray<ATottem_Piece*> TottemPieces;

	UFUNCTION(BlueprintCallable,Category="LLPLayer|Totems")
	void AddTottemPiece(ATottem_Piece* Piece);
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void PlayerCatchByMonster();

	UFUNCTION(BlueprintCallable)
	bool IsPlayerAlive();



	UFUNCTION(BlueprintImplementableEvent)
		void AddHUD();

	UFUNCTION(BlueprintImplementableEvent)
		void TorchOff();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ALevel_Manager_Base* LevelManager;

	UFUNCTION(BlueprintCallable)
		void StopCharacter();
	UFUNCTION(BlueprintCallable)
		void ContinueMovement();

	UFUNCTION(BlueprintCallable)
		void ActionButtonCall();
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
		void MovePlayerTo(FVector Location, float Speed = 150.0f,bool bNotify = false,bool bLightUpOrb = true,bool bStartOrbDecay = true);
	
	UFUNCTION(BlueprintCallable,Category="LLPLayer|LL_Player")
		void ResetWalkSpeed(float speed=400.0f);
#pragma region EspecialMovements

	UFUNCTION(BlueprintCallable)
		void JumpOver(class AJumpOverZone* TempZone);

	UFUNCTION(BlueprintCallable)
		void CrossBalancing(class AJumpOverZone* TempZone);

#pragma endregion EspacialMovements

	UFUNCTION()
		void ShowHint(bool bShowHint, const FString& textToShow);

	UFUNCTION(BlueprintCallable)
		void JumpButtonCall();

	/// <summary>
	/// Evaluates what is the PlayerCharacater is looking at so it can display something in HUD(or not)
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "LLPLayer|HUD")
		void LookingAt();

	UFUNCTION(BlueprintCallable, Category ="LLPLayer|Lantern")
		void SpawnLanternOrb();

	UFUNCTION(Category = "LLPLayer|Balancing")
		void BalanceUpdate();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void PlayerFall();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="LLPLayer|Abilities")
	ULL_AbilityComponent* AbilityComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLPlayer|Player Tools")
		ULL_ToolsComponent* ToolsComponent;
	

	UFUNCTION(BlueprintCallable)
		void TorchLightingCompleted();
	UFUNCTION(BlueprintNativeEvent)
		void ResetCameraPosition();

	UFUNCTION(BlueprintCallable)
		void StepAnimationGiven();

	UFUNCTION(BlueprintCallable)
		void MakeStepNoise();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLPlayer|AI")
		int32 MinStepsGivenForSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LLPlayer|AI")
		int32 MaxStepsGivenForSound;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bInfiniteSprint;

		
#pragma region Camera Oclussion
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion|Occlusion",
   meta=(ClampMin="0.1", ClampMax="10.0") )
	float CapsulePercentageForTrace;
  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion|Materials")
	UMaterialInterface* FadeMaterial;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion|Materials")
	// ECollisionChannel CollisionChannelToTrace ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion")
	bool IsOcclusionEnabled;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion|Occlusion")
	bool DebugLineTraces;
#pragma endregion
	
private:
	void ForwardMovement(float AxisValue);
	void RightMovement(float AxisValue);
	void CameraHorizontalRotation(float AxisValue);
	void UpdateRotation();

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> GameplayHUD_Class;


	FQuat EndRotation;

	int32 stepsGiven;
	int32 stepsForSound;

	float TempRefillAmount;
	friend class AMainPlayer_DebugHUD;

	FTimerHandle DelayLightingTorch;

	
	UFUNCTION()
		void SprintAction();
	UFUNCTION()
		void SprintCancelled();
#pragma region Camera Occlusion
	TMap<const AActor*, FCameraOccludedActor> OccludedActors;
  
	bool HideOccludedActor(const AActor* Actor);
	bool OnHideOccludedActor(const FCameraOccludedActor& OccludedActor) const;
	void ShowOccludedActor(FCameraOccludedActor& OccludedActor);
	bool OnShowOccludedActor(const FCameraOccludedActor& OccludedActor) const;
	void ForceShowOccludedActors();
	__forceinline bool ShouldCheckCameraOcclusion() const
	{
		return IsOcclusionEnabled && FadeMaterial && CameraComp && GetCapsuleComponent();
	}
  
public:
	UFUNCTION(BlueprintCallable)
	void SyncOccludedActors();
#pragma endregion 

};
