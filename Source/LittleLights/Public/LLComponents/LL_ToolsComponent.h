// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "LL_ToolsComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOrbRemainingTimeChanged, AActor*, InstigatorActor, float, RemainingDuration);


class ALL_Orb;
class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LITTLELIGHTS_API ULL_ToolsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULL_ToolsComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LL Tools | Orb")
		TSubclassOf<ALL_Orb> OrbClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LL Tools | Orb")
		ALL_Orb* Orb;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LL Tools | Orb")
		bool bStartWithOrb;
protected:

	APlayerCharacter* PlayerCharacter;

	// Called when the game starts
	virtual void BeginPlay() override;

	friend class APlayerCharacter;//carefull

	float TempRefillAmount;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#pragma region Orb
	UFUNCTION(BlueprintCallable, Category = "LL Tools | Orb")
	void SpawnOrb();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "LL Tools | Orb")
		FOnOrbRemainingTimeChanged OnOrbRemainingTimeChanged;

	void StartOrbDecay();
	UFUNCTION(BlueprintCallable, Category = "LL Tools | Orb")
		void StartOrbRefill(float Amount = 30.f);
	UFUNCTION(BlueprintCallable, Category = "LL Tools | Orb")
		void RefillOrb(float Amount=30.f,bool bStartDecay = true);

	UFUNCTION(BlueprintCallable, Category = "LL Tools | Orb")
		void OrbRefillFinished();

	UFUNCTION(BlueprintCallable, Category = "LL Tools | Orb")
		float GetDeltaRemainOrb();

#pragma endregion
};
