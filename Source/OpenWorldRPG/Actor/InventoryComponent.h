// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailTreeNode.h"
#include "Components/ActorComponent.h"
#include "OpenWorldRPG/Items/ItemBase.h"
#include "InventoryComponent.generated.h"

class UInventoryWidget;

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	UItemBase* Item;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Quantity;

	FInventoryItem() : Item(nullptr), Quantity(0) {}

	FInventoryItem(UItemBase* Item, int32 Quantity) : Item(Item), Quantity(Quantity) {}
	bool IsStackable() const
	{
		return Item && Item->MaxStackSize > 1;
	}
	bool CanStackWith(const FInventoryItem& Other) const
	{
		return Item->GetName() == Other.Item->GetName() && IsStackable();
	}
	
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPENWORLDRPG_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	int32 MaxSlots =30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory")
	TArray<FInventoryItem> Items;
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(UItemBase* Item, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(UItemBase* Item, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void DropItem(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FInventoryItem GetItem(int32 Index) const;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;
	
	UPROPERTY()
	class UInventoryWidget* InventoryWidget;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ToggleInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseItem(int32 Index);
};
