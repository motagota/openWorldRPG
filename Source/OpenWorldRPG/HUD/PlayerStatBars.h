// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatBars.generated.h"

/**
 * 
 */
UCLASS()
class OPENWORLDRPG_API UPlayerStatBars : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, Category="HealthBar")
	void UpdateHealthDisplay(float CurrentHealth, float MaxHealth);

	UFUNCTION(BlueprintCallable, Category="StaminaBar")
	void UpdateStaminaDisplay(float CurrentStamina, float MaxStamina);

	UFUNCTION(BlueprintCallable, Category="ManaBar")
	void UpdateManaDisplay(float CurrentMana, float MaxMana);
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UProgressBar* HealthProgressBar;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UProgressBar* ManaProgressBar;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UProgressBar* StaminaProgressBar;
};
