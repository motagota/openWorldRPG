// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PoisonStatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class OPENWORLDRPG_API UPoisonStatusWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateBar(float NormalizedValue);

	UFUNCTION(BlueprintCallable)
	void PlayPoisonedEffect();
	void SetPoisonVisibility(bool bVisible);

protected:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PoisonProgressBar;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* FadeAnimation;
	
};
