// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusContainerWidget.h"

#include "GameplayTagContainer.h"
#include "StatusBarWidget.h"
#include "Components/VerticalBox.h"



void UStatusContainerWidget::ShowOrUpdateStatus(FGameplayTag StatusTag, float NormalizedValue)
{

	UE_LOG(LogTemp, Warning, TEXT("UStatusContainerWidget::ShowOrUpdateStatus StatusTag %s, NormalizedValue %f"), *StatusTag.ToString(), NormalizedValue);

	if ( !StatusBarWidgetClass) return;

	UE_LOG(LogTemp, Warning, TEXT("UStatusContainerWidget::ShowOrUpdateStatus StatusBarWidgetClass "));

	UStatusBarWidget** Found = ActiveStatusBars.Find(StatusTag);;
	UStatusBarWidget* NewStatusBar = nullptr;
	if ( !Found)
	{
		NewStatusBar = CreateWidget<UStatusBarWidget>(GetWorld(),StatusBarWidgetClass );
		NewStatusBar->SetStatusInfo(GetLabelForTag(StatusTag));
		StatusBarStack->AddChildToVerticalBox(NewStatusBar);
		ActiveStatusBars.Add(StatusTag, NewStatusBar);
	}else
	{
		NewStatusBar = *Found;
	}

	NewStatusBar->UpdateProgressBar(NormalizedValue);
}
FText UStatusContainerWidget::GetLabelForTag(FGameplayTag StatusTag)
{
	return FText::FromName(StatusTag.GetTagName());
}