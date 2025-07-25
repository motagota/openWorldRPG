// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHealthBarWidget.h"

#include "Components/ProgressBar.h"

void UEnemyHealthBarWidget::UpdateHealthDisplay(float CurrentHealth, float MaxHealth)
{
	HealthProgressBar->SetPercent((CurrentHealth/MaxHealth));
}

void UEnemyHealthBarWidget::UpdateStaminaDisplay(float CurrentStamina, float MaxStamina)
{
	StaminaProgressBar->SetPercent((CurrentStamina/MaxStamina));
}

void UEnemyHealthBarWidget::UpdateManaDisplay(float CurrentMana, float MaxMana)
{
	ManaProgressBar->SetPercent((CurrentMana/MaxMana));
}
