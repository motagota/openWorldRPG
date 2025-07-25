// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"

#include "InventorySlotWidget.h"
#include "Components/UniformGridPanel.h"

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RefreshInventory();
}

void UInventoryWidget::InitialiseInventory(UInventoryComponent* InInventory)
{
	InventoryRef = InInventory;
	RefreshInventory();
}

void UInventoryWidget::RefreshInventory()
{
	if ( !InventoryRef || ! InventoryGrid || !InventorySlotWidgetClass)
	{
		return;
	}

	InventoryGrid->ClearChildren();

	const int32 NumCols =5;
	for ( int32 Index =0; Index < InventoryRef->Items.Num(); ++Index)
	{
		const FInventoryItem& Item = InventoryRef->Items[Index];
		UInventorySlotWidget* TheSlot = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);
		TheSlot->SetSlotData(Item, Index);

		int32 Row = Index /NumCols;
		int32 Col = Index % NumCols;
		InventoryGrid->AddChildToUniformGrid(TheSlot, Row, Col);
	}
}
/*
bool UInventoryComponent::RemoveItem(UItemBase* Item, int32 Quantity)
{
	if (!Item || Quantity <= 0) return false;
    
	for (int32 i = Items.Num() - 1; i >= 0; --i)
	{
		if (Items[i].Item == Item)
		{
			int32 ToRemove = FMath::Min(Items[i].Quantity, Quantity);
			Items[i].Quantity -= ToRemove;
			Quantity -= ToRemove;
            
			if (Items[i].Quantity <= 0)
			{
				Items.RemoveAt(i);
			}
            
			if (Quantity <= 0) return true;
		}
	}
	return false;
}
*/