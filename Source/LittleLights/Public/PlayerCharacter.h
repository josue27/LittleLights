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
class ATorch;
class UArrowComponent;
class AFirePit;
class ALevel_Manager_Base;
class UCurveFloat;
class USpringArmComponent;
class AJumpOverZone;
class ULL_InteractorComponent;

UCLASS()
class LITTLELIGHTS_API APlayerCharacter : public ACharacter, public ILL_GameplayInterface
{
	GENERATED_BODY()

	virtual void StartGame_Implementation() override;
public:
	// Sets default values for this character's properties
	APlayerCharacter();

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
	
	UFUNCTION( Category="LL Player Camera")
	void UpdateFov();
	

	UPROPERTY(EditAnywhere, Category = "Jump Settings")
		float DelayForCompletedJump = 1.1f;
	UPROPERTY(EditAnywhere, Category = "Jump Settings")
		float JumpDistance = 5.0f;
	FTimerHandle DelayForJumpAnimation;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Player Camera")
	UCameraComponent* CameraComp;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Player Camera")
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite ,Category="Player Camera")
	float Fov_B = 60.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite ,Category="Player Camera")
	float Fov_A = 45.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite ,Category="Player Camera")
	USpotLightComponent* FillLight;
	UPROPERTY(BlueprintReadWrite,Category="Player Camera")
	FRotator FillLightInitRotation;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="LLPlayer Camera")
		bool bUpdateFov;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float VelocidadRotacion = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float VelocidadMovimiento = 1.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UArrowComponent* PosicionSpawnBengala;
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
		bool bFlaresEnabled = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bStartWithLight = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> Flare;
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
		float Stamine = 10.0f;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Settings")
		bool bCanJump = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Settings")
		bool bJumping;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Settings")
		float JumpDistanceDetection = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bSprint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsCrossingUnder;		

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UUserWidget* Gameplay_HUD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hints")
		bool bShowHints = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USpringArmComponent* SpringArmRef = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Over settings")
		bool bInJumpOverZone = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Over settings")
		bool bJumpingOver = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Over settings")
		AJumpOverZone* Temp_JumpOverZone = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Over settings")
		bool bBalancing = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Over settings Balancing")
		float Balancing_X;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Over settings Balancing")
		float BalancingSensitivity = 0.01f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Over settings Balancing")
		float UnbalanceVelocity = 0.001f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Over settings Balancing")
		bool bLostBalance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hints")
		FText TextHint;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Totems")
	TArray<ATottem_Piece*> TottemPieces;

	UFUNCTION(BlueprintCallable,Category="Totems")
	void AddTottemPiece(ATottem_Piece* Piece);
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void PlayerCatchByMonster();

	UFUNCTION(BlueprintCallable)
	bool IsPlayerAlive();

	UFUNCTION()
		void TorchLightDecay();

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
		void MovePlayerTo(FVector Location, float Speed = 150.0f);
	

#pragma region EspecialMovements

	UFUNCTION(BlueprintCallable)
		void JumpOver(class AJumpOverZone* TempZone);
	UFUNCTION(BlueprintCallable)
		void CrouchUnder(class AJumpOverZone* TempZone);
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
	UFUNCTION(BlueprintCallable, Category = "HUD")
		void LookingAt();

	UFUNCTION(BlueprintCallable, Category ="Lantern")
		void SpawnLanternOrb();

	UFUNCTION(Category = "Balancing")
		void BalanceUpdate();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void PlayerFall();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="Abilities")
	ULL_AbilityComponent* AbilityComponent;

	UFUNCTION(BlueprintCallable)
		void TorchLightingCompleted();
	UFUNCTION(BlueprintNativeEvent)
		void ResetCameraPosition();

private:
	void MovimientoForward(float AxisValue);
	void MovimientoRight(float AxisValue);
	void RotacionHorizontal(float AxisValue);
	void UpdateRotacion();
	UFUNCTION()
		void ShootFlare();



	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> GameplayHUD_Class;


	FQuat RotacionFinal;


	float TempRefillAmount;
	friend class AMainPlayer_DebugHUD;

	FTimerHandle DelayLightingTorch;

	
	UFUNCTION()
		void SprintAction();
	UFUNCTION()
		void SprintCancelled();
	

	

};
