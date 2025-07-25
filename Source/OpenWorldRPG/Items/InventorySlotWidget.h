// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryContextMenuWidget.h"
#include "Blueprint/UserWidget.h"
#include "OpenWorldRPG/Actor/InventoryComponent.h"
#include "InventorySlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class OPENWORLDRPG_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, Category = "Context Menu")
	TSubclassOf<UInventoryContextMenuWidget> ContextMenuClass;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UImage* ItemIcon;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* QuantityText;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* ItemText;

	UPROPERTY(BlueprintReadOnly)
	FInventoryItem SlotData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 SlotIndex;

	UFUNCTION(BlueprintCallable)
	void SetSlotData(const FInventoryItem& InSlotData, int InSlotIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void OnSlotClicked();

	virtual FReply NativeOnMouseButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent) override;
protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void HandleClick();
};
