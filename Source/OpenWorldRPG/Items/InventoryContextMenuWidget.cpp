// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryContextMenuWidget.h"
#include "../Actor/InventoryComponent.h" 
#include "Components/Button.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/Character.h"


void UInventoryContextMenuWidget::HandleDrop()
{
	UE_LOG(LogTemp, Display, TEXT("UInventoryContextMenuWidget::HandleDrop - Drop %d x %s"),
		CurrentItem.Quantity, *CurrentItem.Item->GetName());
	CloseContextMenu();
}

void UInventoryContextMenuWidget::InitWithItem(FInventoryItem InItem, int32 InIndex)
{
	CurrentItem = InItem;
	ItemIndex = InIndex;

	if (UseButton && CurrentItem.Item)
	{
		APlayerController* PC = GetOwningPlayer();
		AActor* PlayerChar = PC ? PC->GetPawn() : nullptr;
		bool bCanUse = CurrentItem.Item->CanUse(PlayerChar);
        
		UseButton->SetIsEnabled(bCanUse);
	}
	else if (UseButton)
	{
		UseButton->SetIsEnabled(false);
	}
}

void UInventoryContextMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetIsFocusable(true);
	if (DropButton && !DropButton->OnClicked.IsAlreadyBound(this, &UInventoryContextMenuWidget::HandleDrop))
	{
		DropButton->OnClicked.AddDynamic(this, &UInventoryContextMenuWidget::HandleDrop);
	}
	if (UseButton && !UseButton->OnClicked.IsAlreadyBound(this, &UInventoryContextMenuWidget::HandleUse))
	{
		UseButton->OnClicked.AddDynamic(this, &UInventoryContextMenuWidget::HandleUse);
	}
	
}

void UInventoryContextMenuWidget::NativeDestruct()
{
	if (DropButton)
	{
		DropButton->OnClicked.RemoveDynamic(this, &UInventoryContextMenuWidget::HandleDrop);
	}
	if (UseButton)
	{
		UseButton->OnClicked.RemoveDynamic(this, &UInventoryContextMenuWidget::HandleUse);
	}
	Super::NativeDestruct();
}

void UInventoryContextMenuWidget::HandleUse()
{
	UE_LOG(LogTemp, Display, TEXT("UInventoryContextMenuWidget::HandleUse - Use %s"), *CurrentItem.Item->GetName());
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (ACharacter* PlayerChar = PC->GetCharacter())
		{
			if (UInventoryComponent* InvComp = PlayerChar->FindComponentByClass<UInventoryComponent>())
			{
				if (InvComp->UseItem(ItemIndex))
				{
					
				}
			}
		}
	}
	CloseContextMenu();
}
void UInventoryContextMenuWidget::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	UE_LOG(LogTemp, Display, TEXT("UInventoryContextMenuWidget::NativeOnFocusLost"));

	// Use a very short delay to allow button clicks to process
	if (GetWorld())
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			if (IsValid(this))
			{
				CloseContextMenu();
			}
		}, 0.25f, false); // 10ms delay
	}
}

void UInventoryContextMenuWidget::CloseContextMenu()
{
	RemoveFromParent();
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PC);
	}
}