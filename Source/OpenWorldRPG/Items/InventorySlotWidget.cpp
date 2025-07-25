// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInventorySlotWidget::SetSlotData(const FInventoryItem& InSlotData, int InSlotIndex)
{
	SlotData = InSlotData;
	SlotIndex = InSlotIndex;
	if ( ItemIcon && SlotData.Item)
	{
		ItemIcon->SetBrushFromTexture(SlotData.Item->Icon);
	}

	if (QuantityText)
	{
		QuantityText->SetVisibility(SlotData.Quantity > 1? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		QuantityText->SetText(FText::AsNumber(SlotData.Quantity));
	}

	if ( ItemText)
	{
		ItemText->SetText(SlotData.Item->Name);
	}
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	UE_LOG(LogTemp, Display, TEXT("InventorySlotWidget::NativeOnMouseButtonDown - OnSlotClicked"));

	if ( MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		UE_LOG(LogTemp, Display, TEXT("InventorySlotWidget::NativeOnMouseButtonDown - Right Click"));

		if ( UWorld* World = GetWorld() )
		{
			APlayerController* PC = World->GetFirstPlayerController();
			if (PC)
			{
				FVector2D MousePosition;				
				UWidgetLayoutLibrary::GetMousePositionScaledByDPI(PC, MousePosition.X, MousePosition.Y);
				UInventoryContextMenuWidget* Menu = CreateWidget<UInventoryContextMenuWidget>(PC, ContextMenuClass);
				if (Menu)
				{
					UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PC, Menu);
					Menu->AddToViewport(10);

					Menu->InitWithItem(SlotData, SlotIndex);  // Updated to pass index
					Menu->SetPositionInViewport(MousePosition, false);
					Menu->SetKeyboardFocus();
				}

				return FReply::Handled();
			}
			else
			{
				return FReply::Unhandled();
			}
		}
	}
	return Super::NativeOnMouseButtonDown(Geometry, MouseEvent);
}

void UInventorySlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetIsEnabled(true);
	SetVisibility(ESlateVisibility::Visible);
	
}


void UInventorySlotWidget::HandleClick()
{
	OnSlotClicked();
}
