// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/LL_WorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"

void ULL_WorldUserWidget::NativeTick(const FGeometry& MovieSceneBlends, float InDeltaTime)
{
	Super::NativeTick(MovieSceneBlends, InDeltaTime);
	if(!IsValid(AttachedActor))
	{
		RemoveFromParent();
		return;
	}

	FVector2D ScreenPosition;
	if(UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(),AttachedActor->GetActorLocation()+WorldOffset,ScreenPosition))
	{
		//Calculate the screen size so it can compensate the aspect ratio
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPosition /= Scale;

		//Calculation for retaining inside screen
		if(ContainInsideWindow)
		{
			FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(UGameplayStatics::GetPlayerController(this,0));
            
          // float SP_X = FMath::Clamp(ScreenPosition.X + (OffsetPosition/2),0.0f,ViewportSize.X + FMath::TruncToInt(OffsetPosition));
          // float SP_Y = FMath::Clamp(ScreenPosition.Y + (OffsetPosition/2),0.0f,ViewportSize.Y + FMath::TruncToInt(OffsetPosition));
			//float SP_X = FMath::Clamp(ScreenPosition.X ,0.0f,(ViewportSize.X - OffsetPosition) );
			//float SP_Y = FMath::Clamp(ScreenPosition.Y ,0.0f,(ViewportSize.Y - OffsetPosition );
			float SP_X = FMath::Clamp(ScreenPosition.X ,0.0f,(ViewportSize.X / 2) );
			float SP_Y = FMath::Clamp(ScreenPosition.Y ,0.0f,(ViewportSize.Y/2) );
          
            FVector2D ScreenPositionFinal = FVector2D(SP_X,SP_Y);

			if(ParentSizeBox)
			{
				ParentSizeBox->SetRenderTranslation(ScreenPositionFinal);

			}

			return;
		}
		
		///
		///
		
		if(ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
			
		}

		
	}
}
