// Fill out your copyright notice in the Description page of Project Settings.


#include "DataFragementsHUDWidget.h"

#include "OpenWorldRPG/Actor/DataFragmentManagerComponent.h"

void UDataFragementsHUDWidget::Init()
{
	FindCurrencyManager();
	if (CurrencyPlayerManager)
	{
		CurrencyPlayerManager->OnDataFragmentsChanged.AddDynamic(this, &UDataFragementsHUDWidget::HandleDataFragmentschanged);
		UpdateDataFragmentDisplay((CurrencyPlayerManager->GetCurrentDataFragments()),0);
	}
}

void UDataFragementsHUDWidget::HandleDataFragmentschanged(int32 NewAmount, int32 OldAmount)
{
	UE_LOG(LogTemp, Display, TEXT("UDataFragementsHUDWidget::HandleDataFragmentschanged"));
	UpdateDataFragmentDisplay(NewAmount,OldAmount);
	
	PlayAnimation(AddedFragmentsTextAnimation, 0.f, 1, EUMGSequencePlayMode::Forward);
	OnDataFragmentsUpdated(NewAmount, OldAmount);
}

void UDataFragementsHUDWidget::FindCurrencyManager()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (APawn* Pawn = PC->GetPawn())
		{
			CurrencyPlayerManager = Pawn->FindComponentByClass<UDataFragmentManagerComponent>();
		}
	}
}

void UDataFragementsHUDWidget::UpdateDataFragmentDisplay(int32 NewAmount, int OldAmount)
{
	if (DataFragementsText)
	{
		DataFragementsText->SetText(FText::AsNumber(NewAmount));
	}
	if (AddedFragmentsText)
	{
		AddedFragmentsText->SetText(FText::FromString( FString::Printf(TEXT(" + %d"), NewAmount-OldAmount )));
	}
}
