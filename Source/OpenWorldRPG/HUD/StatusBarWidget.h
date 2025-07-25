// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class OPENWORLDRPG_API UStatusBarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void UpdateProgressBar(float NormalizedValue);

	UFUNCTION(BlueprintCallable)
	void SetStatusInfo(FText Label);
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock;
};
