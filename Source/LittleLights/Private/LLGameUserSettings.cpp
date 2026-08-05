// Fill out your copyright notice in the Description page of Project Settings.

#include "LLGameUserSettings.h"

#include "Containers/Array.h"
#include "Engine/Engine.h"
#include "Kismet/KismetSystemLibrary.h"

ULLGameUserSettings* ULLGameUserSettings::GetLLGameUserSettings()
{
	return Cast<ULLGameUserSettings>(ULLGameUserSettings::GetGameUserSettings());
}

TArray<FIntPoint> ULLGameUserSettings::GetAvailableResolutions() const
{
	TArray<FIntPoint> Resolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions);

	// Sort descending by total pixel count so largest resolutions appear first
	Resolutions.Sort([](const FIntPoint& A, const FIntPoint& B)
	{
		return (A.X * A.Y) > (B.X * B.Y);
	});

	return Resolutions;
}

FIntPoint ULLGameUserSettings::GetCurrentResolution() const
{
	return GetScreenResolution();
}

int32 ULLGameUserSettings::GetCurrentWindowModeInt() const
{
	return static_cast<int32>(GetFullscreenMode());
}

void ULLGameUserSettings::SetCustomResolution(int32 Width, int32 Height, bool bFullscreen)
{
	const FIntPoint NewResolution(Width, Height);
	SetScreenResolution(NewResolution);

	const EWindowMode::Type WindowMode = bFullscreen ? EWindowMode::Fullscreen : EWindowMode::Windowed;
	SetFullscreenMode(WindowMode);

	ApplyAndSave();

	UE_LOG(LogTemp, Log, TEXT("LLGameUserSettings: Resolution set to %dx%d, Fullscreen=%d"), Width, Height, bFullscreen);
}

void ULLGameUserSettings::ApplyAndSave()
{
	ApplySettings(true);
	ConfirmVideoMode();
	SaveSettings();
}

void ULLGameUserSettings::SetToDefaults()
{
	Super::SetToDefaults();

	// Default to native desktop resolution and fullscreen
	const FIntPoint NativeRes = GetDesktopResolution();
	SetScreenResolution(NativeRes);
	SetFullscreenMode(EWindowMode::Fullscreen);

	// Cap framerate at 60 by default for consistency
	SetFrameRateLimit(60.0f);
}
