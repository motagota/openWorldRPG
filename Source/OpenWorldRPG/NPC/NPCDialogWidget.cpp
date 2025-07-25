// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCDialogWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "BaseNPC.h" // Include BaseNPC.h

void UNPCDialogWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!GoodByeButton)
	{
		UE_LOG(LogTemp, Warning, TEXT("NPCDialogWidget: Goodbye button is not bound"));
		return;
	}

	if ( !NextButton)
	{
		UE_LOG(LogTemp, Warning, TEXT("NPCDialogWidget: NextButton is not bound"));
		return;
	}

	if (!DialogText)
	{
		UE_LOG(LogTemp, Warning, TEXT("NPCDialogWidget: DialogText is not bound correctly"));
		return;
	}

	GoodByeButton->OnClicked.Clear();
	GoodByeButton->OnClicked.AddDynamic(this, &UNPCDialogWidget::HandleGoodByeClicked);

	NextButton->OnClicked.Clear();
	NextButton->OnClicked.AddDynamic(this, &UNPCDialogWidget::HandleNextClicked);

	if (OptionsContainer)
	{
		OptionsContainer->ClearChildren();
	}
	
}

void UNPCDialogWidget::SetDialogText(const FText& NewText)
{
	if ( DialogText)
	{

		GetWorld()->GetTimerManager().ClearTimer(TypewriterTimer);

		FullText = NewText.ToString();
		CurrentCharIndex = 0;

		DialogText->SetText(FText::FromString(TEXT("")));

		// Start timer
		GetWorld()->GetTimerManager().SetTimer(
			TypewriterTimer,
			this,
			&UNPCDialogWidget::TypeNextLetter,
			0.02f, // Delay per letter (can adjust)
			true
		);
	}
}


void UNPCDialogWidget::HandleGoodByeClicked()
{
	OnGoodbye.Broadcast();
}

void UNPCDialogWidget::HandleNextClicked()
{
	OnNext.Broadcast();
}

void UNPCDialogWidget::TypeNextLetter()
{
	if (CurrentCharIndex >= FullText.Len())
	{
		GetWorld()->GetTimerManager().ClearTimer(TypewriterTimer);
		return;
	}

	FString CurrentText = FullText.Left(CurrentCharIndex + 1);
	DialogText->SetText(FText::FromString(CurrentText));
	++CurrentCharIndex;
}


void UNPCDialogWidget::SetDialogOptions(const TArray<FName>& Options)
{
	if (!OptionsContainer) 
	{
		UE_LOG(LogTemp, Warning, TEXT("NPCDialogWidget: OptionsContainer is null"));
		return;
	}

	OptionsContainer->ClearChildren();

	// Check if we have a widget class set
	if (!OptionWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("NPCDialogWidget: OptionWidgetClass is not set! Please set it in the Blueprint."));
		return;
	}

	for (int32 Index = 0; Index < Options.Num(); ++Index)
	{
		UNPCDialogOptionWidget* OptionWidget = CreateWidget<UNPCDialogOptionWidget>(this, OptionWidgetClass);
		if (OptionWidget)
		{
			OptionWidget->SetOptionText(Options[Index], Index);
			OptionWidget->OnOptionSelected.AddDynamic(this, &UNPCDialogWidget::HandleOptionSelected);
			OptionsContainer->AddChild(OptionWidget);
			
			UE_LOG(LogTemp, Log, TEXT("Added option widget with text: %s"), *Options[Index].ToString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create option widget for index %d"), Index);
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("SetDialogOptions completed. Added %d options"), Options.Num());
}

void UNPCDialogWidget::HandleOptionSelected(int32 OptionIndex)
{
	UE_LOG(LogTemp, Log, TEXT("Option selected: %d"), OptionIndex);
	OnOptionSelected.Broadcast(OptionIndex);
}