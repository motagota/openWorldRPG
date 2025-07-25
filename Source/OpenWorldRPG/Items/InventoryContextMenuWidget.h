

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OpenWorldRPG/Actor/InventoryComponent.h"
#include "InventoryContextMenuWidget.generated.h"


UCLASS()
class OPENWORLDRPG_API UInventoryContextMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* UseButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* DropButton;
	
	

	UFUNCTION(BlueprintCallable)
	void HandleDrop();

	UFUNCTION(BlueprintCallable)
	void HandleUse();
	
	UFUNCTION(BlueprintCallable)
	void InitWithItem(FInventoryItem InItem, int32 InIndex);
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;

	FInventoryItem CurrentItem;
	int32 ItemIndex;
	
	UFUNCTION()
	void CloseContextMenu();
};
