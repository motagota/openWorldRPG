// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatBars.h"
#include "Components/ProgressBar.h"



void UPlayerStatBars::UpdateHealthDisplay(float CurrentHealth, float MaxHealth)
{
	HealthProgressBar->SetPercent((CurrentHealth/MaxHealth));
}

void UPlayerStatBars::UpdateStaminaDisplay(float CurrentStamina, float MaxStamina)
{
	StaminaProgressBar->SetPercent((CurrentStamina/MaxStamina));
}

void UPlayerStatBars::UpdateManaDisplay(float CurrentMana, float MaxMana)
{
	ManaProgressBar->SetPercent((CurrentMana/MaxMana));
}

