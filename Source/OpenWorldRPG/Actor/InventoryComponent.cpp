
#include "InventoryComponent.h"

#include "OpenWorldRPG/Items/InventoryWidget.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

bool UInventoryComponent::AddItem(UItemBase* Item, int32 Quantity)
{

	UE_LOG(LogTemp, Display, TEXT("UInventoryComponent::AddItem - Adding item"));
	if (!Item || Quantity <= 0) return false;

	UE_LOG(LogTemp, Display, TEXT("UInventoryComponent::AddItem - Adding %d x %s "), Quantity,*Item->Name.ToString());
	
	for( FInventoryItem& Slot : Items)
	{
		UE_LOG(LogTemp, Display, TEXT("UInventoryComponent::AddItem - Checking slot with item: %s ( Quantity: %d/%d) "),
			*Slot.Item->GetName(), Slot.Quantity, Slot.Item->MaxStackSize);
		
		if ( Slot.CanStackWith({Item, Quantity}) && Slot.Quantity < Slot.Item->MaxStackSize )
		{
			int32 SpaceLeft = Slot.Item->MaxStackSize - Slot.Quantity;
			int ToAdd = FMath::Min( SpaceLeft, Quantity );
			UE_LOG(LogTemp, Display, TEXT("UInventoryComponent::AddItem - Found stackable slot. Before Update - Item: %s, Current: %d, Adding: %d, SpaceLeft: %d"),
				*Slot.Item->GetName(), Slot.Quantity, ToAdd, SpaceLeft);
			Slot.Quantity += ToAdd;
			Quantity -= ToAdd;
			
			UE_LOG(LogTemp, Display, TEXT("UInventoryComponent::AddItem - After stack - New Quantity %d/%d, Remaining to add: %d  "), 
				Slot.Quantity, Slot.Item->MaxStackSize, ToAdd);
			
			if  (Quantity <=0)
			{
				UE_LOG(LogTemp, Display, TEXT("UInventoryComponent::AddItem - All items stacked successfully"));
				return true;
			}
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("UInventoryComponent::AddItem - Slot Cannot stack - CanStackWith: %d, HasSpace: %d"),
				Slot.CanStackWith({Item, Quantity}), (Slot.Quantity< Slot.Item->MaxStackSize ));
		}
	}

	UE_LOG(LogTemp, Display, TEXT("UInventoryComponent::AddItem - No existing slots can stack, remaining quantity %d"), Quantity)

	while (Quantity > 0 && Items.Num() < MaxSlots)
	{
		int32 ToAdd = FMath::Min( Item->MaxStackSize, Quantity );
		UE_LOG(LogTemp, Display, TEXT("UInventoryComponent::AddItem Creating new slot for %s - Adding: %d, Remaining:%d"),
			*Item->GetName(), ToAdd, Quantity-ToAdd);
		Items.Add({Item, ToAdd});
		Quantity -= ToAdd;

		UE_LOG(LogTemp, Display, TEXT("UInventoryComponent::AddItem New Slot Created. Total slots noe: %d/%d"),
			Items.Num(), MaxSlots)
	}

	if ( Quantity > 0)
	{
		UE_LOG(LogTemp, Display, TEXT("UInventoryComponent::AddItem Could not add all items! Inventory full. Remaining: %d"), Quantity)
		return false;
	}
	UE_LOG(LogTemp, Display, TEXT("UInventoryComponent::AddItem All items added sucessfully"))
	return true;
}

bool UInventoryComponent::RemoveItem(UItemBase* Item, int32 Quantity)
{
	return false;
}

void UInventoryComponent::DropItem(int32 Index)
{
	
}

FInventoryItem UInventoryComponent::GetItem(int32 Index) const
{
	if (Items.IsValidIndex(Index))
		return Items[Index];
	{
	}
	return FInventoryItem();
}

void UInventoryComponent::ToggleInventory()
{

	if ( !GetOwner() ) return;
	APlayerController* PC = Cast<APlayerController>(GetOwner()->GetInstigatorController());
	if (!PC) return ;

	if ( !InventoryWidget)
	{
		InventoryWidget = CreateWidget<UInventoryWidget>(PC, InventoryWidgetClass);
		InventoryWidget->InitialiseInventory(this);
	}

	if ( InventoryWidget->IsInViewport())
	{
		InventoryWidget->RemoveFromParent();
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetShowMouseCursor(false);
	}else
	{
		InventoryWidget->AddToViewport();
		PC->SetInputMode(FInputModeGameAndUI());
		PC->SetShowMouseCursor(true);
		InventoryWidget->RefreshInventory();
	}
}


bool UInventoryComponent::UseItem(int32 Index)
{
    if (Items.IsValidIndex(Index))
    {
        FInventoryItem& Slot = Items[Index];
        if (Slot.Item && Slot.Quantity > 0)
        {
            if (Slot.Item->Use(GetOwner()))
            {
                Slot.Quantity--;
                if (Slot.Quantity <= 0)
                {
                    Items.RemoveAt(Index);
                }
                // Update UI if necessary
                return true;
            }
        }
    }
    return false;
}

