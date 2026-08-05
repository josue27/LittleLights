// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"

#include "LLGameUserSettings.generated.h"

/**
 * Custom GameUserSettings with Blueprint-friendly resolution management.
 * Registered in DefaultEngine.ini via GameUserSettingsClassName.
 */
UCLASS()
class LITTLELIGHTS_API ULLGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "LLGameSettings")
	static ULLGameUserSettings* GetLLGameUserSettings();
	/** Get list of supported fullscreen resolutions sorted largest-first. */
	UFUNCTION(BlueprintCallable, Category = "LLGameSettings")
	TArray<FIntPoint> GetAvailableResolutions() const;

	/** Get current screen resolution. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LLGameSettings")
	FIntPoint GetCurrentResolution() const;

	/** Get current window mode as int32: 0=Fullscreen, 1=WindowedFullscreen, 2=Windowed. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LLGameSettings")
	int32 GetCurrentWindowModeInt() const;

	/** One-call resolution + fullscreen change. Applies, confirms, and saves. */
	UFUNCTION(BlueprintCallable, Category = "LLGameSettings")
	void SetCustomResolution(int32 Width, int32 Height, bool bFullscreen);

	/** Apply pending settings, confirm video mode, and save to disk. */
	UFUNCTION(BlueprintCallable, Category = "LLGameSettings")
	void ApplyAndSave();

protected:
	virtual void SetToDefaults() override;
};
