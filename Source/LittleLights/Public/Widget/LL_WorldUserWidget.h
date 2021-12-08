// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "LL_WorldUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class LITTLELIGHTS_API ULL_WorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

	protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta=(BindWidget))
	USizeBox* ParentSizeBox;
	
	public:
	UPROPERTY(EditAnywhere,Category="UI")
	FVector WorldOffset;

	UPROPERTY(BlueprintReadWrite,Category="UI",meta=(ExposeOnSpawn=true))
	AActor* AttachedActor;
	
};
