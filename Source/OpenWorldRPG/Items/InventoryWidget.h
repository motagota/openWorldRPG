#pragma once

#include "CoreMinimal.h"
#include "InventorySlotWidget.h"
#include "Blueprint/UserWidget.h"
#include "OpenWorldRPG/Actor/InventoryComponent.h"
#include "InventoryWidget.generated.h"


UCLASS()
class OPENWORLDRPG_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, meta = ( BindWidget))
	class UUniformGridPanel* InventoryGrid;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventorySlotWidget> InventorySlotWidgetClass;

	UFUNCTION(BlueprintCallable)
	void InitialiseInventory(UInventoryComponent* InInventory);

	UFUNCTION(BlueprintCallable)
	void RefreshInventory();

	protected:
	UPROPERTY()
	UInventoryComponent* InventoryRef;

	virtual void NativeOnInitialized() override;

};
