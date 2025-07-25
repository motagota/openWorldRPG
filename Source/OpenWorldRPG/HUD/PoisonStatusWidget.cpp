// Fill out your copyright notice in the Description page of Project Settings.


#include "PoisonStatusWidget.h"
#include "Components/ProgressBar.h"

void UPoisonStatusWidget::UpdateBar(float NormalizedValue)
{
	UE_LOG(LogTemp, Warning, TEXT("UPoisonStatusWidget::UpdateBar"));
	if (PoisonProgressBar)
	{
		PoisonProgressBar->SetPercent(NormalizedValue);
	}
}

void UPoisonStatusWidget::PlayPoisonedEffect()
{
}

void UPoisonStatusWidget::SetPoisonVisibility(bool bVisible)
{
	if (bVisible)
	{
		SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(FadeAnimation, 0.f, 1, EUMGSequencePlayMode::Forward);
	}
	else
	{
		PlayAnimation(FadeAnimation, 0.f, 1, EUMGSequencePlayMode::Reverse);

		// Optionally hide after fade-out completes
		FTimerHandle HideTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(HideTimerHandle, [this]()
		{
			SetVisibility(ESlateVisibility::Hidden);
		}, 0.3f, false);
	}
}