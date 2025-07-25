// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenWorldRPGPlayerController.h"

#include "Blueprint/UserWidget.h"

void AOpenWorldRPGPlayerController::SetInputModeToUIOnly(UUserWidget* WidgetToFocus)
{
	FInputModeUIOnly InputModeToUIOnly;

	if ( WidgetToFocus)
	{
		InputModeToUIOnly.SetWidgetToFocus(WidgetToFocus->TakeWidget());
	}
	InputModeToUIOnly.SetLockMouseToViewportBehavior((EMouseLockMode::DoNotLock));

	SetInputMode(InputModeToUIOnly);
	bShowMouseCursor = true;
	UE_LOG(LogTemp, Display, TEXT("Input mode set to UI Only"))
}

void AOpenWorldRPGPlayerController::SetInputModeToGameOnly()
{

	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
	bShowMouseCursor = false;

	UE_LOG(LogTemp, Display, TEXT("Input mode set to Game only"))
}
