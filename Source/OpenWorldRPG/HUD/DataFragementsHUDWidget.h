// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DataFragementsHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class OPENWORLDRPG_API UDataFragementsHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void Init();
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DataFragementsText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* AddedFragmentsText;

	UPROPERTY(meta = (BindWidget))
	UImage* DataFragementsImage;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* AddedFragmentsTextAnimation;

	//UPROPERTY(meta = (BindWidget))
	//class UWidgetAnimation* FragmentGainAnimation;

	//UPROPERTY(meta = (BindWidget))
	//class UWidgetAnimation* FragmentSpendAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FLinearColor GainColor = FLinearColor::Green;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FLinearColor SpendColor = FLinearColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Style")
	FLinearColor NormalColor = FLinearColor::White;

	UFUNCTION(BlueprintImplementableEvent, Category="UI")
	void OnDataFragmentsUpdated(int32 NewAmount, int32 OldAmount);

	UFUNCTION()
	void HandleDataFragmentschanged(int32 NewAmount, int32 OldAmount);

	
	
private:
	UPROPERTY()
	class UDataFragmentManagerComponent* CurrencyPlayerManager;
	void FindCurrencyManager();	
	void UpdateDataFragmentDisplay(int32 Amount, int OldAmount);
};
