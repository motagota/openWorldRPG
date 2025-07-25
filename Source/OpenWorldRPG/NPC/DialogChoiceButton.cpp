// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogChoiceButton.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "BaseNPC.h"

void UDialogChoiceButton::Setup(FText _ChoiceText, FName ChoiceID)
{
	MyChoiceID = ChoiceID;
	ChoiceText->SetText(_ChoiceText);

	ChoiceButton->OnClicked.AddDynamic(this, &UDialogChoiceButton::HandleClick);
}

void UDialogChoiceButton::HandleClick()
{
	OnChoiceButtonClicked.Broadcast(MyChoiceID);
}
