// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "StatusContainerWidget.generated.h"

/**
 * 
 */
UCLASS()
class OPENWORLDRPG_API UStatusContainerWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	FText GetLabelForTag(FGameplayTag StatusTag);
	
	UFUNCTION(BlueprintCallable)
	void ShowOrUpdateStatus(FGameplayTag StatusTag, float NormalizedValue);

	UPROPERTY(EditDefaultsOnly, Category = "UI|Status")
	TSubclassOf<class UStatusBarWidget> StatusBarWidgetClass;
protected:
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* StatusBarStack;

	UPROPERTY()
	TMap<FGameplayTag, UStatusBarWidget*> ActiveStatusBars;
	
};
