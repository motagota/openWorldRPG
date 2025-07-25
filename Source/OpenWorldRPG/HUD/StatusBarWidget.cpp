// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UStatusBarWidget::UpdateProgressBar(float NormalizedValue)
{
	UE_LOG(LogTemp, Warning, TEXT("UStatusBarWidget::UpdateProgressBar"));
	if (ProgressBar)
	{
		ProgressBar->SetPercent(NormalizedValue);
	}	
}

void UStatusBarWidget::SetStatusInfo(FText Label)
{
	UE_LOG(LogTemp, Warning, TEXT("UStatusBarWidget::SetStatusInfo"));

	if ( TextBlock)
	{
		TextBlock->SetText(Label);
	}
}
